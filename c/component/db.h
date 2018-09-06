//
// Created by hujianzhe
//

#ifndef UTIL_C_COMPONENT_DB_H
#define UTIL_C_COMPONENT_DB_H

#include "../datastruct/url.h"
#include "../platform_define.h"

#ifdef DB_ENABLE_MYSQL
	#if defined(_WIN32) || defined(_WIN64)
		#include <mysql.h>
		#pragma comment(lib, "libmysql.lib")/* you need copy libmysql.dll to your exe path */
	#else
		#include <mysql/mysql.h>
	#endif
#endif
#include <stddef.h>

typedef enum DB_RETURN {
	DB_ERROR,
	DB_SUCCESS
} DB_RETURN;

enum {
	DB_FIELD_TYPE_TINY,
	DB_FIELD_TYPE_SMALLINT,
	DB_FIELD_TYPE_INT,
	DB_FIELD_TYPE_BIGINT,
	DB_FIELD_TYPE_FLOAT,
	DB_FIELD_TYPE_DOUBLE,
	DB_FIELD_TYPE_VARCHAR,
	DB_FIELD_TYPE_BLOB,
};

/* HANDLE */
typedef struct DBHandle_t {
	int type;
	short initok;
	short connectok;
	union {
		char reserved[1];
#ifdef DB_ENABLE_MYSQL
		struct {
			MYSQL mysql;
			const char* error_msg;
		} mysql;
#endif
	};
} DBHandle_t;

/* STMT */
typedef struct DBStmt_t {
	struct DBHandle_t* handle;
	union {
		char reserved[1];
#ifdef DB_ENABLE_MYSQL
		struct {
			MYSQL_STMT* stmt;
			MYSQL_BIND* exec_param;
			unsigned short exec_param_count;
			unsigned short result_field_count;
			MYSQL_RES* result_field_meta;
			MYSQL_BIND* result_field_param;
			int result_index;
			const char* error_msg;
		} mysql;
#endif
	};
} DBStmt_t;

#ifdef __cplusplus
extern "C" {
#endif

/* env */
UTIL_LIBAPI DB_RETURN dbInitEnv(const char* dbtype);
UTIL_LIBAPI void dbCleanEnv(const char* dbtype);
UTIL_LIBAPI DB_RETURN dbAllocTls(void);
UTIL_LIBAPI void dbFreeTls(void);
/* handle */
UTIL_LIBAPI DBHandle_t* dbCreateHandle(DBHandle_t* handle, const char* dbtype);
UTIL_LIBAPI void dbCloseHandle(DBHandle_t* handle);
UTIL_LIBAPI DBHandle_t* dbConnect(DBHandle_t* handle, const char *ip, unsigned short port, const char *user, const char *pwd, const char *dbname, int timeout_sec);
UTIL_LIBAPI DBHandle_t* dbConnectURL(DBHandle_t* handle, URL_t* url, int timeout_sec);
UTIL_LIBAPI DBHandle_t* dbConnectStringURL(DBHandle_t* handle, const char* str, int timeout_sec);
UTIL_LIBAPI DB_RETURN dbCheckAlive(DBHandle_t* handle);
UTIL_LIBAPI const char* dbHandleErrorMessage(DBHandle_t* handle);
/* transaction */
UTIL_LIBAPI DB_RETURN dbEnableAutoCommit(DBHandle_t* handle, int bool_val);
UTIL_LIBAPI DB_RETURN dbCommit(DBHandle_t* handle, int bool_val);
/* SQL execute */
UTIL_LIBAPI DBStmt_t* dbAllocStmt(DBHandle_t* handle, DBStmt_t* stmt);
UTIL_LIBAPI DB_RETURN dbCloseStmt(DBStmt_t* stmt);
UTIL_LIBAPI const char* dbStmtErrorMessage(DBStmt_t* stmt);
typedef struct DBExecuteParam_t {
	int field_type;
	const void* buffer;
	size_t buffer_length;
} DBExecuteParam_t;
UTIL_LIBAPI DB_RETURN dbSQLPrepareExecute(DBStmt_t* stmt, const char* sql, size_t sqllen, DBExecuteParam_t* param, unsigned short paramcnt);
/* result set */
UTIL_LIBAPI long long dbAutoIncrementValue(DBStmt_t* stmt);
UTIL_LIBAPI long long dbAffectedRows(DBStmt_t* stmt);
UTIL_LIBAPI short dbGetResult(DBStmt_t* stmt);
UTIL_LIBAPI DB_RETURN dbFreeResult(DBStmt_t* stmt);
typedef struct DBResultParam_t {
	void* buffer;
	size_t buffer_length;
	size_t* value_length;
	union {
		unsigned long mysql_value_length;
	};
} DBResultParam_t;
UTIL_LIBAPI short dbFetchResult(DBStmt_t* stmt, DBResultParam_t* param, unsigned short paramcnt);

#ifdef __cplusplus
}
#endif

#endif
