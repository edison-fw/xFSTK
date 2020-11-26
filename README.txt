Install prerequisites:
$ sudo apt-get install g++ qtbase5-dev qtcreator build-essential devscripts libxml2-dev alien doxygen graphviz libusb-dev libboost-all-dev 

Install CMake:
*note: Some repositories come with older versions of cmake. If that is the case, please build/install version cmake 2.8.11 or later
http://www.cmake.org/cmake/resources/software.html
Recommended version:
http://www.cmake.org/files/v2.8/cmake-2.8.12.2.tar.gz
View readme.txt in cmake tar.gz file for cmake build and install 

How To Build This Software:
$ mdkir build (in the extracted xfstk downloader directory)
$ cd build
$ export DISTRIBUTION_NAME=ubuntu12.04
$ export BUILD_VERSION=0.0.0
$ cmake .. 
(make sure that this was the cmake directory that was manually built and installed, i.e. /usr/local/bin/cmake ..)
$ make

Optional targets are available. I.E:
$ make docs
or
$ make package
Type `make help` (after cmake has been run) for a list of targets.

Explanation:
1) Make a directory. I.E:
	$ mkdir temporary_directory
 Q: Why do I have to make a directory?
 A: This is to initiate an "out-of-source" build so no build clutter will be left in the source tree.
    If you want to clean up the build you can just delete temporary_directory
    (more information: http://www.cmake.org/Wiki/CMake_FAQ#Out-of-source_build_trees).

2) Change to the directory you just created. I.E:
	$ cd temporary_directory
3) Execute cmake. It's argument will be the path to the CMakeLists.txt file.
	$ cmake ..
4) Simply run the make command with optional arguments.

