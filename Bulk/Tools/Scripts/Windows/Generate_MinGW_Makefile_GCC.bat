@echo off

:: Ensure local scope
setlocal

:: Ensure current dir
pushd %~dp0

:: CD 3 steps up to root/Bulk
cd ../../../

:: Make Generated dir for generated project if not exist
IF NOT EXIST Generated_MinGW_Makefile_GCC ( MKDIR Generated_MinGW_Makefile_GCC )

:: Step in Generated dir
cd Generated_MinGW_Makefile_GCC

IF NOT EXIST Release ( MKDIR Release )
IF NOT EXIST Debug ( MKDIR Debug )

cd Release
:: Clean cmake garbage if there
IF EXIST CMakeFiles ( rmdir /S /Q CMakeFiles )
IF EXIST cmake_install.cmake ( del cmake_install.cmake )
IF EXIST CMakeCache.txt ( del CMakeCache.txt )
:: Rebuild Project
cmake -G "MinGW Makefiles" -DRIVET_USE_CONFIG:BOOL=ON -DCMAKE_BUILD_TYPE=Release -DRIVET_EXPLICIT_COMPILER_ID:STRING="GCC" -DRIVET_EXPLICIT_HOST_ID:STRING="WIN" ../../Source

cd ..
cd Debug
:: Clean cmake garbage if there
IF EXIST CMakeFiles ( rmdir /S /Q CMakeFiles )
IF EXIST cmake_install.cmake ( del cmake_install.cmake )
IF EXIST CMakeCache.txt ( del CMakeCache.txt )
:: Rebuild Project
cmake -G "MinGW Makefiles" -DRIVET_USE_CONFIG:BOOL=ON -DCMAKE_BUILD_TYPE=Debug -DRIVET_EXPLICIT_COMPILER_ID:STRING="GCC" -DRIVET_EXPLICIT_HOST_ID:STRING="WIN" ../../Source