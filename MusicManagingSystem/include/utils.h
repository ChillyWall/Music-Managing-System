#ifndef MMS_UTILS_H
#define MMS_UTILS_H

#include "types.h"
#include <sqlite3.h>

// 数据库指针
typedef sqlite3 *pdb;
typedef sqlite3_stmt *pstmt;

/* 用于链接数据库，若不存在则创建一个
 * 返回数据库的指针 */
int connect_db(pdb *ppdb);

/* 创建表
 * 返回0则创建成功 */
int create_tables(pdb db);

/* 向数据库中添加一首歌曲, 若已存在则更新其他信息
 * 返回0则添加完成，返回1表示已存在并更新，其他值则表示运行错误 */
int add_song(pdb db, Song *song);

/* 从数据库中删除一首歌曲
 * 返回0则删除完成，其他值则删除失败 */
int delete_song(pdb db, const char *title, const char *album);

/* 删除歌曲数组中的所有歌曲
 * 该函数并不会释放arr的资源 */
int delete_songs(pdb db, SongArray *arr);

/* 搜索特定歌曲
 * 返回0则表示查找成功，1表示不存在，其他表示出错
 * 若不存在或查找失败，则不会对song进行更改
 * 若查找成功，需要使用destruct_song释放song中的资源 */
int search(pdb db, Song *song, const char *title, const char *album);

/* 筛选歌曲
 * args为字符串数组, 分别为title, album, singer, lyricist, composer,
 * arranger的值 若为NULL则忽略该条件,
 * 返回0则筛选成功，其他值则筛选失败
 * 需要使用destruct_song_array 释放资源 */
int filter(pdb db, SongArray *arr, const char **args);

/* 清空数据库中所有数据 */
int clear_all(pdb db);

/* 关闭数据库 */
int close_db(pdb db);

/* 用于从命令行读取歌曲信息
 * 创建的Song需要使用destruct_song释放 */
int read_song(Song *song);

/* 释放Song对象的内存 */
void destruct_song(Song *song);

/* 销毁SongArray对象，释放其中数组的内存,
 * 同时会对每一个Song对象使用destruct_song释放资源 */
void destruct_song_array(SongArray *arr);

/* 打印歌曲信息 */
int print_song_info(Song *song);

/* 打印歌曲数组 */
int print_song_array(SongArray *song_arr);

#endif // !UTILS_H
