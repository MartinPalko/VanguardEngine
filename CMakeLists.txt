CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

# Makelist script starts here
PROJECT (Vanguard)

# Create variable for engine root folder
SET (EngineRoot "${CMAKE_CURRENT_SOURCE_DIR}")

INCLUDE ("${EngineRoot}/CMake/VanguardFunctions.cmake")


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
	# Enable C++11
	# -fPIC : Position independent code (needed for shared libs on UNIX based OS).
	# -fexceptions -pthread : exeptions and threading.
	set (CMAKE_CXX_FLAGS "-std=c++11 -fPIC -fexceptions -pthread")
ENDIF()

IF (APPLE)
	set (CMAKE_CXX_FLAGS "-x objective-c++ -std=c++11")
	set (CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
	set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
ENDIF()

# Enable folders in the solution
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS "ON")

# First do thirdparty
ADD_TOPLEVEL_MAKELISTS("${EngineRoot}/ThirdParty")

# Then engine
ADD_TOPLEVEL_MAKELISTS("${EngineRoot}/Engine/Source")

# Then modules
ADD_TOPLEVEL_MAKELISTS("${EngineRoot}/Modules")

# Then projects
FIND_VANGUARD_PROJECTS_IN_FOLDER("${EngineRoot}/Projects")


# Now Create and link all the deferred projects
MESSAGE("Creating deferred projects:")
	
FOREACH(project ${deferredAddProjects})
	ADD_DEFFERED_PROJECTS_RECURSIVE(${project})
ENDFOREACH()

#IF visual studio, find Csharp projects and add them to the solution.
IF (MSVC)
	#Find and add all csharp projects
	FILE(GLOB_RECURSE CSharpProjects "${CMAKE_CURRENT_SOURCE_DIR}/*.csproj")

	#Add them all to solution.
	FOREACH(CSharpProject ${CSharpProjects})
			GET_FILENAME_COMPONENT(ProjectName ${CSharpProject} NAME_WE)		
			INCLUDE_EXTERNAL_MSPROJECT("${ProjectName}" "${CSharpProject}" PLATFORM "Any CPU")
		
			GET_FILENAME_COMPONENT(projectFolder "${CSharpProject}" DIRECTORY)
		
			PUT_IN_FOLDER_RELATIVE_ROOT ("${ProjectName}" "${projectFolder}/..")
		
			MESSAGE("Added CSharp Project: ${ProjectName} from ${CSharpProject}")
	ENDFOREACH()
ENDIF()