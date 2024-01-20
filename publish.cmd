rem @ECHO OFF
SET RELEASE_FILE=%1
SET DEST_PATH=%2
SET DEST_FILE=%~nx1
SET QT_BIN=%3

rmdir /S /Q %DEST_PATH%
mkdir %DEST_PATH%
copy /B %RELEASE_FILE% %DEST_PATH% /Y

IF "%QT_BIN%"=="" (
  SET QT_BIN=e:\Soft\Qt\6.5.0\msvc2019_64\bin
)

%QT_BIN%\windeployqt6.exe %DEST_PATH%\%DEST_FILE%

exit 0
