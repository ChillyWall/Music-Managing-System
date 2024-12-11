#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui.h>
#include <utils.h>

void _remove_newline(char *str) {
    char *pos = strchr(str, '\n');
    if (pos != NULL) {
        *pos = '\0';
    }
}

void _clear_stdin() {
    while (getchar() != '\n' && getchar() != EOF) {}
}

int main() {
    int rc = menu_main();
    if (rc == -1) {
        fprintf(stderr, "打开数据库失败%d\n", rc);
    } else if (rc == -2) {
        fprintf(stderr, "创建表单失败%d\n", rc);
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
        printf("添加 - 请输入指令(0: 回退到上一级, 1: 添加一首歌): ");
        int arg;
        rc = scanf("%d", &arg);
        _clear_stdin();
        if (rc != 1) {
            printf("未知指令, 请重新输入\n");
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
            destruct_song(&song);

            if (rc == 1) {
                printf("该歌曲已经存在并更新为此次的数据\n");
            } else if (rc == 0) {
                printf("添加成功\n");
            } else {
                fprintf(stderr, "添加失败\n");
            }
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
    size_t len = 100;
    char *title = NULL;
    char *album = NULL;
    while (active) {
        printf("删除 - 请输入指令(0: 回退到上一级, 1: 删除一首歌): ");
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
            printf("输入标题: ");
            title = malloc(len * sizeof(char));
            fgets(title, len, stdin);
            _remove_newline(title);

            printf("输入专辑: ");
            album = malloc(len * sizeof(char));
            fgets(album, len, stdin);
            _remove_newline(album);

            rc = delete_song(db, title, album);
            if (rc == 0) {
                printf("删除成功\n");
            } else {
                fprintf(stderr, "删除失败\n");
            }
            free(title);
            title = NULL;
            free(album);
            album = NULL;
            break;
        default:
            printf("未知指令,请重新输入\n");
        }
    }
    return 0;
}

int menu_search(pdb db) {
    int active = 1;
    int rc;
    size_t len = 100;
    char *title = NULL;
    char *album = NULL;
    while (active) {
        printf("查找 - 请输入指令(0: 回退到上一级, 1: 查找一首歌): ");
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
            printf("输入标题: ");
            title = malloc(len * sizeof(char));
            fgets(title, len, stdin);
            _remove_newline(title);

            printf("输入专辑: ");
            album = malloc(len * sizeof(char));
            fgets(album, len, stdin);
            _remove_newline(album);

            Song song;
            rc = search(db, &song, title, album);
            if (rc == 0) {
                print_song_info(&song);
                destruct_song(&song);
            } else if (rc == 1) {
                printf("未找到该歌曲.\n");
            } else {
                fprintf(stderr, "查找失败\n");
            }

            free(title);
            title = NULL;
            free(album);
            album = NULL;
            break;
        default:
            printf("未知指令,请重新输入\n");
        }
    }
    return 0;
}

int menu_filter(pdb db) {
    int active = 1;
    int rc;
    size_t len;
    while (active) {
        printf("筛选 - 请输入指令(0: 回退到上一级, 1: 开始筛选): ");
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
            menu_filter_operation(db);
            break;
        default:
            printf("未知指令,请重新输入\n");
        }
    }
    return 0;
}

int menu_filter_operation(pdb db) {
    int rc;
    size_t len;
    size_t size = 100;
    char *args[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    for (int i = 0; i < 6; ++i) {
        printf("请输入%s: ", columns_zh[i]);
        args[i] = malloc(size * sizeof(char));
        fgets(args[i], size, stdin);
        _remove_newline(args[i]);
        len = strlen(args[i]);
        if (len == 0) {
            free(args[i]);
            args[i] = NULL;
        }
    }

    SongArray arr;
    rc = filter(db, &arr, (const char **) args);
    if (rc != 0) {
        fprintf(stderr, "筛选失败\n");
        return 0;
    }

    int active = 1;
    while (active) {
        printf(
            "筛选 - 操作 - 请输入指令"
            "(0: 回到上一层, 1: 查看信息, 2: 删除已选中歌曲): ");
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
            print_song_array(&arr);
            break;
        case 2:
            delete_songs(db, &arr);
            active = 0;
            break;
        default:
            printf("未知指令, 请重新输入\n");
        }
    }
    destruct_song_array(&arr);
    return 0;
}
