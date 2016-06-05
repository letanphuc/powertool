#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SQLiteAPI.h"

// return 1 for opened database, return 0 for fail while open database
int ConnectDB(sqlite3 * * dbfile, const char * filename)
{
	if (sqlite3_open_v2(filename, dbfile, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_WAL, NULL) == SQLITE_OK)
	{
		return 1;
	}

	return 0;
}

void DisonnectDB(sqlite3 * dbfile, int isOpenDB)
{
	if (isOpenDB == 1)
	{
		sqlite3_close(dbfile);
	}
}

int insert_sensor_value(sqlite3 * dbfile, int isOpenDB, const char * raw_data)
{
	sqlite3_stmt *statement;
	int result = -1;	
	char query[1024] = "INSERT INTO data(value1, value2, value3, value4) VALUES(";
	char end_query[] = ")";

	strcat(query, raw_data);
	strcat(query, end_query);	
	printf("insert data cmd: %s\r\n", query);
	{
		if (isOpenDB)
		{			
			int prepare_query = sqlite3_prepare(dbfile, query, -1, &statement, 0);
			if (prepare_query == SQLITE_OK)
			{
				int res = sqlite3_step(statement);
				result = res;
				sqlite3_finalize(statement);
			}
			return result;
		}
		else
		{
			result = 0;
		}
	}
	return result;
}
