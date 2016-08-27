sourceRoot=$PWD
cmakeExecutable=$sourceRoot"/ThirdParty/CMake/Linux/bin/cmake"

projectfileDirectory=$sourceRoot"/Intermediate/CodeBlocks/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "CodeBlocks - Unix Makefiles" $sourceRoot
