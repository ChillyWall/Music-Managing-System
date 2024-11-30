#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <sqlite3.h>

/* 用于链接数据库，若不存在则创建一个
 * 返回数据库的指针 */
pdb connect_db();

/* 创建表
 * 返回0则创建成功 */
int create_tables(pdb db);

/* 向数据库中添加一首歌曲
 * 返回0则添加完成，返回其他值则表示添加失败或已添加 */
int add_song(const char* title, const char* artist, const char* album, pdb db);

/* 从数据库中删除一首歌曲
 * 返回0则删除完成，其他值则删除失败 */
int delete_song(const char* title, pdb db);

/* 向数据库中添加一位歌手
 * 返回0则添加完成，返回其他值则表示添加失败 */
int add_artist(const char* name, const char* gender, pdb db);

/* 从数据库中删除一位歌手及其所有歌曲
 * 返回0则删除完成，其他值则删除失败 */
int delete_by_artist(const char* name, pdb db);

/* 向数据库中添加一张专辑
 * 返回0则添加完成，返回其他值则表示添加失败 */
int add_album(const char* title, pdb db);

/* 从数据库中删除一张专辑及其中所有歌曲
 * 返回0则删除完成，其他值则删除失败 */
int delete_album(const Album* title, pdb db);

/* 根据歌曲名搜索歌曲
 * 返回Song类型 */
Song search_by_title(const char* title, pdb db);

/* 根据歌手搜索歌曲
 * 返回Artist类型 */
Artist search_by_artist(const char* artist, pdb db);

/* 根据专辑搜索歌曲
 * 返回Album类型 */
Album search_by_album(const char* album, pdb db);

/* 销毁SongArray对象，释放其中数组的内存 */
void destruct_song_array(SongArray* itms);

#endif // !UTILS_H
