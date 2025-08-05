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
#include "sqlite-database.h"

#include "katla/core/error.h"
#include "katla/sqlite/sqlite-errors.h"

#include "sqlite3.h"

#include "fmt/core.h"

#include <filesystem>
#include <algorithm>

namespace katla {

SqliteDatabase::SqliteDatabase() {
}

SqliteDatabase::~SqliteDatabase() {
}

katla::result<void, Error> SqliteDatabase::init()
{
    return outcome::success();
}

katla::result<void, Error> SqliteDatabase::create(std::string path)
{
    if (std::filesystem::exists(path)) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseAlreadyExists), katla::format("Database exists at: {}", path));
    }

    sqlite3* handle;
    auto result = sqlite3_open_v2(path.c_str(), &handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    
    if (result != SQLITE_OK) {
        if (handle == nullptr) {
            return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseAlreadyExists), katla::format("Error creating database at: {}. Insufficient resources",
            path));
        }
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseAlreadyExists), katla::format("Error creating database at: {}. Reason: {}",
            path,
            sqlite3_errmsg(handle)));
    }

    sqlite3_close(handle);

    m_path = path;

    return outcome::success();
}

katla::result<void, Error> SqliteDatabase::open()
{
    if (!std::filesystem::exists(m_path)) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseNotFound), katla::format("Database not found at: {}", m_path));
    }

    auto result = sqlite3_open_v2(m_path.c_str(), &m_handle, SQLITE_OPEN_READWRITE, nullptr);

    if (result != SQLITE_OK) {
        if (m_handle == nullptr) {
            return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseAlreadyExists), katla::format("Error opening database connection: {}. Insufficient resources",
                                                                                                               m_path));
        }
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseAlreadyExists), katla::format("Error opening database at: {}. Reason: {}",
                                                                                                           m_path,
                                                                                                           sqlite3_errmsg(m_handle)));
    }

    return outcome::success();
}

katla::result<void, Error> SqliteDatabase::close()
{
    if (m_handle == nullptr) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseNotOpened), katla::format("Database not opened"));;
    }
    auto result = sqlite3_close(m_handle);
    if (result != SQLITE_OK) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::CloseDatabaseFailed), katla::format("Error closing database : {}", sqlite3_errmsg(m_handle)));
    }
    m_handle = {};

    return outcome::success();
}

int callbackFunc(void* user, int nrOfColumns, char** data, char** columnNames) {
    auto queryResult = reinterpret_cast<SqliteQueryResult*>(user);

    if (nrOfColumns == 0) {
        return 0;
    }

    if (!queryResult->queryResult.has_value()) {
        queryResult->queryResult = SqliteTableData {};
        queryResult->queryResult->nrOfColumns = nrOfColumns;

        for (int i = 0; i < nrOfColumns; i++) {
            queryResult->queryResult->columnNames.push_back(columnNames[i]);
        }
    }

    for (int i = 0; i<nrOfColumns; i++) {
        queryResult->queryResult->data.push_back(data[i]);
    }

    return 0;
}

katla::result<SqliteQueryResult, Error> SqliteDatabase::exec(std::string sql)
{
    if (m_handle == nullptr) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseNotOpened), katla::format("Database not opened"));;
    }

    char* errorMessage = nullptr;

    SqliteQueryResult queryResult;

    auto result = sqlite3_exec(
        m_handle,
        sql.c_str(),
        callbackFunc,
        &queryResult,
        &errorMessage
    );

    if (result != 0 && errorMessage) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::QueryFailed), katla::format("Error executing query : {}", errorMessage));
    }

    return queryResult;
}

katla::result<SqliteQueryResult, Error> SqliteDatabase::insert(std::string table, katla::span<std::pair<std::string, std::string>> values)
{
    if (m_handle == nullptr) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::DatabaseNotOpened), katla::format("Database not opened"));;
    }

    SqliteQueryResult queryResult;

    std::vector<std::string> columns;
    std::transform(values.begin(), values.end(), std::back_inserter(columns),
                   [](std::pair<std::string, std::string> pair) -> std::string { return pair.first; });

    std::vector<std::string> valueTemplates;
    for (size_t i = 0; i < values.size(); i++) {
        valueTemplates.push_back(katla::format("?{:0>#3}", i+1));
    }

    auto sqlColumns = fmt::format("({})", fmt::join(columns, ", "));
    auto sqlValueTemplates = fmt::format("({})", fmt::join(valueTemplates, ", "));

    auto queryTemplate = katla::format("INSERT INTO {} {} VALUES {};", table, sqlColumns, sqlValueTemplates);

    sqlite3_stmt* sqlStatement = nullptr;
    const char* pzTail;
    auto prepareResult = sqlite3_prepare_v2(
        m_handle,
        queryTemplate.c_str(),
        queryTemplate.size(),
        &sqlStatement,
        &pzTail
    );
    if (prepareResult != SQLITE_OK) {
        return Error(katla::make_error_code(katla::SqliteErrorCodes::QueryFailed), katla::format("Error creating query: {}", sqlite3_errstr(prepareResult)));
    }

    for (size_t i=0; i<values.size(); i++) {
        sqlite3_bind_text(sqlStatement, i + 1, values[i].second.c_str(), values[i].second.size(), SQLITE_STATIC);
    }

    int stepResult = SQLITE_ROW;
    while(stepResult != SQLITE_DONE) {
        stepResult = sqlite3_step(sqlStatement);

        if (stepResult != SQLITE_DONE && stepResult != SQLITE_ROW) {
            return Error(katla::make_error_code(katla::SqliteErrorCodes::QueryFailed), katla::format("Error executing query: {}", sqlite3_errstr(stepResult)));
        }

        // TODO needed for insert?
        int nrOfColumns = sqlite3_column_count(sqlStatement);

        if (!queryResult.queryResult.has_value()) {
            queryResult.queryResult = SqliteTableData {};
            queryResult.queryResult->nrOfColumns = nrOfColumns;

            for (int i = 0; i < nrOfColumns; i++) {

                auto name = sqlite3_column_name(sqlStatement, i);
                if (name) {
                    queryResult.queryResult->columnNames.push_back(std::string(name));
                }
            }
        }

        for (int i=0; i < nrOfColumns; i++) {
            auto value = sqlite3_column_text(sqlStatement, i);
            queryResult.queryResult->data.push_back(std::string(reinterpret_cast<const char*>(value)));
        }
    }

    return queryResult;
}

}
