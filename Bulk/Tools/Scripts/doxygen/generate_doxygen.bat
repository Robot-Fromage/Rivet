@echo off

:: Ensure local scope
setlocal

:: Ensure current dir
pushd %~dp0

:: CD 3 steps up to root/Bulk
cd ../../../

:: Step in Source dir
cd Source

doxygen %~dp0doxyfile
PAUSE
