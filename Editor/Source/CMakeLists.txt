if (QtPrefix) # Make sure to only create editor project if Qt has been found.
	IMPLEMENT_EXECUTABLE(Editor "Core" "libsass")
	
	#Set application icon on windows
	if (MSVC)
		target_sources(Editor PUBLIC "Editor.rc")
	endif()
	
	LINK_TO_QT(Editor)
endif()