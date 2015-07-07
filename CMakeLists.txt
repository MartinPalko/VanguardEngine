CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

# Makelist script starts here
PROJECT (Vanguard)

# Create variable for engine root folder
SET (EngineRoot "${CMAKE_CURRENT_SOURCE_DIR}")

INCLUDE ("${EngineRoot}/CMake/VanguardFunctions.cmake")

ADD_SUBDIRECTORY ("${EngineRoot}/Engine/Source")