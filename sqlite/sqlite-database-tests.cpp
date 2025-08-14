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

#include "katla/core/core.h"
#include "sqlite-database.h"
#include "sqlite-errors.h"

#include "gtest/gtest.h"

#include <filesystem>

namespace katla {

    using namespace std::chrono_literals;

    std::string helloWorld = "Hello World!";

    void cleanAndPrepare() {
        if (std::filesystem::exists("/tmp/katla-sqlite-test/sqlite.db")) {
            std::filesystem::remove("/tmp/katla-sqlite-test/sqlite.db");
        }

        std::error_code error;
        std::filesystem::create_directories("/tmp/katla-sqlite-test/", error);
    }
    SqliteDatabase createAndOpenDatabase() {
        SqliteDatabase sqliteDatabase;
        [[maybe_unused]] auto _ = sqliteDatabase.init();
        _ = sqliteDatabase.create("/tmp/katla-sqlite-test/sqlite.db");
        sqliteDatabase.setPath("/tmp/katla-sqlite-test/sqlite.db");
        _ = sqliteDatabase.open();

        return sqliteDatabase;
    }

    TEST(KatlaSqliteDatabaseTests, CreateTest) {

        cleanAndPrepare();

        SqliteDatabase sqliteDatabase;
        auto createResult = sqliteDatabase.create("/tmp/katla-sqlite-test/sqlite.db");
        ASSERT_TRUE(createResult) << createResult.error().message();
    }

    TEST(KatlaSqliteDatabaseTests, AlreadyExistTest) {

        cleanAndPrepare();

        SqliteDatabase sqliteDatabase;
        auto createResult = sqliteDatabase.create("/tmp/katla-sqlite-test/sqlite.db");
        ASSERT_TRUE(createResult) << createResult.error().message();
        createResult = sqliteDatabase.create("/tmp/katla-sqlite-test/sqlite.db");

        // TODO same category?
        ASSERT_TRUE(createResult.error().code().value() == katla::make_error_code(katla::SqliteErrorCodes::DatabaseAlreadyExists).value());
    }

    TEST(KatlaSqliteDatabaseTests, CreateTable) {

        cleanAndPrepare();
        auto sqliteDatabase = createAndOpenDatabase();

        [[maybe_unused]] auto _ = sqliteDatabase.exec("CREATE TABLE IF NOT EXISTS test (id TEXT NOT NULL PRIMARY KEY, rootId TEXT, timestamp INTEGER, event TEXT, data TEXT);");

        [[maybe_unused]] auto _1 = sqliteDatabase.close();
    }

TEST(KatlaSqliteDatabaseTests, InsertInTable) {

    cleanAndPrepare();
    auto sqliteDatabase = createAndOpenDatabase();

    [[maybe_unused]] auto _ = sqliteDatabase.exec("CREATE TABLE IF NOT EXISTS test (id TEXT NOT NULL PRIMARY KEY, rootId TEXT, timestamp INTEGER, event TEXT, data TEXT);");
    _ = sqliteDatabase.exec( "INSERT INTO test (id, rootId, timestamp, event, data) VALUES('id1', 'rootid1', 1, 'event1', 'data1');");
    _ = sqliteDatabase.exec( "INSERT INTO test (id, rootId, timestamp, event, data) VALUES('id2', 'rootid2', 2, 'event1', 'data1');");

    [[maybe_unused]] auto _1 = sqliteDatabase.close();
}

TEST(KatlaSqliteDatabaseTests, QueryTable) {

    cleanAndPrepare();
    auto sqliteDatabase = createAndOpenDatabase();

    [[maybe_unused]] auto _ = sqliteDatabase.exec("CREATE TABLE IF NOT EXISTS test (id TEXT NOT NULL PRIMARY KEY, rootId TEXT, timestamp INTEGER, event TEXT, data TEXT);");
    _ = sqliteDatabase.exec( "INSERT INTO test (id, rootId, timestamp, event, data) VALUES('id1', 'rootid1', 1, 'event1', 'data1');");
    _ = sqliteDatabase.exec( "INSERT INTO test (id, rootId, timestamp, event, data) VALUES('id2', 'rootid2', 2, 'event1', 'data1');");

    auto result = sqliteDatabase.exec( "SELECT * FROM test;");
    if (result.has_error()) {
        katla::printError("{}: {}", result.error().message(), result.error().description());
        ASSERT_FALSE(result.has_error()) << result.error().message();
    }

    katla::printInfo("{::<10}", result.value().queryResult->columnNames);

    auto& data = result.value().queryResult->data;
    auto nrOfColumns = result.value().queryResult->nrOfColumns;

    for (int r = 0; r < data.size(); r += nrOfColumns) {
        std::vector<std::string> rowData = { data.begin() + r, data.begin() + r + nrOfColumns };
        katla::printInfo("{::<10}", rowData);
    }

    [[maybe_unused]] auto _1 = sqliteDatabase.close();
}

}

