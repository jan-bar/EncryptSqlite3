@echo off

gcc -s -o sqlite3.exe sqlite3.c shell.c

set DB=.\t.db
del %DB%

gcc -s -o janbar.exe sqlite3.c janbar.c
.\janbar.exe %DB%
