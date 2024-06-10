@echo off

@REM example: $(SolutionDir) $(Platform) $(Configuration) $(OutDir) $(QMake_QT_INSTALL_PREFIX_)
set solution_dir=%1
set platform=%2
set configuration=%3
set output_dir=%4
set QT_INSTALL_PATH=%5
@REM set has_qml=%6

set sdk_dir=%solution_dir%\Build\%platform%-%configuration%\
set sdk_inc_dir=%sdk_dir%Include\
set sdk_bin_dir=%sdk_dir%\Bin\

@REM --------------------------------------------------
@REM copy output files
@REM --------------------------------------------------

@REM dll file
xcopy /s /y /d /i "%sdk_bin_dir%\*.dll" "%output_dir%"

@REM copy dependent files
xcopy /s /y /d /i "%~dp0\Publish.bat" "%output_dir%"
call "%output_dir%Publish.bat" %configuration% %QT_INSTALL_PATH%
