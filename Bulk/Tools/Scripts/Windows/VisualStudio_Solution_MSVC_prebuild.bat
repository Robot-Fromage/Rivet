@echo off
setlocal
pushd %~dp0

CALL smart_cp_resources "..\..\..\Resources" "..\..\..\Generated_VisualStudio_Solution_MSVC\Resources"

popd
