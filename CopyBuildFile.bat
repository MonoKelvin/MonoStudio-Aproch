@echo off
set project_dir=%1
set build_dir=%2
set output_dir=%3
set src_dir=%project_dir%
set output_inc_dir=%build_dir%\Include\
set output_bin_dir=%build_dir%\Bin\
set output_lib_dir=%build_dir%\Lib\

if not exist %build_dir% (
    md %build_dir%
)
if not exist %output_inc_dir% (
    md %output_inc_dir%
)
if not exist %output_bin_dir% (
    md %output_bin_dir%
)
if not exist %output_lib_dir% (
    md %output_lib_dir%
)

rem include files
pushd %project_dir%
    for /r %%a in (*.h) do (
        echo %%~dpI
        xcopy /s /y /i /q /t "%%a" "%output_inc_dir%" /exclude:%src_dir%..\BuildExcludeFileList
    )
popd

rem dll file
xcopy /s /y /i "%output_dir%\*.dll" "%output_bin_dir%"

rem lib file
xcopy /s /y /i "%output_dir%\*.lib" "%output_lib_dir%"
