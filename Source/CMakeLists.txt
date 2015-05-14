CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

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


# Decode a string saved as "Project=Dependency" to two seperate strings.
MACRO (DECODE_DEPENDENCY_PAIR return_project return_dependency pair)
	STRING(FIND ${pair} "=" seperatorPosition)
	MATH(EXPR seperatorPositionPlusOne "${seperatorPosition}+1")		
	STRING(SUBSTRING ${dependencyPair} 0 ${seperatorPosition} ${return_project})
	STRING(SUBSTRING ${dependencyPair} ${seperatorPositionPlusOne} -1 ${return_dependency})
ENDMACRO()

# Recursively find all "CMakeLists.txt" files. Only finds "top level" files, so recursion stops when it finds one.
FUNCTION (FIND_TOPLEVEL_MAKELISTS return_list)
	#FILE(GLOB_RECURSE subMakeLists *CMakeLists.txt)
	FILE(GLOB subDirectories "${CMAKE_CURRENT_SOURCE_DIR}/*")
	FOREACH (directory ${subDirectories})
		FIND_MAKELISTS_RECURSIVE(${return_list} ${directory})
	ENDFOREACH()
	
	SET(${return_list} ${subMakeLists} PARENT_SCOPE)
ENDFUNCTION()

# Recursively searches the specified folder for "CMakeLists.txt" files. Recursion stops when it finds one.
FUNCTION (FIND_MAKELISTS_RECURSIVE return_lists directoryToSearch)
	FILE(GLOB makelistInDir "${directoryToSearch}/CMakeLists.txt")
	IF("${makelistInDir}" STREQUAL "")		
		FILE(GLOB subDirectories "${directoryToSearch}/*")
		FOREACH (directory ${subDirectories})
			FIND_MAKELISTS_RECURSIVE(${return_list} ${directory})
		ENDFOREACH()
	ELSE()
		MESSAGE("${makelistInDir}")
		LIST(APPEND ${return_list} "${makelistInDir}")
	ENDIF()
	SET(${return_list} ${subMakeLists} PARENT_SCOPE)

ENDFUNCTION()

# Solves dependencies based on those found in the global variable "recordedProjectDependencies".
# Functions recursively, so if the dependency has dependencies, it solves those first.
FUNCTION (RECURSIVE_SOLVE_DEPENDENCIES in_project)
	FOREACH (dependencyPair ${recordedProjectDependencies})
		DECODE_DEPENDENCY_PAIR(temp_project temp_dependency "${dependencyPair}")
		#Determine if this is a dependency we need
		IF(${in_project} MATCHES ${temp_project})
			LIST(FIND solvedProjects ${temp_dependency} isInSolvedList)
			#If dependency hasn't had it's dependencies solved yet, solve it.
			IF(${isInSolvedList} EQUAL -1)
				RECURSIVE_SOLVE_DEPENDENCIES(${temp_dependency})				
			ENDIF()
			
			# Add dependency to platform
			ADD_DEPENDENCIES(${in_project} ${temp_dependency})
			# Append project include directories with that of it's dependency's
			GET_TARGET_PROPERTY(dependencyPublicIncludes ${temp_dependency} INCLUDE_DIRECTORIES)
			GET_TARGET_PROPERTY(projectPublicIncludes ${in_project} INCLUDE_DIRECTORIES)
			
			SET(appendedDependencies "${projectPublicIncludes}")
			LIST(APPEND appendedDependencies "${dependencyPublicIncludes}")
			TARGET_INCLUDE_DIRECTORIES(${in_project} PUBLIC ${appendedDependencies})
		ENDIF()		
	ENDFOREACH()
	
	# All our dependencies have been solved. Now we can enter the solved list.
	LIST(APPEND solvedProjects "${in_project}")
	# Update the variable in parent scope, since we want to use it globally.
	SET (solvedProjects "${solvedProjects}" PARENT_SCOPE)
	MESSAGE("Solved dependencies for ${in_project}")
ENDFUNCTION()

# Same as IMPLEMENT_PROJECT, but .h and .cpp are found automatically by scanning sub-directories.
MACRO (IMPLEMENT_PROJECT_AUTOFINDSOURCES projType projName projDependencies)
	FILE (GLOB_RECURSE sourceFiles "*.h" "*.cpp")
	IMPLEMENT_PROJECT("${projType}" "${projName}" "${projDependencies}" "${sourceFiles}")
