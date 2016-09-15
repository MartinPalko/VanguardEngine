CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

# Makelist script starts here
PROJECT (Vanguard)

# Create variable for engine root folder
SET (EngineRoot "${CMAKE_CURRENT_SOURCE_DIR}")

INCLUDE ("${EngineRoot}/VanguardFunctions.cmake")

# Reset
SET (deferredAddProjects "" CACHE INTERNAL "deferredAddProjects")
SET (projectLaunchers "" CACHE INTERNAL projectLaunchers)

# Undefine install, we don't want install targets.
#MACRO (install targets...)
#ENDMACRO()

# Output all runtime files to the bin folder on build.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EngineRoot}/Bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${EngineRoot}/Bin")
# Put all configurations into the same folder
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${youroutputdirectory} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# Disables the "ZERO_CHECK" target, and trying to regenerate on build. (We want to manually regenerate)
SET(CMAKE_SUPPRESS_REGENERATION true)

IF(UNIX AND NOT APPLE)
	# Enable debug symbols for debug build.
	set(CMAKE_CXX_FLAGS_DEBUG  "-g")
	# Enable C++11
	# -fPIC : Position independent code (needed for shared libs on UNIX based OS).
	# -fexceptions -pthread : exeptions and threading.
	# -lrt : Links to the librt.so library, see: http://stackoverflow.com/questions/2418157/ubuntu-linux-c-error-undefined-reference-to-clock-gettime-and-clock-settim
	# -lX11 -lXt: Links to the X11 window system library
	set (CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fexceptions -pthread -lrt -lX11 -lXt")
ENDIF()

IF (APPLE)
	set (CMAKE_CXX_FLAGS "-x objective-c++ -std=c++11")
	set (CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
ENDIF()

IF (MSVC)
	# Test 32/64 bits
	if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
		message(STATUS "Target is 64 bits")
		if (WIN32)
			set(WINXXBITS Win64)
		endif(WIN32)
	else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
		message(STATUS "Target is 32 bits")
		if (WIN32)
			set(WINXXBITS Win32)
		endif(WIN32)
	endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
ENDIF()

# Enable folders in the solution
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS "ON")

# First do thirdparty
add_subdirectory("${EngineRoot}/ThirdParty")

# Then engine
add_subdirectory("${EngineRoot}/Engine")

# Then modules
add_subdirectory("${EngineRoot}/Modules")

# Then projects
add_subdirectory("${EngineRoot}/Projects")