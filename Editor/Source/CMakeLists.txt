if (QtPrefix) # Make sure to only create editor project if Qt has been found.
	IMPLEMENT_EXECUTABLE(Editor "Core" "libsass")
	LINK_TO_QT(Editor)
endif()