ENDMACRO()

MACRO(PUT_IN_FOLDER_RELATIVE_ROOT in_projectName in_additionalSorting)
	# Put us in a IDE folder to match the disk folder relative to root cmake file.
	GET_FILENAME_COMPONENT(prent_path "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
	file (RELATIVE_PATH relativeCurrentSourceDir "${CMAKE_SOURCE_DIR}" "${prent_path}")
	SET_TARGET_PROPERTIES("${in_projectName}" PROPERTIES FOLDER "${relativeCurrentSourceDir}${in_additionalSorting}")
ENDMACRO()

# Macro to easily implement a project. Automatically includes all code files, and writes dependencies to "recordedProjectDependencies" for linking later.
MACRO (IMPLEMENT_PROJECT projType projName projDependencies sourceFiles)
	MESSAGE("Creating project \"${projName}\"")

	# Ensure source files are absolute
	SET(absoluteSourceFiles "")
	FOREACH(sourcePath ${sourceFiles})
		IF (NOT IS_ABSOLUTE ${sourcePath})
			GET_FILENAME_COMPONENT(sourcePath ${sourcePath} ABSOLUTE)
			LIST(APPEND absoluteSourceFiles ${sourcePath})
		ELSE()
			LIST(APPEND absoluteSourceFiles ${sourcePath})
		ENDIF()
	ENDFOREACH()
	
	IF (${projType} MATCHES "EXECUTABLE")
		ADD_EXECUTABLE(${projName} ${absoluteSourceFiles})
	ELSE()
		ADD_LIBRARY(${projName} ${projType} ${absoluteSourceFiles})
	ENDIF()
	
	PUT_IN_FOLDER_RELATIVE_ROOT(${projName} "")
	
	# Group source files in the same way that they are found in the folders
	FOREACH (file ${absoluteSourceFiles})
		file (RELATIVE_PATH relative_file "${CMAKE_CURRENT_SOURCE_DIR}" ${file}) # Make relative to CMakeLists
		GET_FILENAME_COMPONENT(dir_path ${relative_file} DIRECTORY) # Get folder path
		SOURCE_GROUP("${dir_path}" FILES ${file}) # Put in group		
	ENDFOREACH()
	
	# Need to include directories of source files so headers can be found
	GET_DIRECTORIES(includeDirs "${absoluteSourceFiles}")
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
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("MODULE" "${moduleName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_STATIC_LIB libName dependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("STATIC" "${libName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_DYNAMIC_LIB libName dependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("SHARED" "${libName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing executables
MACRO (IMPLEMENT_EXECUTABLE executableName dependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("EXECUTABLE" "${executableName}" "${dependencies}")	
ENDMACRO ()

MACRO (IMPLEMENT_THIRDPARTY_STATIC_LIB libName dependencies sourceFiles)
	IMPLEMENT_PROJECT("STATIC" "${libName}" "${dependencies}" "${sourceFiles}")
ENDMACRO()

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
FIND_TOPLEVEL_MAKELISTS(subMakeLists)
GET_DIRECTORIES(libDirectories "${subMakeLists}")

 # Make sure we're not adding our own (root) directory
LIST(REMOVE_ITEM libDirectories ${CMAKE_CURRENT_SOURCE_DIR})

# Run CmakeLists.txt scripts for each project.
FOREACH(file_path ${libDirectories})
		add_subdirectory (${file_path})
ENDFOREACH()

# Now link dependencies. "recordedProjectDependencies" is written to from the macros.
FOREACH (dependencyPair ${recordedProjectDependencies})
	DECODE_DEPENDENCY_PAIR(platform dependency "${dependencyPair}")
	
	LIST(FIND solvedProjects ${platform} alreadySolved)
	#MESSAGE(" ${alreadySolved}\nplatform  : ${platform}\ndependency  : ${dependency}\nsolved list  : ${solvedProjects}\n"  )
	#If dependency hasn't had it's dependencies solved yet, solve it.
	IF(${alreadySolved} EQUAL -1)
		
		# Recursively solve dependencies for each project that has at least one dependency
		RECURSIVE_SOLVE_DEPENDENCIES(${platform})	
	ENDIF()
ENDFOREACH()