#include "utils.h"
#include <sqlite3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>

pdb connect_db() {
    pdb db;
    // 连接数据库
    int rc = sqlite3_open("../data.db", &db);
    if (rc) {
        fprintf(stderr, "Cannot open the database.\n");
        exit(rc);
    }
    return db;
}

int create_tables(pdb db) {
    char* errmsg = NULL; // 错误信息
    // 创建表songs
    int rc = sqlite3_exec(db,
                          "CREATE TABLE IF NOT EXISTS songs (id INTEGER "
                          "PRIMARY KEY AUTOINCREMENT UNIQUE, title "
                          "TEXT, artist TEXT);",
                          NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table songs: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 1;
    }

    // 创建表artists
    rc = sqlite3_exec(db,
                      "CREATE TABLE IF NOT EXISTS artists (id INTEGER PRIMARY "
                      "KEY AUTOINCREMENT UNIQUE, name "
                      "TEXT UNIQUE, gender TEXT);",
                      NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table artists: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 1;
    }

    return 0;
}

int add_song(const char* title, const char* artist, pdb db) {
    // 准备搜索语句
    char* sql = "SELECT * FROM songs WHERE title = ?";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, title, 0, SQLITE_TRANSIENT);
    // 搜索
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    while (rc == SQLITE_ROW) { // 搜索成功且同名歌曲已经在数据库中
        if (strcmp((const char*) sqlite3_column_text(stmt, 2), artist) == 0) {
            // 相同歌曲已经在数据库中
            return 0;
        }
        rc = sqlite3_step(stmt);
    }
    if (rc == SQLITE_DONE) { // 搜索成功且该歌曲不在数据库中
        // 准备语句
        char* sql = "INSERT INTO songs (title, artist) VALUES (?, ?);";
        stmt = NULL;
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, artist, -1, SQLITE_TRANSIENT);
        // 执行插入
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) { // 插入失败
            fprintf(stderr, "Error while inserting: %d\n", rc);
            return 2;
        }
    } else if (rc != SQLITE_ROW) { // 搜索失败
        fprintf(stderr, "Error while searching for the song %s: %d", title, rc);
        return 1;
    }
    return 0;
}

int delete_song(const char* title, pdb db) {
    // 准备语句
    char* sql = "DELETE FROM songs WHERE title = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, title, 0, SQLITE_TRANSIENT);
    // 执行删除操作
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { // 删除失败
        fprintf(stderr, "Failed to delete the song %s: %d\n", title, rc);
        return 2;
    }
    return 0;
}

int add_artist(const char* name, const char* gender, pdb db) {
    // 准备语句
    char* sql = "SELECT * FROM artists WHERE name = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, name, 0, SQLITE_TRANSIENT);
    // 执行搜索
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) { // 搜索成功且歌手不在数据库中
        // 准备语句
        char* sql = "INSERT INTO artists (name, gender) VALUES (?, ?);";
        stmt = NULL;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, gender, -1, SQLITE_TRANSIENT);
        // 执行插入操作
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) { // 插入失败
            fprintf(stderr, "Error while inserting: %d\n", rc);
            return 2;
        }
    } else if (rc != SQLITE_ROW) { // 搜索失败
        fprintf(stderr, "Failed to search the song before inserting: %d\n", rc);
        return 1;
    }
    return 0;
}

int delete_artist(const char* name, pdb db) {
    // 准备语句
    char* sql = "DELETE FROM artists WHERE name = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name, 0, SQLITE_TRANSIENT);
    // 执行删除操作
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { // 删除失败
        fprintf(stderr, "Failed to delete the artist %s: %d.\n", name, rc);
        return 1;
    }
    return 0;
}

int delete_by_artist(const char* name, pdb db) {
    // 先删除歌手
    int rc = delete_artist(name, db);
    if (rc != 0) {
        return rc;
    }

    // 准备语句
    char* sql = "DELETE FROM songs WHERE artist = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name, 0, SQLITE_TRANSIENT);
    // 执行删除操作
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { // 删除失败
        fprintf(stderr, "Failed to delete the songs: %d.\n", rc);
        return 1;
    }
    return 0;
}

int search_by_title(const char* title, pdb db, Song* song) {
    // 准备语句
    char* sql = "SELECT * FROM songs WHERE title = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, title, 0, SQLITE_TRANSIENT);
    // 执行搜索
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) { // 搜索成功且查找到结果
        // 填充数据
        song->id = (uint64_t) sqlite3_column_int64(stmt, 0);
        song->title = title;
        song->artist = (const char*) sqlite3_column_text(stmt, 2);
    } else if (rc == SQLITE_DONE) { // 未找到结果
        // 不做任何处理
        return 1;
    } else { // 搜索失败
        fprintf(stderr, "Failed to search for song.\n");
        return 2;
    }
    // 释放资源
    sqlite3_finalize(stmt);
    return 0;
}

int search_by_artist(const char* artist, pdb db, SongArray* song_arr) {
    // 先搜索确定结果个数
    size_t size;
    // 准备语句
    char* sql = "SELECT COUNT(*) FROM songs WHERE artist = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, artist, 0, SQLITE_TRANSIENT);
    // 执行搜索
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) { // 搜索成功
        // 读取统计结果
        size = sqlite3_column_int(stmt, 0);
        if (size == 0) { // 箬没有符合条件的结果直接退出，返回值1
            return 1;
        }
    } else { // 搜索失败
        fprintf(stderr, "Failed to search for songs.\n");
        return 2;
    }
    sqlite3_finalize(stmt);
    song_arr->size = size;

    // 动态分配内存以保存结果
    song_arr->data = (Song*) malloc(size * sizeof(Song));
    // 准备语句
    sql = "SELECT * FROM songs WHERE artist = ?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, artist, 0, SQLITE_TRANSIENT);
    Song* ptr = song_arr->data;
    // 执行搜索
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) { // 搜索到结果
        ptr->id = sqlite3_column_int64(stmt, 0);
        ptr->title = (const char*) sqlite3_column_text(stmt, 1);
        ptr->artist = artist;
        ++ptr;
    }
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error while searching for songs.\n");
        return 2;
    }
    sqlite3_finalize(stmt);
    return 0;
}

void destruct_song_array(SongArray* itms) {
    free(itms->data);
}
