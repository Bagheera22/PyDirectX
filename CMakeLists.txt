cmake_minimum_required(VERSION 3.5)

if(NOT WIN32 AND NOT APPLE)
	set(CMAKE_CXX_COMPILER "clang++")
endif()

SET( PROJ_PATH      ${CMAKE_CURRENT_SOURCE_DIR} )
SET( PROJ_OUT_PATH  ${CMAKE_CURRENT_BINARY_DIR} )


macro(ApplyIncludes)

	set (CMAKE_CXX_STANDARD 14)
	set(PYTHON_PATH "C:\\Users\\Alba\\AppData\\Local\\Programs\\Python\\Python35-32")
	if(WIN32)
	    message("${PYTHON_PATH}\\Include")
	    include_directories("${PYTHON_PATH}")
		link_directories("${PYTHON_PATH}\\libs\\")
		include_directories("${PYTHON_PATH}\\include")

		#include_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")
	else()
	    include_directories("${PYTHON_PATH}")
	    include_directories("${PYTHON_PATH}/Include")

	    if(APPLE)
		else()
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
		endif()
	endif()

endmacro()

## ---------------------------------

SET( PROJ_NAME "pycxx" )
SET( SRC_PATH "${PROJ_PATH}/lib/pycxx-7.0.3")
project (${PROJ_NAME})
include_directories("${SRC_PATH}")
FILE( GLOB pycxx "${SRC_PATH}/Src/*.cxx" "${SRC_PATH}/Src/*.c" )
ApplyIncludes()
add_library(${PROJ_NAME} STATIC ${pycxx})

## ---------------------------------

SET( PROJ_NAME "core" )
SET( SRC_PATH "")
project (${PROJ_NAME})
include_directories("${PROJ_PATH}")
FILE( GLOB core "${PROJ_PATH}/src/*.cpp")
add_library(${PROJ_NAME} STATIC ${core})


## ---------------------------------


SET( PROJ_NAME      "BridgeEngine" )
project (${PROJ_NAME})
include_directories("${PROJ_PATH}/src")
include_directories("${PROJ_PATH}/bridge")
include_directories("${PROJ_PATH}/lib/pycxx-7.0.3")

FILE( GLOB_RECURSE PROJ_SOURCES bridge/*.cxx)
ApplyIncludes()
add_library(${PROJ_NAME} MODULE ${PROJ_SOURCES})


if(APPLE)
	SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")

	target_link_libraries(${PROJ_NAME} "-framework AVFoundation")
	target_link_libraries(${PROJ_NAME} "-framework OpenCL")
	target_link_libraries(${PROJ_NAME} "-framework Accelerate")
	target_link_libraries(${PROJ_NAME} "-framework Cocoa")
	target_link_libraries(${PROJ_NAME} "-framework Foundation")
	target_link_libraries(${PROJ_NAME} "-framework CoreVideo")
	target_link_libraries(${PROJ_NAME} "-framework QuartzCore")
	target_link_libraries(${PROJ_NAME} "-framework CoreGraphics")
else()
	if(WIN32)
		target_link_libraries(${PROJ_NAME} "d3d11")
	else()# linux
	endif()
endif()

target_link_libraries( ${PROJ_NAME} "pycxx")
target_link_libraries( ${PROJ_NAME} "core")
set_target_properties( ${PROJ_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/exe"  )
set_target_properties( ${PROJ_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/exe" )
set_target_properties( ${PROJ_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/exe"  )
set_target_properties( ${PROJ_NAME} PROPERTIES OUTPUT_NAME "BridgeEngine")
if(WIN32)
	set_target_properties( ${PROJ_NAME} PROPERTIES SUFFIX ".pyd")
endif()
set_target_properties( ${PROJ_NAME} PROPERTIES PREFIX "")
