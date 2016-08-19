[![Build Status](https://travis-ci.org/MartinPalko/VanguardEngine.svg?branch=master)](https://travis-ci.org/MartinPalko/VanguardEngine) [![Build status](https://ci.appveyor.com/api/projects/status/6yi9e7ki2dr9kjsw?svg=true)](https://ci.appveyor.com/project/MartinPalko/vanguardengine) <a href="https://scan.coverity.com/projects/5497">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/5497/badge.svg"/>
</a>

# VanguardEngine
A small (for now) game engine, created for fun and learning.

Please Note: Vanguard is still in the early stages! Lots of stuff is still not done, but it's getting there!

## Compiling

### General

- CMake is used for project file generation, a few shell scripts and batch files are included, if you want to generate projects for something else, use them as an example.
- Binaries are output to the Bin folder in the root Vanguard directory.

### Windows

Requirements:
- Visual Studio (Tested on 2015)

Run "Windows_CreateVSProjectFiles.bat", found in the root directory. This will have CMake create a solution in the Intermediate/VisualStudio directory, as well as a shotrcut in the root. Just open "Vanguard.sln" and it should all compile. All dependencies are included in the "ThirdParty" submodule.

### Linux

Currently work in progress!

I've included two shell scripts for generating project files on Linux; "Linux_CreateMakefile.sh", and "Linux_CreateCodeBlocksProjectFiles.sh". They do just as they say, and output to the intermediate foler.

Currently only tested with gcc.

### OS X

Currently work in progress!
I don't have a mac, so I'm not actively able to test on OS X. If you want to help, drop me a line!
