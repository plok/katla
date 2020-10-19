/***
 * Copyright 2020 The Katla Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "sqlite-errors.h"

std::string katla::SqliteErrorCategory::message(int c) const {
    switch (static_cast<SqliteErrorCodes>(c))
    {
        case SqliteErrorCodes::DatabaseAlreadyExists:
            return "Database already exists";
        case SqliteErrorCodes::DatabaseNotFound:
            return "Database not found";
        case SqliteErrorCodes::CreateDatabaseFailed:
            return "Create database failed";
        case SqliteErrorCodes::OpenDatabaseFailed:
            return "Opening database failed";
        case SqliteErrorCodes::CloseDatabaseFailed:
            return "Closing database failed";
        case SqliteErrorCodes::DatabaseNotOpened:
            return "Database not opened";
        case SqliteErrorCodes::QueryFailed:
            return "Query failed";
    }

    return "unknown";
}

std::error_condition katla::SqliteErrorCategory::default_error_condition(int c) const noexcept {
    switch (static_cast<SqliteErrorCodes>(c))
    {
        case SqliteErrorCodes::DatabaseAlreadyExists:
            return make_error_condition(std::errc::file_exists);
        case SqliteErrorCodes::DatabaseNotFound:
            return make_error_condition( std::errc::io_error);
        case SqliteErrorCodes::CreateDatabaseFailed:
            return make_error_condition(std::errc::io_error);
        case SqliteErrorCodes::OpenDatabaseFailed:
            return make_error_condition(std::errc::io_error);
        case SqliteErrorCodes::CloseDatabaseFailed:
            return make_error_condition(std::errc::io_error);
        case SqliteErrorCodes::DatabaseNotOpened: {
            return make_error_condition(std::errc::io_error);
        }
        case SqliteErrorCodes::QueryFailed: {
            std::error_condition(c, *this);
        }
    }

    return std::error_condition(c, *this);
}

const katla::SqliteErrorCategory &katla::sqliteErrorCategory() {
    static katla::SqliteErrorCategory c;
    return c;
}


