@echo off
setlocal
pushd %~dp0

:::::::::::::::
:: Arg processing
IF [%1]==[] (
    ECHO %0: ERROR
    ECHO You need to pass src path as first param
    goto :error
)

IF [%2]==[] (
    ECHO %0: ERROR
    ECHO You need to pass dst path as first param
    goto :error
)

SET SRC_DIR=%1
SET SRC_DIR=%SRC_DIR:"=%
SET DST_DIR=%2
SET DST_DIR=%DST_DIR:"=%
SET SRC_HASH=
SET DST_HASH=

for /f "tokens=*" %%i in ('dirhash "%SRC_DIR%" SHA256 -quiet -nowait -progress') do SET SRC_HASH=%%i
for /f "tokens=*" %%i in ('dirhash "%DST_DIR%" SHA256 -quiet -nowait -progress') do SET DST_HASH=%%i

ECHO "%SRC_DIR%"
ECHO "%SRC_HASH%"
ECHO "%DST_DIR%"
ECHO "%DST_HASH%"

PAUSE

IF "%SRC_HASH%"=="%DST_HASH%" goto:EOF
goto:Clean

:::::::::::::::
:: Cleaning
:Clean
:Repeat
RMDIR /s /q "%DST_DIR%\"
IF EXIST "%DST_DIR%\" goto :Repeat
ECHO dir at output location was cleaned
MKDIR "%DST_DIR%"
ECHO "%DST_DIR%"

:::::::::::::::
:: Copying
:Copy
XCOPY /e /h /y "%SRC_DIR%" "%DST_DIR%"

PAUSE

:::::::::::::::
:: Reaching End of the Script
GOTO :success

:::::::::::::::
:error
ECHO %0: ERROR
ECHO //////////
EXIT /B 1

:::::::::::::::
:success
ECHO %0: SUCCESS
ECHO //////////
EXIT /B 0