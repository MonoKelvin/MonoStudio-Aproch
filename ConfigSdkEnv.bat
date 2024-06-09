@echo off

set SdkPath=%~dp0Build\
set "SdkPath=%SdkPath:"=%"

echo APROCH_SDK_PATH has been successfully set to the path: %SdkPath%
setx APROCH_SDK_PATH %SdkPath%
