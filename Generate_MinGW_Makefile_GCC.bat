@echo off

if not exist "%~dp0Bulk\Tools\Scripts\Windows\Generate_MinGW_Makefile_GCC.bat" goto Error_NotFound
call "%~dp0Bulk\Tools\Scripts\Windows\Generate_MinGW_Makefile_GCC.bat" %*
exit /B %ERRORLEVEL%

:Error_NotFound
echo Generate_MinGW_Makefile_GCC ERROR: The batch file does not appear to be located where it should.
pause
exit /B 1
