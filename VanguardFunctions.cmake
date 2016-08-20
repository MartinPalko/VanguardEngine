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

MACRO(PUT_IN_FOLDER_RELATIVE_ROOT in_projectName in_projectLocation)
	IF(TARGET "${in_projectName}") #Check to make sure target exists
		# Put us in a IDE folder to match the disk folder relative to root cmake file.
		file (RELATIVE_PATH relativeCurrentSourceDir "${EngineRoot}" "${in_projectLocation}")
		
		#Remove "source" from path, since it's implied.
		STRING(REPLACE "Source" "" relativeCurrentSourceDir ${relativeCurrentSourceDir})
		
		SET_TARGET_PROPERTIES("${in_projectName}" PROPERTIES FOLDER "${relativeCurrentSourceDir}")
	ELSE()
		MESSAGE("Warning: Cannot place project \"${in_projectName}\" in folder; project does not exist.")
	ENDIF()
ENDMACRO()


# Recursively find all "CMakeLists.txt" files. Only finds "top level" files, so recursion stops when it finds one.
FUNCTION (FIND_TOPLEVEL_MAKELISTS return_list_var searchDirectory)
	SET(${return_list_var} "")	 
	SET(return_list "")

	# Find first level of subdirectories
	FILE(GLOB subDirectories "${searchDirectory}/*")
	FOREACH (directory ${subDirectories})
		FIND_MAKELISTS_RECURSIVE(return_list ${directory})
		
	ENDFOREACH()
	
	SET(${return_list_var} ${return_list} PARENT_SCOPE)	
ENDFUNCTION()

# Recursively find all "CMakeLists.txt" files. Only finds "top level" files, so recursion stops when it finds one.
MACRO (ADD_TOPLEVEL_MAKELISTS searchDirectory)

	# Find all sub directories containting CMakeLists.txt files
	FIND_TOPLEVEL_MAKELISTS(subMakeLists "${searchDirectory}")
	GET_DIRECTORIES(libDirectories "${subMakeLists}")

	 # Make sure we're not adding our own (root) directory
	LIST(REMOVE_ITEM libDirectories ${searchDirectory})

	LINK_DIRECTORIES(${libDirectories})

	# Run CmakeLists.txt scripts for each project.
	FOREACH(file_path ${libDirectories})
			MESSAGE("Executing makelist at: ${file_path}")
			add_subdirectory (${file_path})
			MESSAGE("")
	ENDFOREACH()

ENDMACRO()

# Recursively searches the specified folder for "CMakeLists.txt" files. Recursion stops when it finds one.
FUNCTION (FIND_MAKELISTS_RECURSIVE return_list_var directoryToSearch)

	SET(parent_returnlist "${${return_list_var}}")
	SET(return_list "")
	
	FILE(GLOB makelistInDir "${directoryToSearch}/CMakeLists.txt")
	
	IF("${makelistInDir}" STREQUAL "")
		FILE(GLOB subDirectories "${directoryToSearch}/*")
		FOREACH (directory ${subDirectories})
			FIND_MAKELISTS_RECURSIVE(return_list ${directory})
		ENDFOREACH()
	ELSE()
		SET(return_list ${return_list} "${makelistInDir}")
	ENDIF()
	
	# Append newly found variables
	LIST(APPEND parent_returnlist "${return_list}")
	
	SET(${return_list_var} ${parent_returnlist} PARENT_SCOPE)

ENDFUNCTION()

