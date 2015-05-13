CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# ---------------- Macros ----------------

# Macro that returns a list of all the directories that contain the specified files.
MACRO(GET_DIRECTORIES return_list fileList)
    SET(dir_list "")
    FOREACH(file_path ${fileList})
        GET_FILENAME_COMPONENT(dir_path ${file_path} DIRECTORY)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# Macro to easily implement a project. Automatically includes all code files, and writes dependencies to "recordedProjectDependencies" for linking later.
MACRO (IMPLEMENT_PROJECT projType projName projDependencies)
	MESSAGE("Creating project \"${projName}\"")

	FILE (GLOB_RECURSE sourceFiles "*.h" "*.cpp")
	
	IF (${projType} MATCHES "EXECUTABLE")
		ADD_EXECUTABLE(${projName} ${sourceFiles})
	ELSE()
		ADD_LIBRARY(${projName} ${projType} ${sourceFiles})
	ENDIF()
	
	# Put us in a IDE folder to match the disk folder relative to root cmake file.
	GET_FILENAME_COMPONENT(prent_path "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
	file (RELATIVE_PATH relativeCurrentSourceDir "${CMAKE_SOURCE_DIR}" "${prent_path}")
	SET_TARGET_PROPERTIES(${projName} PROPERTIES FOLDER "${relativeCurrentSourceDir}")
	
	# Group source files in the same way that they are found in the folders
	FOREACH (file ${sourceFiles})
		file (RELATIVE_PATH relative_file "${CMAKE_CURRENT_SOURCE_DIR}" ${file}) # Make relative to CMakeLists
		GET_FILENAME_COMPONENT(dir_path ${relative_file} DIRECTORY) # Get folder path
		SOURCE_GROUP("${dir_path}" FILES ${file}) # Put in group		
	ENDFOREACH()
	
	# Need to include directories of source files so headers can be found
	GET_DIRECTORIES(includeDirs "${sourceFiles}")
	TARGET_INCLUDE_DIRECTORIES(${projName} PUBLIC ${includeDirs})
	
	# Put all cross project dependencies into a list to be linked later, after all projects have been added.
	FOREACH (dependency ${projDependencies})
		LIST(APPEND recordedProjectDependencies "${projName}=${dependency}")
		MESSAGE("    Dependency: ${dependency}")
	ENDFOREACH()	
	SET (recordedProjectDependencies "${recordedProjectDependencies}" PARENT_SCOPE) # Update the variable in parent scope
	
ENDMACRO ()

# Macro for implementing modules
MACRO (IMPLEMENT_MODULE moduleName dependencies)
	IMPLEMENT_PROJECT("MODULE" "${moduleName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_STATIC_LIB libName dependencies)
	IMPLEMENT_PROJECT("STATIC" "${libName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_DYNAMIC_LIB libName dependencies)
	IMPLEMENT_PROJECT("SHARED" "${libName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing executables
MACRO (IMPLEMENT_EXECUTABLE executableName dependencies)
	IMPLEMENT_PROJECT("EXECUTABLE" "${executableName}" "${dependencies}")	
ENDMACRO ()

# ---------------- End Macros ----------------

# Makelist script starts here
PROJECT (Vanguard)

# Enable folders in the solution
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS "ON")

# Create variable for engine root folder
GET_FILENAME_COMPONENT(EngineRoot "${CMAKE_SOURCE_DIR}" DIRECTORY) # Engine root is one up from source directory.
# Create variable for bin folder
SET(BinFolder "${EngineRoot}/Bin")

# Output all runtime files to the bin folder on build.
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BinFolder}")

# Find all sub directories containting CMakeLists.txt files
FILE(GLOB_RECURSE subMakeLists *CMakeLists.txt)
GET_DIRECTORIES(libDirectories "${subMakeLists}")

 # Make sure we're not adding our own (root) directory
LIST(REMOVE_ITEM libDirectories ${CMAKE_CURRENT_SOURCE_DIR})

# Run CmakeLists.txt scripts for each project.
FOREACH(file_path ${libDirectories})
		add_subdirectory (${file_path})
ENDFOREACH()

# Now link dependencies. "recordedProjectDependencies" is written to from the macros.
FOREACH (dependencyPair ${recordedProjectDependencies})
	# Decode the dependency pair into two variables.
	STRING(FIND ${dependencyPair} "=" seperatorPosition)
	MATH(EXPR seperatorPositionPlusOne "${seperatorPosition}+1")		
	STRING(SUBSTRING ${dependencyPair} 0 ${seperatorPosition} platform)
	STRING(SUBSTRING ${dependencyPair} ${seperatorPositionPlusOne} -1 dependency)
	
	# Add dependency to platform
	ADD_DEPENDENCIES(${platform} ${dependency})
	
	# Add include directories from dependencies so it can find headers
	GET_TARGET_PROPERTY(dependencyIncludes ${dependency} INTERFACE_INCLUDE_DIRECTORIES)
	TARGET_INCLUDE_DIRECTORIES(${platform} PRIVATE ${dependencyIncludes})
	
ENDFOREACH()