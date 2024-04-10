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
#ifndef KATLA_SQLITE_DATABASE_H
#define KATLA_SQLITE_DATABASE_H

#include "katla/core/core.h"
#include "katla/core/error.h"

#include "sqlite3.h"

#include <memory>
#include <vector>
#include <optional>
#include <gsl/span>

namespace katla {

struct SqliteTableData {
    size_t nrOfColumns {};
    std::vector<std::string> columnNames;
    std::vector<std::string> data;
};

struct SqliteQueryResult {
    std::optional<SqliteTableData> queryResult;
};

class SqliteDatabase {
public:
    SqliteDatabase();
    virtual ~SqliteDatabase();

    outcome::result<void, Error> init();

    outcome::result<void, Error> open();
    outcome::result<void, Error> close();

    outcome::result<void, Error> create(std::string path);

    outcome::result<SqliteQueryResult, Error> exec(std::string sql);

    outcome::result<SqliteQueryResult, Error> insert(std::string table,  gsl::span<std::pair<std::string, std::string>> values);

    void setPath(std::string path) {
        m_path = path;
    }

    bool isOpen () {
        return m_handle != nullptr;
    }

private:
    sqlite3* m_handle {};
    std::string m_path {};
};

}

#endif
