projectfileDirectory=$(dirname $0)"/Intermediate/XCode/"
cmakeExecutable=$(dirname $0)"/ThirdParty/CMake/OSX/CMake.app/contents/bin/cmake"
sourceRoot=$(dirname $0)

mkdir -p $projectfileDirectory
cd $projectfileDirectory

# $cmakeExecutable -G "Xcode" -DCMAKE_OSX_ARCHITECTURES=i386 $sourceRoot
$cmakeExecutable -G "Xcode" -DCMAKE_OSX_ARCHITECTURES=x86_64 $sourceRoot
