package main

import (
	"database/sql"
	"fmt"
	"math/rand"
	"os"
	"time"
	"unsafe"

	_ "github.com/mattn/go-sqlite3"
)

func main() {
	if len(os.Args) != 2 {
		return
	}
	db, err := openSqlite3(os.Args[1])
	if err != nil {
		panic(err)
	}
	_, err = db.Exec("CREATE TABLE IF NOT EXISTS COMPANY(ID INTEGER PRIMARY KEY AUTOINCREMENT,NAME TEXT NOT NULL)")
	if err != nil {
		panic(err)
	}

	sqlByte := []byte("INSERT INTO COMPANY('NAME') VALUES('a2345678912345678912345678b')")
	rand.Seed(time.Now().UnixNano())
	sqlStr := *(*string)(unsafe.Pointer(&sqlByte))

	tx, err := db.Begin()
	if err != nil {
		panic(err)
	}
	defer tx.Rollback()
	for i := 0; i < 100000; i++ {
		for j := 36; j <= 62; j++ {
			sqlByte[j] = '0' + byte(rand.Intn(10))
		}
		_, err = tx.Exec(sqlStr)
		if err != nil {
			panic(err)
		}
	}
	err = tx.Commit()
	if err != nil {
		panic(err)
	}
	_, err = db.Exec("delete from COMPANY where ID > 100")
	if err != nil {
		panic(err)
	}
	cnt := -1
	err = db.QueryRow("select count(ID) from COMPANY").Scan(&cnt)
	if err != nil {
		panic(err)
	}
	fmt.Println("count:", cnt)
}

func openSqlite3(f string) (*sql.DB, error) {
	db, err := sql.Open("sqlite3", f)
	if err != nil {
		return nil, err
	}
	if err = db.Ping(); err != nil {
		return nil, err
	}
	db.SetMaxIdleConns(10)
	db.SetMaxOpenConns(50)
	return db, nil
}
