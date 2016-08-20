cmakeExecutable=$(dirname $0)"/ThirdParty/CMake/Linux/bin/cmake"
sourceRoot=$(dirname $0)

projectfileDirectory=$(dirname $0)"/Intermediate/MakeFile/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "Unix Makefiles" $sourceRoot