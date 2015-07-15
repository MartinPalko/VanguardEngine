
# Find all sub directories containting CMakeLists.txt files
FIND_TOPLEVEL_MAKELISTS(subMakeLists "${CMAKE_CURRENT_SOURCE_DIR}")
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