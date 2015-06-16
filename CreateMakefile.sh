cmakeExecutable=$(dirname $0)"/Intermediate/CMake/Linux/bin/cmake"
sourceRoot=$(dirname $0)"/Source"

projectfileDirectory=$(dirname $0)"/Intermediate/MakeFile/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "Unix Makefiles" $sourceRoot