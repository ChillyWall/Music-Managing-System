#include "utils.h"
#include <locale.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <wchar.h>

// 列名
const char *columns[6] = {"title",    "album",    "singer",
                          "lyricist", "composer", "arranger"};

void _fill_song(pstmt stmt, Song *song);
void _remove_newline(char *str);

int connect_db(pdb *ppdb) {
    // 连接数据库
    int rc = sqlite3_open("../data.db", ppdb);
    if (rc) {
        fprintf(stderr, "Cannot open the database.\n");
        return -1;
    }
    return 0;
}

int create_tables(pdb db) {
    char *errmsg = NULL; // 错误信息
    // 创建表songs
    int rc =
        sqlite3_exec(db,
                     "CREATE TABLE IF NOT EXISTS songs (id INTEGER PRIMARY KEY "
                     "AUTOINCREMENT UNIQUE, title TEXT, album TEXT, singer "
                     "TEXT, lyricist TEXT, composer TEXT, arranger TEXT);",
                     NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table songs: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    return 0;
}

int add_song(pdb db, Song *song) {
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

        // 执行更新
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) { // 更新失败
            fprintf(stderr, "Error while inserting the song: %d\n", rc);
            return -3;
        }

        return 1;
    } else { // 搜索失败
        fprintf(stderr, "Error while searching for the song: %d\n", rc);
        return -1;
    }
}

int delete_song(pdb db, const char *title, const char *album) {
    // 准备语句
    char sql[] = "DELETE FROM songs WHERE title = ? AND album = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, album, -1, SQLITE_TRANSIENT);
    // 执行删除操作
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) { // 删除失败
        fprintf(stderr, "Failed to delete the song: %d\n", rc);
        return -1;
    }
    return 0;
}

int delete_songs(pdb db, SongArray *arr) {
    Song *song;
    int rc;
    int res = 0;
    for (size_t i = 0; i < arr->size; i++) {
        song = arr->data + i;
        rc = delete_song(db, song->title, song->album);
        if (rc != 0) {
            fprintf(stderr, "The song %s - %s is not deleted", song->title,
                    song->album);
            res = -1;
        }
    }
    return res;
}

int search(pdb db, Song *song, const char *title, const char *album) {
    // 准备搜索语句
    const char *sql = "SELECT * FROM songs WHERE title = ? AND album = ?;";
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, album, -1, SQLITE_TRANSIENT);
    // 搜索
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) { // 搜索成功且找到结果
        _fill_song(stmt, song);
        rc = 0;
    } else if (rc == SQLITE_DONE) { // 搜索成功但未找到结果
        rc = 1;
    } else { // 搜索失败
        fprintf(stderr, "Failed to search for song: %d\n", rc);
        rc = -1;
    }
    sqlite3_finalize(stmt);
    return rc;
}

int filter(pdb db, SongArray *arr, const char **args) {
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
    char conditions[120] = "";
    if (cols > 0) { // 条件数不为0
        // 拼接条件语句
        char buf[20]; // 缓存区长度为20
        sprintf(buf, "WHERE %s = ?", columns[i_cols[0]]);
        strncat(conditions, buf, 20);
        for (int i = 1; i < cols; ++i) {
            sprintf(buf, "AND %s = ?", columns[i_cols[i]]);
            strncat(conditions, buf, 20);
        }
    }

    // 先查询符合条件的歌曲数量
    char sql1[150] = "SELECT COUNT(*) FROM songs ";
    // 拼接sql语句
    strncat(sql1, conditions, 150);
    pstmt stmt = NULL;
    sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL);

    // 绑定参数
    for (int i = 0; i < cols; ++i) {
        sqlite3_bind_text(stmt, i + 1, args[i_cols[i]], -1, SQLITE_TRANSIENT);
    }

    // 执行查询
    int rc = sqlite3_step(stmt);

    size_t count = 0;       // 符合条件的歌曲数量
    if (rc != SQLITE_ROW) { // 查询失败
        fprintf(stderr, "Failed to count the number of songs.\n");
        sqlite3_finalize(stmt);
        return -1;
    } else { // 查询成功
        count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }

    // 若没有符合条件的歌曲
    if (count == 0) {
        arr->size = 0;
        arr->data = NULL;
        return 0;
    }

    // 按照歌曲数量分配内存
    arr->data = malloc(count * sizeof(Song));
    // 逐个查询符合条件的歌曲
    char sql2[630] = "SELECT * FROM songs ";
    strncat(sql2, conditions, 600); // 拼接sql语句
    sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL);
    for (int i = 0; i < cols; ++i) { // 绑定参数
        sqlite3_bind_text(stmt, i + 1, args[i_cols[i]], -1, SQLITE_TRANSIENT);
    }

    Song *song = arr->data;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // 填充歌曲信息
        _fill_song(stmt, song);
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
    arr->size = count;
    sqlite3_finalize(stmt);
    return 0;
}

int clear_all(pdb db) {
    // 删除所有数据
    int rc = sqlite3_exec(db, "DELETE FROM songs;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to clear all data.\n");
        return -1;
    }
    return 0;
}

int close_db(pdb db) {
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to close the database.\n");
        return -1;
    }
    return 0;
}

int read_song(Song *song) {
    char *buf = NULL;
    size_t size;
    for (int i = 0; i < 6; i++) {
        printf("Enter the %s: ", columns[i]);
        getline(&buf, &size, stdin);
        _remove_newline(buf);
        switch (i) {
        case 0:
            song->title = buf;
            break;
        case 1:
            song->album = buf;
            break;
        case 2:
            song->singer = buf;
            break;
        case 3:
            song->lyricist = buf;
            break;
        case 4:
            song->composer = buf;
            break;
        case 5:
            song->arranger = buf;
            break;
        }
        buf = NULL;
    }
    return 0;
}

void destruct_song(Song *song) {
    free((void *) song->title);
    free((void *) song->album);
    free((void *) song->singer);
    free((void *) song->lyricist);
    free((void *) song->composer);
    free((void *) song->arranger);
    song->title = NULL;
    song->album = NULL;
    song->singer = NULL;
    song->lyricist = NULL;
    song->composer = NULL;
    song->arranger = NULL;
}

void destruct_song_array(SongArray *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        destruct_song(arr->data + i);
    }
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
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
    printf("There're %zd songs in total.\n", song_arr->size);
    for (size_t i = 0; i < song_arr->size; ++i) {
        print_song_info(song_arr->data + i);
    }
    return 0;
}

void _fill_song(pstmt stmt, Song *song) {
    char **buf;
    for (int i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            buf = &song->title;
            break;
        case 1:
            buf = &song->album;
            break;
        case 2:
            buf = &song->singer;
            break;
        case 3:
            buf = &song->lyricist;
            break;
        case 4:
            buf = &song->composer;
            break;
        case 5:
            buf = &song->arranger;
            break;
        }
        char *text = (char *) sqlite3_column_text(stmt, i + 1);
        size_t len = strlen(text);
        *buf = malloc(len + 1);
        strcpy(*buf, text);
    }
}

void _remove_newline(char *str) {
    char *pos = strchr(str, '\n');
    if (pos != NULL) {
        *pos = '\0';
    }
}
