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
		LIST(APPEND ${return_list} "${makelistInDir}")
	ENDIF()
	SET(${return_list} ${subMakeLists} PARENT_SCOPE)

ENDFUNCTION()

# Same as IMPLEMENT_PROJECT, but .h and .cpp are found automatically by scanning sub-directories.
MACRO (IMPLEMENT_PROJECT_AUTOFINDSOURCES projType projName projDependencies)
	FILE (GLOB_RECURSE sourceFiles "*.h" "*.cpp")
	IMPLEMENT_PROJECT("${projType}" "${projName}" "${projDependencies}" "${sourceFiles}")
ENDMACRO()

MACRO(PUT_IN_FOLDER_RELATIVE_ROOT in_projectName in_additionalSorting)
	IF(TARGET "${in_projectName}") #Check to make sure target exists
		# Put us in a IDE folder to match the disk folder relative to root cmake file.
		GET_FILENAME_COMPONENT(prent_path "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
		file (RELATIVE_PATH relativeCurrentSourceDir "${CMAKE_SOURCE_DIR}" "${prent_path}")
		SET_TARGET_PROPERTIES("${in_projectName}" PROPERTIES FOLDER "${relativeCurrentSourceDir}${in_additionalSorting}")
	ELSE()
		MESSAGE("Warning: Cannot place project \"${in_projectName}\" in folder; project does not exist.")
	ENDIF()
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
	
	#Add it to list of projects to add at a later time.
	LIST(APPEND deferredAddProjects ${projName})
	SET(deferredAddProjects ${deferredAddProjects} PARENT_SCOPE)
	
	#Create variables for project type, sources, and dependencies for use later.
	SET("${projName}_Type" "${projType}" PARENT_SCOPE)
	SET("${projName}_Sources" "${absoluteSourceFiles}" PARENT_SCOPE)
	SET("${projName}_Dependencies" "${projDependencies}" PARENT_SCOPE)
	SET("${projName}_Path" "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)	
ENDMACRO ()

FUNCTION(ADD_DEFFERED_PROJECTS_RECURSIVE in_project)
	SET(projectName ${in_project})
	SET(projectType "${${in_project}_Type}")
	SET(projectSources "${${in_project}_Sources}")
	SET(projectDependencies "${${in_project}_Dependencies}")
	SET(projectPath "${${in_project}_Path}")	
	
	#For each dependency we have, add it first
	FOREACH(dependency ${projectDependencies})
		IF(NOT TARGET "${dependency}")
			ADD_DEFFERED_PROJECTS_RECURSIVE(${dependency})
		ENDIF()
	ENDFOREACH()

	#If target does not exist, it needs to be added, so do it.	
	IF(NOT TARGET "${projectName}" AND projectType)
		IF (${projectType} MATCHES "EXECUTABLE")
			ADD_EXECUTABLE(${projectName} ${projectSources})
			
			# If not specified as a console project, set subsystem to windows (will not spawn console window)
			IF(NOT "${${projectName}_IS_CONSOLE}" AND WIN32)
				set_target_properties(${projectName} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
			ENDIF()

			IF (Modules)
				#If executable, add all modules as dependencies, so they automatically build when debugging.
				ADD_DEPENDENCIES(${projectName} ${Modules})
			ENDIF()
			
		ELSE()
			ADD_LIBRARY(${projectName} ${projectType} ${projectSources})
		ENDIF()
		
		#On Unix, link libraries needed for dynamic links.
		IF(UNIX AND NOT ${projectType} MATCHES "STATIC")
			target_link_libraries(${projectName} PUBLIC ${CMAKE_DL_LIBS})
			#SUPER HACKY
			IF(NOT APPLE)
			target_link_libraries(${projectName} PUBLIC ${CMAKE_CXX_FLAGS})
			ENDIF()
		ENDIF()
		
		# Put project in folder inside of solution
		FILE (RELATIVE_PATH folder "${CMAKE_SOURCE_DIR}" "${projectPath}")
		GET_FILENAME_COMPONENT(folder "${folder}" DIRECTORY)
		SET_TARGET_PROPERTIES("${projectName}" PROPERTIES FOLDER "${folder}")
		
		# Group source files in the same way that they are found in the folders
		FOREACH (file ${projectSources})
			file (RELATIVE_PATH relative_file "${projectPath}" ${file}) # Make relative to CMakeLists
			GET_FILENAME_COMPONENT(dir_path ${relative_file} DIRECTORY) # Get folder path
			STRING (REPLACE "/" "\\" fixed "${dir_path}") # On windows the dir_path will have forward slashes and subgroups use backslashes, so convert them.
			SOURCE_GROUP("${fixed}" FILES ${file}) # Put in group
		ENDFOREACH()
		
		# Need to include directories of source files so headers can be found
		GET_DIRECTORIES(includeDirs "${projectSources}")
		TARGET_INCLUDE_DIRECTORIES(${projectName} PUBLIC ${includeDirs})
	ENDIF()
	
	MESSAGE("Linking dependencies for ${projectName}")
	
	#Now link us to our dependencies
	FOREACH(dependency ${projectDependencies})
		IF (${dependency}_INCLUDES)
			TARGET_INCLUDE_DIRECTORIES(${projectName} PUBLIC ${${dependency}_INCLUDES})
		ENDIF()
		
		if (TARGET ${dependency})
			MESSAGE("target ${dependency}")
		
			GET_TARGET_PROPERTY(publicIncludeDirectories ${dependency} INCLUDE_DIRECTORIES)		
			#Verify target has include directories first
			IF (publicIncludeDirectories)
				INCLUDE_DIRECTORIES(${publicIncludeDirectories})
			ENDIF()
			TARGET_LINK_LIBRARIES(${projectName} PUBLIC ${dependency})
			
		ELSEIF(${dependency})#Could be linking directly to lib file.
			MESSAGE("${dependency}")
			TARGET_LINK_LIBRARIES(${projectName} PUBLIC ${${dependency}})
		ELSE()
			MESSAGE("ERROR: Dependency ${dependency} not found")
		ENDIF()
		
	ENDFOREACH()
	
	MESSAGE("")
ENDFUNCTION()

# Macro for implementing modules
MACRO (IMPLEMENT_MODULE moduleName dependencies)
	SET(Modules ${Modules} ${moduleName} PARENT_SCOPE)
	#IMPLEMENT_PROJECT_AUTOFINDSOURCES("MODULE" "${moduleName}" "${dependencies}")
	#Shared libraries behave the same as actual modules, but also allow dependencies.
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("SHARED" "${moduleName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_STATIC_LIB libName dependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("STATIC" "${libName}" "${dependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_DYNAMIC_LIB libName dependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("SHARED" "${libName}" "${dependencies}")
ENDMACRO ()

MACRO (IMPLEMENT_CONSOLE_EXECUTABLE executableName dependencies)
	SET("${executableName}_IS_CONSOLE" TRUE PARENT_SCOPE)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("EXECUTABLE" "${executableName}" "${dependencies}")	
ENDMACRO ()
# Macro for implementing executables
MACRO (IMPLEMENT_EXECUTABLE executableName dependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("EXECUTABLE" "${executableName}" "${dependencies}")	
ENDMACRO ()