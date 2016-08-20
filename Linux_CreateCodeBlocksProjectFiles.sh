cmakeExecutable=$(dirname $0)"/ThirdParty/CMake/Linux/bin/cmake"
sourceRoot=$(dirname $0)

projectfileDirectory=$(dirname $0)"/Intermediate/CodeBlocks/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "CodeBlocks - Unix Makefiles" $sourceRoot