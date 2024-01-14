
@echo off
REM #push build to itch.io using butler / refinery

REM ### Please fill file path and ItchIO profile name ####
REM ### also set game name and operating system (windows = win) ####

set pathToBuild="C:\Users\timmo\Desktop\ExecutableGames\Dragon_Canvas"
set butlerName=pluz21/dragon-canvas:win
@echo on
@echo %pathToBuild%
@echo %butlerName%
@echo on
pause

REM ###run####


cd %pathToBuild%


REM #send file to itch.io via Butler:
butler push %pathToBuild%\ %butlerName%


pause