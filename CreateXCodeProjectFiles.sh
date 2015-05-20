projectfileDirectory=$(dirname $0)"/Intermediate/ProjectFiles/"
cmakeExecutable=$(dirname $0)"/Intermediate/CMake/OSX/CMake.app/contents/bin/cmake"
sourceRoot=$(dirname $0)"/Source"

mkdir -p $projectfileDirectory
cd $projectfileDirectory

$cmakeExecutable -G "Xcode" $sourceRoot
