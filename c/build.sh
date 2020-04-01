#!/bin/bash -x

gcc -s -o sqlite3 sqlite3.c shell.c -lpthread -ldl

DB=./t.db
rm $DB

gcc -s -o janbar sqlite3.c janbar.c -lpthread -ldl
./janbar $DB
