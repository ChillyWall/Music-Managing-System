#ifndef TYPES_H
#define TYPES_H
/*
 * 用于声明类型或类型别名
 */

#include <sqlite3.h>
#include <stddef.h>

// 数据库指针
typedef sqlite3* pdb;
typedef sqlite3_stmt* pstmt;

/* 歌曲类型 */
typedef struct {
    const char* title;  // 歌曲标题
    const char* artist; // 歌手名字
    const char* album;  // 专辑名称
} Song;

/* 歌曲数组 */
typedef struct {
    Song* data;  // 数组
    size_t size; // 长度
} SongArray;

/* 歌手类型 */
typedef struct {
    const char* name;   // 姓名
    const char* gender; // 性别
    SongArray songs;    // 歌曲作品
} Artist;

/* 专辑类型 */
typedef struct {
    const char* title;       // 标题
    SongArray songs;         // 包含歌曲
    const char* description; // 描述
} Album;

#endif // !TYPES_H
