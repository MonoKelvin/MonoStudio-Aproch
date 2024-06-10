@echo off

set output_dir=%~dp0
set configuration=%1
set QT_INSTALL_PATH=%2

for %%F in ("%output_dir%*.dll") do (
    set "target_file=%output_dir%\%%~nxF"
    call "%QT_INSTALL_PATH%\bin\windeployqt6.exe" "%target_file%"
)
for %%F in ("%output_dir%*.exe") do (
    set "target_file=%output_dir%\%%~nxF"
    call "%QT_INSTALL_PATH%\bin\windeployqt6.exe" "%target_file%"
)

set dll_suffix=.dll
if "%configuration%"=="Debug" (
    set dll_suffix=d.dll
)
set qtquick_dll=Qt6QuickControls2%dll_suffix%
xcopy /y/s/i/d "%QT_INSTALL_PATH%\bin\%qtquick_dll%" "%output_dir%"
call "%QT_INSTALL_PATH%\bin\windeployqt6.exe" --qmldir %QT_INSTALL_PATH%\qml "%output_dir%\%qtquick_dll%"
