/* mpn_dc_bdiv_qr -- divide-and-conquer Hensel division with precomputed
   inverse, returning quotient and remainder.

   Contributed to the GNU project by Niels M�ller and Torbjorn Granlund.

   THE FUNCTIONS IN THIS FILE ARE INTERNAL WITH MUTABLE INTERFACES.  IT IS ONLY
   SAFE TO REACH THEM THROUGH DOCUMENTED INTERFACES.  IN FACT, IT IS ALMOST
   GUARANTEED THAT THEY WILL CHANGE OR DISAPPEAR IN A FUTURE GMP RELEASE.

Copyright 2006, 2007, 2009, 2010 Free Software Foundation, Inc.

Copyright 2010 William Hart (minor modifications)

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library.  If not, see http://www.gnu.org/licenses/.  */

#include "mpir.h"
#include "gmp-impl.h"

/* Computes Hensel binary division of {np, 2*n} by {dp, n}.

   Output:

      q = n * d^{-1} mod 2^{qn * GMP_NUMB_BITS},

      r = (n - q * d) * 2^{-qn * GMP_NUMB_BITS}

   Stores q at qp. Stores the n least significant limbs of r at the high half
   of np, and returns the borrow from the subtraction n - q*d.

   d must be odd. dinv is d^-1 mod 2^GMP_NUMB_BITS. 
*/
mp_limb_t
mpn_dc_bdiv_qr_n (mp_ptr qp, mp_ptr np, mp_srcptr dp, mp_size_t n,
		     mp_limb_t dinv, mp_ptr tp)
{
  mp_size_t lo, hi;
  mp_limb_t cy;
  mp_limb_t rh;

  lo = n >> 1;	/* floor(n/2) */
  hi = n - lo;	/* ceil(n/2) */

  if (BELOW_THRESHOLD (lo, DC_BDIV_QR_THRESHOLD))
    cy = mpn_sb_bdiv_qr (qp, np, 2 * lo, dp, lo, dinv);
  else
    cy = mpn_dc_bdiv_qr_n (qp, np, dp, lo, dinv, tp);

  mpn_mul (tp, dp + lo, hi, qp, lo);

  mpn_incr_u (tp + lo, cy);
  rh = mpn_sub (np + lo, np + lo, n + hi, tp, n);

  if (BELOW_THRESHOLD (hi, DC_BDIV_QR_THRESHOLD))
    cy = mpn_sb_bdiv_qr (qp + lo, np + lo, 2 * hi, dp, hi, dinv);
  else
    cy = mpn_dc_bdiv_qr_n (qp + lo, np + lo, dp, hi, dinv, tp);

  mpn_mul (tp, qp + lo, hi, dp + hi, lo);

  mpn_incr_u (tp + hi, cy);
  rh += mpn_sub_n (np + n, np + n, tp, n);

  return rh;
}
