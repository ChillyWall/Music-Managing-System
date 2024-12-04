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
    char *errmsg = NULL; // 错误信息
    // 创建表songs
    int rc = sqlite3_exec(db,
                          "CREATE TABLE IF NOT EXISTS songs (id INTEGER "
                          "PRIMARY KEY AUTOINCREMENT UNIQUE, title "
                          "TEXT, artist TEXT);",
                          NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table songs: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}

int add_song(Song *song, pdb db) {
    Song tmp;
    int rc = search(db, &tmp, song->title, song->album);
    pstmt stmt = NULL;
    if (rc == 1) { // 搜索成功且该歌曲不在数据库中
        // 插入数据
        char sql[] =
            "INSERT INTO songs (title, album, singer, lyricist, composer, "
            "arranger) VALUES (?, ?, ?, ?, ?, ?);";
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, song->title, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, song->album, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, song->singer, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, song->lyricist, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, song->composer, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, song->arranger, -1, SQLITE_TRANSIENT);
        // 执行插入
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) { // 插入失败
            fprintf(stderr, "Error while inserting the song: %d\n", rc);
            return -2;
        }
        return 0;
    } else if (rc == 0) { // 搜索成功且该歌曲已在数据库中
        // 更新数据
        char sql[] =
            "UPDATE songs SET singer = ?, lyricist = ?, composer = ?, "
            "arranger = ? WHERE title = ? AND album = ?;";
        sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, song->singer, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, song->lyricist, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, song->composer, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, song->arranger, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, song->title, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, song->album, -1, SQLITE_TRANSIENT);

        // 执行插入
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) { // 更新失败
            fprintf(stderr, "Error while inserting the song: %d\n", rc);
            return -3;
        }

        return 1;
    } else { // 搜索失败
        fprintf(stderr, "Error while searching for the song: %d", rc);
        return -1;
    }
}

int delete_song(Song *song, pdb db) {
    // 准备语句
    char sql[] = "DELETE FROM songs WHERE title = ? AND album = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, song->title, 0, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 1, song->album, 0, SQLITE_TRANSIENT);
    // 执行删除操作
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { // 删除失败
        fprintf(stderr, "Failed to delete the song: %d\n", rc);
        return -1;
    }
    return 0;
}

int search(pdb db, Song *song, const char *title, const char *album) {
    // 准备搜索语句
    char sql[] = "SELECT * FROM songs WHERE title = ? AND album = ?";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, title, 0, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, album, 0, SQLITE_TRANSIENT);
    // 搜索
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) { // 搜索成功且找到结果
        song->title = title;
        song->album = album;
        song->singer = (const char *) sqlite3_column_text(stmt, 3);
        song->lyricist = (const char *) sqlite3_column_text(stmt, 4);
        song->composer = (const char *) sqlite3_column_text(stmt, 5);
        song->arranger = (const char *) sqlite3_column_text(stmt, 6);
        rc = 0;
    } else if (rc == SQLITE_DONE) { // 搜索成功但未找到结果
        rc = 1;
    } else { // 搜索失败
        fprintf(stderr, "Failed to search for song.\n");
        rc = -1;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int filter(pdb db, SongArray *arr, const char **args) {
    // 列名
    const char *columns[6] = {"title",    "album",    "singer",
                              "lyricist", "composer", "arranger"};
    // 条件对应列索引
    int i_cols[6];
    // 条件数量
    int cols = 0;
    // 记录条件对应列索引
    for (int i = 0; i < 6; ++i) {
        if (args[i] != NULL) {
            i_cols[cols++] = i;
        }
    }

    // 条件部分sql表达式
    char conditions[600] = "";
    if (cols > 0) { // 条件数不为0
        // 拼接条件语句, 每条语句长度限制为100个字符
        char buf[100];
        sprintf(buf, "WHERE %s = %s", columns[i_cols[0]], args[i_cols[0]]);
        strncat(conditions, buf, 100);
        for (int i = 1; i < cols; ++i) {
            sprintf(buf, "AND %s = %s", columns[i_cols[i]], args[i_cols[i]]);
            strncat(conditions, buf, 100);
        }
    }

    // 先查询符合条件的歌曲数量
    char sql1[630] = "SELECT COUNT(*) FROM songs ";
    strncat(sql1, conditions, 600);
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL);
    int rc = sqlite3_step(stmt);
    size_t size;
    if (rc != SQLITE_ROW) { // 查询失败
        fprintf(stderr, "Failed to count the number of songs.\n");
        sqlite3_finalize(stmt);
        return -1;
    } else { // 查询成功
        size = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // 若没有符合条件的歌曲
    if (size == 0) {
        arr->size = 0;
        arr->data = NULL;
        return 0;
    }

    // 按照歌曲数量分配内存
    arr->data = malloc(size * sizeof(Song));

    // 逐个查询符合条件的歌曲
    char sql2[630] = "SELECT * FROM songs ";
    strncat(sql2, conditions, 600);
    sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL);
    Song *song = arr->data;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // 填充歌曲信息
        song->title = (const char *) sqlite3_column_text(stmt, 1);
        song->album = (const char *) sqlite3_column_text(stmt, 2);
        song->singer = (const char *) sqlite3_column_text(stmt, 3);
        song->lyricist = (const char *) sqlite3_column_text(stmt, 4);
        song->composer = (const char *) sqlite3_column_text(stmt, 5);
        song->arranger = (const char *) sqlite3_column_text(stmt, 6);
        ++song;
    }
    if (rc != SQLITE_DONE) { // 非正常结束
        fprintf(stderr, "Failed to search for songs.\n");
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        sqlite3_finalize(stmt);
        return -2;
    }
    arr->size = size;
    sqlite3_finalize(stmt);
    return 0;
}

void destruct_song_array(SongArray *itms) {
    free(itms->data);
}

int print_song_info(Song *song) {
    printf(
        "Title: %s, Album: %s, Singer: %s, Lyricist: %s, Composer: %s, "
        "Arranger: %s\n",
        song->title, song->album, song->singer, song->lyricist, song->composer,
        song->arranger);
    return 0;
}

int print_song_array(SongArray *song_arr) {
    for (size_t i = 0; i < song_arr->size; ++i) {
        print_song_info(song_arr->data + i);
    }
    return 0;
}
