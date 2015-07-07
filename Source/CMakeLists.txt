CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

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

# Find all sub directories containting CMakeLists.txt files
FIND_TOPLEVEL_MAKELISTS(subMakeLists "${EngineRoot}/Source")
GET_DIRECTORIES(libDirectories "${subMakeLists}")

 # Make sure we're not adding our own (root) directory
LIST(REMOVE_ITEM libDirectories ${CMAKE_CURRENT_SOURCE_DIR})

LINK_DIRECTORIES(${libDirectories})

# Run CmakeLists.txt scripts for each project.
FOREACH(file_path ${libDirectories})
		MESSAGE("Executing makelist at: ${file_path}")
		add_subdirectory (${file_path})
		MESSAGE("")
ENDFOREACH()

# Now create all vanguard projects found in the projects folder.
FIND_VANGUARD_PROJECTS_IN_FOLDER("${EngineRoot}/Projects")

# Create all the deferred projects
MESSAGE("Creating deferred projects:")
	
FOREACH(project ${deferredAddProjects})
	ADD_DEFFERED_PROJECTS_RECURSIVE(${project})
ENDFOREACH()

#Find Csharp projects
IF (MSVC)
	#Find and add all csharp projects
	FILE(GLOB_RECURSE CSharpProjects "${EngineRoot}/Source/Managed/*.csproj")

	#Add them all to solution.
	FOREACH(CSharpProject ${CSharpProjects})
			GET_FILENAME_COMPONENT(ProjectName ${CSharpProject} NAME_WE)		
			INCLUDE_EXTERNAL_MSPROJECT("${ProjectName}" "${CSharpProject}" PLATFORM "Any CPU")
		
			GET_FILENAME_COMPONENT(projectFolder "${CSharpProject}" DIRECTORY)
		
			PUT_IN_FOLDER_RELATIVE_ROOT ("${ProjectName}" "${projectFolder}/..")
		
			MESSAGE("Added CSharp Project: ${ProjectName}")
	ENDFOREACH()
ENDIF()