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
MACRO (IMPLEMENT_PROJECT_AUTOFINDSOURCES projType projName publicDependencies privateDependencies)
	FILE (GLOB_RECURSE sourceFiles "*.h" "*.cpp")
	IMPLEMENT_PROJECT("${projType}" "${projName}" "${publicDependencies}" "${privateDependencies}" "${sourceFiles}")
	PUT_IN_FOLDER_RELATIVE_ROOT ("${projName}" "${CMAKE_CURRENT_SOURCE_DIR}/..")
ENDMACRO()

MACRO (LINK_DEPENDENCY projName dependency IsPrivate)
	if (TARGET ${dependency})
		GET_TARGET_PROPERTY(publicIncludeDirectories ${dependency} INCLUDE_DIRECTORIES)	
		IF (publicIncludeDirectories)
			TARGET_INCLUDE_DIRECTORIES(${projName} PRIVATE ${publicIncludeDirectories})
		ELSEIF(NOT publicIncludeDirectories)
			MESSAGE("Warning: ${dependency} has no includes")
		ENDIF()
	
		# Atm, just assume all non-deferred projects are thirdparty. This might need to be changed.
		SET(IsThirdparty "true")
		
		FOREACH(defferedProj deferredAddProjects)
			IF(${defferedProj} MATCHES ${dependency})
				SET(IsThirdparty "false")
			ENDIF()
		ENDFOREACH()
		
		#Thirdparty libs link privately.
		if (NOT ${IsPrivate})
			GET_TARGET_PROPERTY(interfaceIncludeDirectories ${dependency} INTERFACE_INCLUDE_DIRECTORIES)
			
			IF (interfaceIncludeDirectories)
				TARGET_INCLUDE_DIRECTORIES(${projName} PUBLIC ${interfaceIncludeDirectories})
			ELSEIF(NOT publicIncludeDirectories)
				MESSAGE("Warning: ${dependency} has no interface includes")
			ENDIF()	
		
			TARGET_LINK_LIBRARIES(${projName} PUBLIC ${dependency})
			MESSAGE("  public dependency ${dependency}")
		ELSE()
			TARGET_LINK_LIBRARIES(${projName} PRIVATE ${dependency})
			MESSAGE("  private dependency ${dependency}")
		endif()
		
		TARGET_LINK_LIBRARIES(${projName} PUBLIC ${dependency})
		
	ELSEIF(${dependency}) #Could be linking directly to lib file.
		MESSAGE("lib ${dependency}")
		TARGET_LINK_LIBRARIES(${projName} PRIVATE ${${dependency}})
	ELSE()
		MESSAGE("ERROR: Dependency ${dependency} not found")
	ENDIF()
ENDMACRO()

# Macro to easily implement a project. Automatically includes all code files, and writes dependencies to "recordedProjectDependencies" for linking later.
MACRO (IMPLEMENT_PROJECT projType projName publicDependencies privateDependencies sourceFiles)
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
		ADD_EXECUTABLE(${projName} ${sourceFiles})
		
		# If not specified as a console project, set subsystem to windows (will not spawn console window)
		IF(NOT "${${projName}_IS_CONSOLE}" AND WIN32)
			set_target_properties(${projName} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
		ENDIF()
		
	ELSE()
		ADD_LIBRARY(${projName} ${projType} ${sourceFiles})
	ENDIF()
	
	SET(projectPath "${CMAKE_CURRENT_SOURCE_DIR}")
	
	# Project path is a public include directory, so other modules can access it's headers
	TARGET_INCLUDE_DIRECTORIES(${projName} PUBLIC ${projectPath})
	
	ADD_FLAGS(${projName} "-DVANGUARD_LIB_NAME=\"${projName}\"")
	ADD_FLAGS(${projName} "-DVANGUARD_LIB_DEPENDENCIES=\"${publicDependencies};${privateDependencies}\"")

	#On Unix, link libraries needed for dynamic links.
	IF(UNIX AND NOT ${projType} MATCHES "STATIC")
		target_link_libraries(${projName} PUBLIC ${CMAKE_DL_LIBS})
		#SUPER HACKY
		IF(NOT APPLE)
		target_link_libraries(${projName} PUBLIC ${CMAKE_CXX_FLAGS})
		ENDIF()
	ENDIF()
	
	# Group source files in the same way that they are found in the folders
	FOREACH (file ${sourceFiles})
		file (RELATIVE_PATH relative_file "${projectPath}" ${file}) # Make relative to CMakeLists
		GET_FILENAME_COMPONENT(dir_path ${relative_file} DIRECTORY) # Get folder path
		STRING (REPLACE "/" "\\" fixed "${dir_path}") # On windows the dir_path will have forward slashes and subgroups use backslashes, so convert them.
		SOURCE_GROUP("${fixed}" FILES ${file}) # Put in group
	ENDFOREACH()
	
	MESSAGE("Linking dependencies for ${projName}:")
	
	#Now link us to our dependencies
	FOREACH(dependency ${publicDependencies})
		LINK_DEPENDENCY(${projName} ${dependency} "false")
	ENDFOREACH()	
	FOREACH(dependency ${privateDependencies})
		LINK_DEPENDENCY(${projName} ${dependency} "true")
	ENDFOREACH()	
	
	MESSAGE("")
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

	
	MESSAGE("")
ENDFUNCTION()

# Macro for implementing modules
MACRO (IMPLEMENT_MODULE moduleName publicDependencies privateDependencies)
	SET(Modules ${Modules} ${moduleName} PARENT_SCOPE)
	#IMPLEMENT_PROJECT_AUTOFINDSOURCES("MODULE" "${moduleName}" "${dependencies}")
	#Shared libraries behave the same as actual modules, but also allow dependencies.
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("SHARED" "${moduleName}" "${publicDependencies}"  "${privateDependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_STATIC_LIB libName publicDependencies privateDependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("STATIC" "${libName}" "${publicDependencies}")
ENDMACRO ()

# Macro for implementing static libraries
MACRO (IMPLEMENT_DYNAMIC_LIB libName publicDependencies privateDependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("SHARED" "${libName}" "${publicDependencies}" "${privateDependencies}" "${privateDependencies}")
ENDMACRO ()

MACRO (IMPLEMENT_CONSOLE_EXECUTABLE executableName publicDependencies privateDependencies)
	SET("${executableName}_IS_CONSOLE" TRUE PARENT_SCOPE)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("EXECUTABLE" "${executableName}" "${publicDependencies}" "${privateDependencies}")
ENDMACRO ()
# Macro for implementing executables
MACRO (IMPLEMENT_EXECUTABLE executableName publicDependencies privateDependencies)
	IMPLEMENT_PROJECT_AUTOFINDSOURCES("EXECUTABLE" "${executableName}" "${publicDependencies}" "${privateDependencies}")
ENDMACRO ()

FUNCTION (FIND_VANGUARD_PROJECTS_IN_FOLDER searchInFolder)
	FILE(GLOB_RECURSE projectConfigFiles "${searchInFolder}/*Project.cfg")
	
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
	MESSAGE("Creating Launcher:  ${projectName}")
	SET(launcherName  "${projectName}_Launcher")
	
	IMPLEMENT_PROJECT("EXECUTABLE" "${launcherName}" "Foundation" "" "${sourceFiles}")
	
	# Insert a macro so we know which project we're launching at runtime
	ADD_FLAGS(${launcherName} "-DVANGUARD_PROJECT=${projectName}")	
	PUT_IN_FOLDER_RELATIVE_ROOT ("${launcherName}" "${CMAKE_CURRENT_SOURCE_DIR}/${projectName}")
ENDMACRO()
