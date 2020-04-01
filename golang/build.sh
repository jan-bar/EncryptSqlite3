#!/bin/bash -x

GOPATH=$(pwd)/lib
CGO_ENABLED=1
DB=./t.db
rm $DB

go build -i -ldflags "-s -w" -o janbar sqlite3.go
./janbar $DB

echo "select *from COMPANY limit 50,5;" | ../c/sqlite3 $DB
