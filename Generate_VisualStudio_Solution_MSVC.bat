@echo off

if not exist "%~dp0Bulk\Tools\Scripts\Windows\Generate_VisualStudio_Solution_MSVC.bat" goto Error_NotFound
call "%~dp0Bulk\Tools\Scripts\Windows\Generate_VisualStudio_Solution_MSVC.bat" %*
exit /B %ERRORLEVEL%

:Error_NotFound
echo Generate_VisualStudio_Solution_MSVC ERROR: The batch file does not appear to be located where it should.
pause
exit /B 1
