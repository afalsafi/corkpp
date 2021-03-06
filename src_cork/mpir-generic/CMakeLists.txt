project (mpir-generic C)

cmake_minimum_required (VERSION 2.8.6)

include_directories("./")
include_directories("./fft")
include_directories("./mpf")
include_directories("./mpq")
include_directories("./mpn/generic")
include_directories("./mpz")
include_directories("./printf")
include_directories("./scanf")

file(
	GLOB mpir_SRC
	"./*.h"
	"./*.c"
	"./fft/*.c"
	"./mpf/*.c"
	"./mpn/generic/*.c"
	"./mpq/*.c"
	"./mpz/*.c"
	"./printf/*.c"
	"./scanf/*.c"
)

file(
	GLOB public_headers
	"./*.h"
)


set_source_files_properties(
${mpir_SRC}
PROPERTIES
COMPILER_FLAGS "-Wno-deprecated"
)

add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D_LIB)
# if(WIN32)
# 	add_definitions(-D_WIN64) #usually building for x64
# endif()

add_library(mpir ${mpir_SRC})

#install public facing headers and lib
# install (TARGETS mpir DESTINATION ${PROJECT_SOURCE_DIR}/lib)
# install (FILES ${public_headers} DESTINATION ${PROJECT_SOURCE_DIR}/include)

