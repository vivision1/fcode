@echo off
SET PKG_CONFIG_PATH=c:\msys64\mingw64\lib\pkgconfig

REM setlocal ENABLEDELAYEDEXPANSION
REM set word=table
REM set str="jump over the chair"
REM set str=%str:chair=!word!%

set p=;c:/msys64/mingw64/bin/;c:/msys64/usr/bin/
REM setlocal ENABLEDELAYEDEXPANSION
REM set PATH=%PATH:!p!=%
REM call set str=%%str:chair=%word%%%
REM call set PATH=%%PATH:  =%% 
REM call set PATH=%%PATH:%p%=%% 
REM call set PATH=%%PATH:; =;%% 
REM set Params=%*
REM for /f "PATH=1*" %%a in ("!Params!") do EndLocal & set %1=%%b
REM SET PATH=%PATH%
REM EndLocal & set %PATH=%*%
REM SET LD_LIBRARY_PATH=


REM replace with empty
call set PATH=%%PATH:%p%=%% 

REM replace two whitespaces with none
call set PATH=%%PATH:  =%% 

SET PATH=%PATH%%p%
c:/msys64/mingw64/bin/mingw32-make.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
