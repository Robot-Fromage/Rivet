@ECHO OFF

:: Init MSVC Env Paths
SET MSVC_INSTALL_DIR=
for /f "usebackq delims=" %%i in (`vswhere -latest -version "[15.0,16.0)" -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set MSVC_INSTALL_DIR=%%i
)

:: Checking MSVC Path
SET "MSVC_VCVARS_DIR=%MSVC_INSTALL_DIR%\VC\Auxiliary\Build"
IF "%MSVC_INSTALL_DIR%"=="" 		( GOTO :error )
IF NOT EXIST "%MSVC_INSTALL_DIR%\" 	( GOTO :error )
IF NOT EXIST "%MSVC_VCVARS_DIR%\" 	( GOTO :error )

:::::::::::::::
:: Init MSVC Dev Env Variables
where /q MSBUILD
IF ERRORLEVEL 1 (
	PUSHD "%CD%"
	CALL "%MSVC_VCVARS_DIR%\vcvars64.bat" || goto :error
	POPD
)

call::checkExe CMAKE
call::checkExe MSBUILD
call::checkExe CLANG
call::checkExe G++
call::checkExe GIT
call::checkExe PYTHON3
PAUSE
goto:EOF

:::::::::::::::
:: Function: checkExe
:checkExe
where /q %1
IF ERRORLEVEL 1 (
    ECHO %1 Missing. Ensure it is installed and placed in your PATH.
) ELSE (
    ECHO %1 Ok.
)
goto:EOF
:: Return