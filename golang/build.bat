@echo off

set GOPATH=%cd%\lib
set CGO_ENABLED=1
set DB=.\t.db
del %DB%

go build -i -ldflags "-s -w" -o janbar.exe sqlite3.go
.\janbar.exe %DB%

echo select *from COMPANY limit 50,5; | ..\c\sqlite3.exe %DB%
