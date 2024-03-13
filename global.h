#pragma once
#include "qtypes.h"

#define CHECK_DOUBLES 1

typedef quint64 id_t;
typedef id_t user_id_t;
typedef quint32 date_t;

constexpr auto DB_PARAM_PROCESSED_POSITION = "processed_position";
constexpr auto DB_TYPE_NAME = "QSQLITE";
constexpr auto DB_SAVE_BY_SIZE = 10;
constexpr auto MEM_MAX_USERS_SIZE = 100000;
constexpr auto STRING_BUFFER_SIZE = 0x7FFF;
