@echo off
set project_dir=%1
set build_dir=%2
set output_dir=%3
set src_dir=%project_dir%
set output_inc_dir=%build_dir%Include\
set output_bin_dir=%build_dir%\Bin\
set output_lib_dir=%build_dir%\Lib\
set output_pdb_dir=%build_dir%\Pdb\

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
if not exist %output_pdb_dir% (
    md %output_pdb_dir%
)

rem include files
start %src_dir%..\CopyFilesToDirectory.exe %src_dir% %output_inc_dir% -s:.h;.inc -e:%src_dir%..\BuildExcludeFileList

rem dll file
xcopy /s /y /d /i "%output_dir%\*.dll" "%output_bin_dir%"

rem lib file
xcopy /s /y /d /i "%output_dir%\*.lib" "%output_lib_dir%"

rem pdb file
xcopy /s /y /d /i "%output_dir%\pdb\*.pdb" "%output_pdb_dir%"

rem test
if exist "%build_dir%\Bin\Aprochd.dll" (
xcopy /s /y /d "%output_bin_dir%\Aprochd.dll" "%build_dir%\Test\TestWinUI\"
)
if exist "%build_dir%\Bin\Aproch.dll" (
xcopy /s /y /d "%output_bin_dir%\Aproch.dll" "%build_dir%\Test\TestWinUI\"
)
