sourceRoot=$PWD
cmakeExecutable=$sourceRoot"/ThirdParty/CMake/Linux/bin/cmake"

projectfileDirectory=$sourceRoot"/Intermediate/Makefile/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "Unix Makefiles" $sourceRoot -DCMAKE_BUILD_TYPE=Debug
