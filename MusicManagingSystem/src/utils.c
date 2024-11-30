#include "utils.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

pdb connect_db() {
    pdb db;
    int msg = sqlite3_open("../data.db", &db);
    if (msg) {
        printf("Cannot open the database.\n");
        exit(msg);
    } else {
        printf("Successfully connect to the database.\n");
        return db;
    }
}

int create_tables(pdb db) {
    char* errmsg = NULL;
    int rc = sqlite3_exec(db,
                          "CREATE TABLE IF NOT EXISTS songs (id INTEGER "
                          "PRIMARY KEY AUTOINCREMENT, title "
                          "TEXT, artist TEXT, album TEXT);",
                          NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table songs: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 1;
    }

    rc = sqlite3_exec(db,
                      "CREATE TABLE IF NOT EXISTS artists (id INTEGER PRIMARY "
                      "KEY AUTOINCREMENT, name "
                      "TEXT, gender TEXT);",
                      NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table artists: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 1;
    }

    rc = sqlite3_exec(db,
                      "CREATE TABLE IF NOT EXISTS albums (id INTEGER PRIMARY "
                      "KEY AUTOINCREMENT, title TEXT, description TEXT);",
                      NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table albums: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 1;
    }
    printf("The tables are ready.\n");
    return 0;
}

int count_res(int* count, int col_num, char** col_value, char** col_name) {
    ++count;
    return 0;
}

int add_song(const char* title, const char* artist, const char* album, pdb db) {
    Song res = search_by_title(title, db);
    if (res.title == NULL) {
        char* sql =
            "INSERT INTO songs (title, artist, album) VALUES (?, ?, ?);";
        pstmt stmt = NULL;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, artist, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, album, -1, SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE) {
                printf("Successfully add a song.\n");
                return 0;
            } else {
                fprintf(stderr, "Error while inserting: %d\n", rc);
                return 3;
            }
        } else {
            fprintf(stderr, "Error while prevaring for inserting: %d\n", rc);
            return 2;
        }
    } else {
        printf("The song has been added before.\n");
        return 0;
    }
}

int delete_song(const char* title, pdb db) {
    char* sql = "DELETE FROM songs WHERE title = ?;";
    pstmt stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, title, 0, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            printf("Successfully delete the song.\n");
            return 0;
        } else {
            fprintf(stderr, "Failed to delete the song: %d\n", rc);
            return 2;
        }
    } else {
        fprintf(stderr, "Failed to prepare for deleting: %d\n", rc);
        return 1;
    }
}

int add_artist(const char* name, const char* gender, pdb db) {
    char* sql = "SELECT * FROM artists WHERE name = ?;";
    pstmt stmt = NULL;
    // working
    int rc = sqlite3_prepare_v2();
    if (res.title == NULL) {
        char* sql =
            "INSERT INTO songs (title, artist, album) VALUES (?, ?, ?);";
        pstmt stmt = NULL;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, artist, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, album, -1, SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE) {
                printf("Successfully add a song.\n");
                return 0;
            } else {
                fprintf(stderr, "Error while inserting: %d\n", rc);
                return 3;
            }
        } else {
            fprintf(stderr, "Error while prevaring for inserting: %d\n", rc);
            return 2;
        }
    } else {
        printf("The song has been added before.\n");
        return 0;
    }
}
