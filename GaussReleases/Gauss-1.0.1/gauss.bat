set currentDir = %~dp0
set targetDir = bin\Gauss
chdir /d %currentDir%bin\
set newCurrentDir = %~dp0
START %currentDir%Gauss.exe