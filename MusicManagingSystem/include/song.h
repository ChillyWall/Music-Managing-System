#ifndef ITEM_H
#define ITEM_H

#include <stddef.h>

/* 用于储存一个歌曲的信息 */
struct song {
    const char* title;  // 歌曲标题
    const char* artist; // 歌手名字
    const char* album;  // 专辑名称
};

struct songs {
    struct song* data;
    size_t size;
};

#endif // !ITEM_H
