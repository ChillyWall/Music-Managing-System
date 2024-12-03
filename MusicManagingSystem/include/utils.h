#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <sqlite3.h>

// 数据库指针
typedef sqlite3* pdb;
typedef sqlite3_stmt* pstmt;

/* 用于链接数据库，若不存在则创建一个
 * 返回数据库的指针 */
pdb connect_db();

/* 创建表
 * 返回0则创建成功 */
int create_tables(pdb db);

/* 向数据库中添加一首歌曲
 * 返回0则添加完成，返回其他值则表示添加失败或已添加 */
int add_song(const char* title, const char* artist, pdb db);

/* 从数据库中删除一首歌曲
 * 返回0则删除完成，其他值则删除失败 */
int delete_song(const char* title, pdb db);

/* 向数据库中添加一位歌手
 * 返回0则添加完成，返回其他值则表示添加失败 */
int add_artist(const char* name, const char* gender, pdb db);

/* 从数据库中删除一位歌手
 * 返回0则表示添加完成，其他职责表示删除失败 */
int delete_artist(const char* name, pdb db);

/* 从数据库中删除一位歌手及其所有歌曲
 * 返回0则删除完成，其他值则删除失败 */
int delete_by_artist(const char* name, pdb db);

/* 根据歌曲名搜索歌曲
 * 返回0则表示查找成功，1表示不存在，其他表示出错 */
int search_by_title(const char* title, pdb db, Song* song);

/* 根据歌手搜索歌曲
 * 返回0表示搜索成功，1表示结果为空，其他标识错误 */
int search_by_artist(const char* artist, pdb db, SongArray* song_arr);

/* 销毁SongArray对象，释放其中数组的内存 */
void destruct_song_array(SongArray* itms);

/* 打印歌曲信息 */
int print_song_info(Song* song);

/* 打印歌曲数组 */
int print_song_array(SongArray* song_arr);

/* 打印歌手信息 */
int print_artist_info(Artist* artist);

#endif // !UTILS_H