# Same as IMPLEMENT_PROJECT, but .h and .cpp are found automatically by scanning sub-directories.
MACRO (IMPLEMENT_PROJECT_AUTOFINDSOURCES projType projName projDependencies)
	FILE (GLOB_RECURSE sourceFiles "*.h" "*.cpp")
	IMPLEMENT_PROJECT("${projType}" "${projName}" "${projDependencies}" "${sourceFiles}")
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
	#SET(deferredAddProjects ${deferredAddProjects} PARENT_SCOPE)
	SET(deferredAddProjects ${deferredAddProjects} CACHE INTERNAL "deferredAddProjects")
	
	#Create variables for project type, sources, and dependencies for use later.
	SET("${projName}_Type" "${projType}")
	SET("${projName}_Sources" "${absoluteSourceFiles}")
	SET("${projName}_Dependencies" "${projDependencies}")
	SET("${projName}_Path" "${CMAKE_CURRENT_SOURCE_DIR}")
	
	SET("${projName}_Type" "${projType}" CACHE INTERNAL "${projName}_Type")
	SET("${projName}_Sources" "${absoluteSourceFiles}" CACHE INTERNAL "${projName}_Sources")
	SET("${projName}_Dependencies" "${projDependencies}" CACHE INTERNAL "${projName}_Dependencies")
	SET("${projName}_Path" "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "${projName}_Path")
ENDMACRO ()

MACRO (ADD_FLAGS project flags)
		GET_TARGET_PROPERTY(oldFlags ${project} COMPILE_FLAGS)
		if(oldFlags MATCHES "oldFlags-NOTFOUND")
			SET(oldFlags "") # set to empty string
		else()
			SET(oldFlags "${oldFlags} ") # a space to cleanly separate from existing content
		endif()		
		SET_TARGET_PROPERTIES(${project} PROPERTIES COMPILE_FLAGS "${oldFlags}${flags}")
ENDMACRO()

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
			MESSAGE("Adding executable target ${projectName}")
			ADD_EXECUTABLE(${projectName} ${projectSources})
			
			# If not specified as a console project, set subsystem to windows (will not spawn console window)
			IF(NOT "${${projectName}_IS_CONSOLE}" AND WIN32)
				set_target_properties(${projectName} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
			ENDIF()
			
			# See if we're a launcher for a project, if so, then insert a macro so we know which project we're launching.
			FOREACH(launcher ${projectLaunchers})
				IF(${projectName} MATCHES "${launcher}_Launcher")
					ADD_FLAGS(${projectName} "-DVANGUARD_PROJECT=${launcher}")
				ENDIF()
			ENDFOREACH()

			IF (Modules)
				#If executable, add all modules as dependencies, so they automatically build when debugging.
				ADD_DEPENDENCIES(${projectName} ${Modules})
			ENDIF()
			
		ELSE()
			ADD_LIBRARY(${projectName} ${projectType} ${projectSources})
		ENDIF()

		MESSAGE("${projectDependencies}")
		
		ADD_FLAGS(${projectName} "-DVANGUARD_LIB_NAME=\"${projectName}\"")
		ADD_FLAGS(${projectName} "-DVANGUARD_LIB_DEPENDENCIES=\"${projectDependencies}\"")
		
		#On Unix, link libraries needed for dynamic links.
		IF(UNIX AND NOT ${projectType} MATCHES "STATIC")
			target_link_libraries(${projectName} PUBLIC ${CMAKE_DL_LIBS})
			#SUPER HACKY
			IF(NOT APPLE)
			target_link_libraries(${projectName} PUBLIC ${CMAKE_CXX_FLAGS})
			ENDIF()
		ENDIF()
		
		PUT_IN_FOLDER_RELATIVE_ROOT ("${projectName}" "${projectPath}/..")
		
		# Group source files in the same way that they are found in the folders
		FOREACH (file ${projectSources})
			file (RELATIVE_PATH relative_file "${projectPath}" ${file}) # Make relative to CMakeLists
			GET_FILENAME_COMPONENT(dir_path ${relative_file} DIRECTORY) # Get folder path
			STRING (REPLACE "/" "\\" fixed "${dir_path}") # On windows the dir_path will have forward slashes and subgroups use backslashes, so convert them.
			SOURCE_GROUP("${fixed}" FILES ${file}) # Put in group
		ENDFOREACH()
		
		# Project path is a public include directory.
		TARGET_INCLUDE_DIRECTORIES(${projectName} PUBLIC ${projectPath})		
		
	ENDIF()
	
	MESSAGE("Linking dependencies for ${projectName} ")
	
	#Now link us to our dependencies
	FOREACH(dependency ${projectDependencies})
		IF (${dependency}_INCLUDES)
			TARGET_INCLUDE_DIRECTORIES(${projectName} PRIVATE ${${dependency}_INCLUDES})
		ENDIF()
		
		if (TARGET ${dependency})
			# Atm, just assume all non-deffered projects are thirdparty. This might need to be changed.
			SET(IsThirdparty "true")
			
			FOREACH(defferedProj deferredAddProjects)
				IF(${defferedProj} MATCHES ${dependency})
					SET(IsThirdparty "false")
				ENDIF()
			ENDFOREACH()
			
			if (${IsThirdparty})
				GET_TARGET_PROPERTY(publicIncludeDirectories ${dependency} INCLUDE_DIRECTORIES)	
				IF (publicIncludeDirectories)
					TARGET_INCLUDE_DIRECTORIES(${projectName} PRIVATE ${publicIncludeDirectories})
				ELSEIF(NOT ${dependency}_INCLUDES)
					MESSAGE("Warning: ${dependency} has no includes")
				ENDIF()		
				
			ELSE()
				GET_TARGET_PROPERTY(interfaceIncludeDirectories ${dependency} INTERFACE_INCLUDE_DIRECTORIES)	
				
				IF (interfaceIncludeDirectories)
					TARGET_INCLUDE_DIRECTORIES(${projectName} PUBLIC ${interfaceIncludeDirectories})
				ELSEIF(NOT ${dependency}_INCLUDES)
					MESSAGE("Warning: ${dependency} has no interface includes")
				ENDIF()		
			endif()
				
			
			#Thirdparty libs link privately.
			if (NOT ${IsThirdparty})
				TARGET_LINK_LIBRARIES(${projectName} PUBLIC ${dependency})
				MESSAGE("  public dependency ${dependency}")
			ELSE()
				TARGET_LINK_LIBRARIES(${projectName} PRIVATE ${dependency})
				MESSAGE("  private dependency ${dependency}")
			endif()
			
			TARGET_LINK_LIBRARIES(${projectName} PUBLIC ${dependency})
			
		ELSEIF(${dependency})#Could be linking directly to lib file.
			MESSAGE("lib ${dependency}")
			TARGET_LINK_LIBRARIES(${projectName} PRIVATE ${${dependency}})
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

FUNCTION (FIND_VANGUARD_PROJECTS_IN_FOLDER searchInFolder)
	FILE(GLOB_RECURSE projectConfigFiles "${searchInFolder}/*project.cfg")
	
	FOREACH(projectConfigFile ${projectConfigFiles})
		GET_FILENAME_COMPONENT(projectFolder "${projectConfigFile}" DIRECTORY)
		GET_FILENAME_COMPONENT(projectName "${projectFolder}" NAME)
		
		CREATE_VANGUARD_PROJECT("${projectFolder}" "${projectName}")	
	
	ENDFOREACH()
ENDFUNCTION()

MACRO (CREATE_VANGUARD_PROJECT projectFolder projectName)

	MESSAGE("Creating vanguard project: ${projectName}")
	# Add this project to the list of projects.
	SET(Vanguard_Projects ${Vanguard_Projects} projectName)
	
	FILE(GLOB_RECURSE CSharpProjects "${projectFolder}/*.csproj")
	
	FIND_TOPLEVEL_MAKELISTS(makeLists "${projectFolder}")
	
	GET_DIRECTORIES(makeListDirectories "${makeLists}")
	
	FOREACH(makeListDir ${makeListDirectories})
		MESSAGE("Executing makelist at: ${makeListDir}")
		add_subdirectory (${makeListDir})
		MESSAGE("")
	ENDFOREACH()
	
	VANGUARD_CREATE_PROJECT_LAUNCHER("${projectFolder}" "${projectName}")
	
ENDMACRO()

MACRO (VANGUARD_CREATE_PROJECT_LAUNCHER projectFolder projectName)
	SET(launcherFolder "${EngineRoot}/Engine/Source/ProjectLauncher")
	FILE (GLOB_RECURSE sourceFiles "${launcherFolder}/*.h" "${launcherFolder}/*.cpp")
	MESSAGE("Found source files: ${sourceFiles}")
	IMPLEMENT_PROJECT("EXECUTABLE" "${projectName}_Launcher" "Foundation" "${sourceFiles}")
	SET ("${projectName}_Launcher_Path" "${projectFolder}/${projectName}" CACHE INTERNAL "${projectName}_Launcher_Path")
	SET (projectLaunchers "${projectLaunchers}" ${projectName} CACHE INTERNAL projectLaunchers)
ENDMACRO()
