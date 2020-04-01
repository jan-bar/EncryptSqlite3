#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    if( argc!=2 ){
        fprintf(stderr, "Usage: %s DATABASE\n", argv[0]);
        return(1);
    }

    rc = sqlite3_open(argv[1], &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    const char *create = "CREATE TABLE IF NOT EXISTS COMPANY(ID INTEGER PRIMARY KEY AUTOINCREMENT,NAME TEXT NOT NULL)";
    rc = sqlite3_exec(db, create, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_close(db);
      return(1);
    }

    char sql[128];
    strcpy(sql, "INSERT INTO COMPANY('NAME') VALUES('a2345678912345678912345678b')");
    srand((unsigned)time(NULL));
    sqlite3_exec(db, "begin;", 0, 0, &zErrMsg); //开启事务
    for (int i=0;i<100000;i++) {
      for (int j=36;j<=62;j++) { // 随机数据内容
        sql[j] = '0' + (rand() % 10);
      }
      rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
      if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        break;
      }
    }
    if(rc == SQLITE_OK) {
      sqlite3_exec(db, "commit;", 0, 0, 0); 
    } else {
      sqlite3_exec(db, "rollback;", 0, 0, 0); 
    }
    
    strcpy(sql, "delete from COMPANY where ID > 100;");
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_close(db);
      return(1);
    }
    
    strcpy(sql, "select count(ID) from COMPANY;");
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_close(db);
      return(1);
    }
    
    sqlite3_close(db);
    return 0;
}
