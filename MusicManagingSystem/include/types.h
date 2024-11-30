#ifndef TYPES_H
#define TYPES_H
/*
 * 用于声明类型或类型别名
 */

#include "song.h"
#include <sqlite3.h>

/* 元素类型
 * 包括三个字符串：title, artist, album，表示歌曲名称，歌手名称和专辑名称*/
typedef struct song item;

/* 元素数组
 * 包括一个item数组data，和数组长度size */
typedef struct songs items;

// 数据库指针
typedef sqlite3* pdb;

#endif // !TYPES_H
