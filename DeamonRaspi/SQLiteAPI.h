#ifndef SQLiteAPI_
#define SQLiteAPI_

#ifdef __cplusplus
extern "C" {
#endif

#include "sqlite3.h"

#define SQL_STR_SIZE 512 // I think this is large enough to store string


int ConnectDB(sqlite3 * * dbfile, const char * filename);
void DisonnectDB(sqlite3 * dbfile, int isOpenDB);

int insert_sensor_value(sqlite3 * dbfile, int isOpenDB, const char * raw_data);

#ifdef __cplusplus
}
#endif
#endif
