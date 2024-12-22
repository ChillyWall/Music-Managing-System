#ifndef TYPES_H
#define TYPES_H
/*
 * 用于声明类型或类型别名
 */

#include <stddef.h>
#include <stdint.h>

/* 歌曲类型
 * 歌曲名允许重名，同名专辑的同名歌曲视为同一首歌 */
typedef struct {
    char *title;    // 歌曲标题
    char *album;    // 专辑名
    char *singer;   // 歌手名字
    char *lyricist; // 作词者
    char *composer; // 作曲者
    char *arranger; // 编曲者
} Song;

/* 歌曲数组 */
typedef struct {
    Song *data;  // 数组
    size_t size; // 长度
} SongArray;

#endif // !TYPES_H
