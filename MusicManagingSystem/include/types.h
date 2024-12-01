#ifndef TYPES_H
#define TYPES_H
/*
 * 用于声明类型或类型别名
 */

#include <sqlite3.h>
#include <stddef.h>
#include <stdint.h>

/* 歌曲类型 */
typedef struct {
    uint64_t id;
    const char* title;  // 歌曲标题
    const char* artist; // 歌手名字
} Song;

/* 歌曲数组 */
typedef struct {
    Song* data;  // 数组
    size_t size; // 长度
} SongArray;

/* 歌手类型 */
typedef struct {
    uint64_t id;
    const char* name;   // 姓名
    const char* gender; // 性别
    SongArray songs;    // 歌曲作品
} Artist;

#endif // !TYPES_H
