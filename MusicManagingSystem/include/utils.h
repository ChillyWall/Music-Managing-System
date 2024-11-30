#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <sqlite3.h>

/* 用于链接数据库，若不存在则创建一个
 * 返回数据库的指针 */
pdb connect_db();

/* 向数据库中添加一个元素
 * 返回0则添加完成，返回其他值则表示添加失败 */
int add_item(const item* itm, pdb db);

/* 从数据库中删除一个元素
 * 返回0则删除完成，其他值则删除失败 */
int delete_item(const item* itm, pdb db);

/* 根据歌曲名搜索歌曲
 * 返回item类型 */
item search_title(const char* title, pdb db);

/* 根据歌手搜索歌曲
 * 返回items类型 */
items search_artist(const char* artist, pdb db);

/* 根据专辑搜索歌曲
 * 返回items类型 */
items search_album(const char* album, pdb db);

/* 销毁items对象，释放其中数组的内存 */
void destruct_items(items* itms);

#endif // !UTILS_H
