@echo off

set output_dir=%~dp0
set configuration=%1
set QT_INSTALL_PATH=%2

set qt_ver=5
set windeployqt=
if exist "%QT_INSTALL_PATH%\bin\windeployqt6.exe" (
    set "windeployqt=%QT_INSTALL_PATH%\bin\windeployqt6.exe"
    set qt_ver=6
) else (
    set "windeployqt=%QT_INSTALL_PATH%\bin\windeployqt.exe"
)

for %%F in ("%output_dir%*.dll") do (
    set "target_file=%output_dir%\%%~nxF"
    call "%windeployqt%" "%target_file%"
)
for %%F in ("%output_dir%*.exe") do (
    set "target_file=%output_dir%\%%~nxF"
    call "%windeployqt%" "%target_file%"
)

set dll_suffix=.dll
if "%configuration%"=="Debug" (
    set dll_suffix=d.dll
)

set qtquick_dll=Qt%qt_ver%QuickControls2%dll_suffix%
xcopy /y/s/i/d "%QT_INSTALL_PATH%\bin\%qtquick_dll%" "%output_dir%"
call "%windeployqt%" --qmldir %QT_INSTALL_PATH%\qml "%output_dir%\%qtquick_dll%"
