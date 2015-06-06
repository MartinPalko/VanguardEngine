cmakeExecutable=$(dirname $0)"/Intermediate/CMake/Linux/bin/cmake"
sourceRoot=$(dirname $0)"/Source"

projectfileDirectory=$(dirname $0)"/Intermediate/CodeBlocks/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "CodeBlocks - Unix Makefiles" $sourceRoot

projectfileDirectory=$(dirname $0)"/Intermediate/Eclipse/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "Eclipse CDT4 - Unix Makefiles" $sourceRoot

projectfileDirectory=$(dirname $0)"/Intermediate/CodeBlocksNinja/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "CodeBlocks - Ninja" $sourceRoot

projectfileDirectory=$(dirname $0)"/Intermediate/EclipseNinja/"
mkdir -p $projectfileDirectory
cd $projectfileDirectory
$cmakeExecutable -G "Eclipse CDT4 - Ninja" $sourceRoot