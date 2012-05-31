@echo off

set BOOST_ROOT=d:\usr\boost_1_49_0

set BUILD_TYPE=Release

set BUILD_FOLDER=_build_%BUILD_TYPE%_64

if not exist %BUILD_FOLDER% (
	mkdir %BUILD_FOLDER%
)

cd %BUILD_FOLDER%
cmake -DVERBOSE=OFF -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DSOLUTION_NAME=system_utilities -DVERBOSE=ON -G "Visual Studio 10 Win64" ../ 
cd ../

echo "%BUILD_FOLDER%/system_utilities.sln" > _start_msvc_2010_release.bat
