@echo off

@REM example: $(SolutionDir) $(Platform) $(Configuration) $(IntDir)
set solution_dir=%1
set platform=%2
set configuration=%3
set output_dir=%4

set src_dir=%solution_dir%Src\
set build_dir=%solution_dir%Build\%platform%-%configuration%\

set output_inc_dir=%build_dir%Include\
set output_bin_dir=%build_dir%Bin\
set output_lib_dir=%build_dir%Lib\
set output_pdb_dir=%build_dir%Pdb\

set third_party_dir=%solution_dir%ThirdParty\
set output_inc_third_party_dir=%output_inc_dir%ThirdParty\

@REM create directory
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
if not exist %output_inc_third_party_dir% (
    md %output_inc_third_party_dir%
)


@REM --------------------------------------------------
@REM copy output files
@REM --------------------------------------------------

@REM include files
start %solution_dir%CopyFilesToDirectory.exe %src_dir% %output_inc_dir% -s:.h;.inc -e:%solution_dir%BuildFile.exc

@REM dll file
xcopy /s /y /d /i "%output_dir%\*.dll" "%output_bin_dir%"
echo %output_dir%

@REM lib file
xcopy /s /y /d /i "%output_dir%\*.lib" "%output_lib_dir%"

@REM pdb file
xcopy /s /y /d /i "%output_dir%\pdb\*.pdb" "%output_pdb_dir%"


@REM --------------------------------------------------
@REM copy third party library
@REM --------------------------------------------------

@REM qwindowkit
xcopy /s /y /d /i "%third_party_dir%\qwindowkit\include" "%output_inc_third_party_dir%"
xcopy /s /y /d /i "%third_party_dir%\qwindowkit\%platform%-%configuration%\lib\*.lib" "%output_lib_dir%"
xcopy /s /y /d /i "%third_party_dir%\qwindowkit\%platform%-%configuration%\bin\*.dll" "%output_bin_dir%"
