@echo off
set HOME=C:
set USER=Windows
ccg_client.exe --user %USER%  --lang de %1 %2 %3 %4 %5 %6 %7 %8 %9  metw.xml
ccg_client --help
pause