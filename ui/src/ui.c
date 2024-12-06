#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ui.h>

void _remove_newline(char *str) {
    char *pos = strchr(str, '\n');
    if (pos != NULL) {
        *pos = '\0';
    }
}

void _clear_stdin() {
    while (getchar() != '\n') {}
}

int main() {
    int rc = menu_main();
    if (rc != 0) {
        fprintf(stderr, "Failed to launch the system: %d\n", rc);
    }
    return 0;
}

int menu_main() {
    pdb db = NULL;
    int rc = connect_db(&db);
    if (rc != SQLITE_OK) {
        return -1;
    }

    rc = create_tables(db);
    if (rc != SQLITE_OK) {
        return -2;
    }

    printf("欢迎使用音乐管理系统\n");
    int active = 1;
    while (active) {
        printf("请输入指令(0: 退出, 1: 添加, 2: 删除, 3: 查询, 4: 筛选): ");
        int arg;
        rc = scanf("%d", &arg);
        _clear_stdin();
        if (rc != 1) {
            printf("未知指令, 请重新输入\n");
            continue;
        }
        switch (arg) {
        case 0:
            active = 0;
            break;
        case 1:
            menu_add(db);
            break;
        case 2:
            menu_delete(db);
            break;
        case 3:
            menu_search(db);
            break;
        case 4:
            menu_filter(db);
            break;
        default:
            printf("未知指令, 请重新输入\n");
        }
    }
    return 0;
}

int menu_add(pdb db) {
    int active = 1;
    int rc;
    while (active) {
        printf("请输入指令(0: 回退到上一级, 1: 添加一首歌): ");
        int arg;
        rc = scanf("%d", &arg);
        _clear_stdin();
        if (rc != 1) {
            printf("未知指令, 请重新输入: %d, %d\n", rc, arg);
            continue;
        }

        Song song;
        switch (arg) {
        case 0:
            active = 0;
            break;
        case 1:
            read_song(&song);
            rc = add_song(db, &song);

            if (rc == 1) {
                printf("该歌曲已经存在并更新为此次的数据\n");
            } else if (rc == 0) {
                printf("添加成功\n");
            } else {
                break;
            }
            destruct_song(&song);
            break;
        default:
            printf("未知指令, 请重新输入\n");
        }
    }
    return 0;
}

int menu_delete(pdb db) {
    int active = 1;
    int rc;
    size_t len;
    char *title;
    char *album;
    while (active) {
        printf("请输入指令(0: 回退到上一级, 1: 删除一首歌): ");
        int arg;
        rc = scanf("%d", &arg);
        _clear_stdin();
        if (rc != 1) {
            printf("未知指令, 请重新输入\n");
            continue;
        }
        switch (arg) {
        case 0:
            active = 0;
            break;
        case 1:
            printf("输入歌名: ");
            getline(&title, &len, stdin);
            _remove_newline(title);
            printf("输入专辑名: ");
            getline(&album, &len, stdin);
            _remove_newline(album);

            rc = delete_song(db, title, album);
            if (rc == 0) {
                printf("删除成功\n");
            }
            break;
        default:
            printf("未知指令,请重新输入\n");
        }
    }
    return 0;
}

int menu_search(pdb db) {
    return 0;
}

int menu_filter(pdb db) {
    return 0;
}
