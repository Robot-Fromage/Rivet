@echo off
setlocal
pushd %~dp0

call Bulk/Tools/Scripts/Windows/SetupHooks.bat
if not exist "VisualStudio_Solution_MSVC_Config.cmake" copy "Bulk\Tools\DefaultConfig.cmake" "VisualStudio_Solution_MSVC_Config.cmake"
pause
