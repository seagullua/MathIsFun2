@echo off
set PHP_DIR=%~1
set TEXTURE_PACKER_DIR=%~2
set BUILD_PROJECT=%~3
set PLATFORM=%~4

set ANDROID_PROJ=%~dp0


echo PHP_DIR: %PHP_DIR%
echo TEXTURE_PACKER_DIR: %TEXTURE_PACKER_DIR%
echo.
echo BUILD_PROJECT: %BUILD_PROJECT%
echo BUILD_PLATFORM: %PLATFORM%
echo.
echo PROJECT_DIR: %ANDROID_PROJ%
echo.
echo.

echo ===============================
echo Building resources
echo ===============================

set CHMOD="%CYGWIN_DIR%\chmod.exe"


cd %ANDROID_PROJ%
cd ResourceCompiler

call "%PHP_DIR%" ResourceCompiler.php "texture_packer=%TEXTURE_PACKER_DIR%;platform=%PLATFORM%;project=%BUILD_PROJECT%;source=../Resources_storage;destination=../Resources;language=en,ru;translate=uk;translation_dir=../Resources_storage/translations"

echo Texture build finished