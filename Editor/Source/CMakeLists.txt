if (QtPrefix) # Make sure to only create editor project if Qt has been found.
	IMPLEMENT_EXECUTABLE(Editor "Core" "")
	LINK_TO_QT(Editor)
endif()