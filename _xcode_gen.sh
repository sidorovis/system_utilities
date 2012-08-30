
export SOLUTION_NAME=pseaker

#set BOOST_ROOT=d:\usr\boost_1_47_0
#set RE_UTILS_ROOT=d:\usr\re-cpp-utils
#set SYSTEM_UTILITIES_ROOT=d:\usr\system_utilities
#set XERCESC_ROOT=d:\usr\xercesc\xerces-c_2_8_0-x86_64-windows-vc_8_0

#set PATH=%PATH%;%QT_ROOT%\bin

export BUILD_TYPE=Debug
if [ "$1" == 'Release' ];
then
	export BUILD_TYPE=Release
fi

export BUILD_FOLDER=_build_$BUILD_TYPE

if [ ! -d $BUILD_FOLDER ];
then
	mkdir $BUILD_FOLDER
fi

cd $BUILD_FOLDER
cmake -DVERBOSE=ON -DSOLUTION_NAME=$SOLUTION_NAME -DCMAKE_BUILD_TYPE=$BUILD_TYPE -G "Xcode" ../ 
cd ../

#echo "%BUILD_FOLDER%/%SOLUTION_NAME%.sln" > _start_msvc.bat
