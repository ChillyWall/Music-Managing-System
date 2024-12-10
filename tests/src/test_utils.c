#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

void test_create_tables(pdb db) {
    printf("Test: create_tables\n");
    int rc = create_tables(db);
    printf("result: %d\n", rc);
}

void test_add_and_delete_song(pdb db) {
    printf("Test: add_song\n");
    Song song = {
        "孤勇者", "孤勇者", "陈奕迅", "唐甜", "钱雷", "钱雷",
    };
    clear_all(db);

    SongArray songs;
    char **args = {NULL, NULL, NULL, NULL, NULL, NULL};
    int rc = add_song(db, &song);
    printf("result: %d\n", rc);
    filter(db, &songs, (const char **) args);
    print_song_array(&songs);

    song.lyricist = "唐恬";
    rc = add_song(db, &song);
    printf("readd result: %d\n", rc);
    printf("Test: delete_song\n");
    rc = delete_song(db, "孤勇者", "孤勇者");
    printf("result: %d\n", rc);
    filter(db, &songs, (const char **) args);
    print_song_array(&songs);
}

void test_search(pdb db) {
    printf("Test: search\n");
    Song song = {
        "孤勇者", "孤勇者", "陈奕迅", "唐恬", "钱雷", "Qian Lei",
    };
    create_tables(db);
    add_song(db, &song);
    Song res;
    int rc = search(db, &res, song.title, song.album);
    print_song_info(&res);
    destruct_song(&res);
    printf("result: %d\n", rc);
}

void test_read_song(pdb db) {
    printf("Test: read_song\n");
    create_tables(db);
    clear_all(db);
    Song song;
    read_song(&song);
    print_song_info(&song);
    add_song(db, &song);
    destruct_song(&song);
}

void test_filter(pdb db) {
    SongArray arr;
    Song song1 = {"song1",     "album1",    "singer1",
                  "lyricist1", "composer1", "arranger1"};
    Song song2 = {"song2",     "album1",    "singer1",
                  "lyricist1", "composer1", "arranger1"};
    Song song3 = {"song3",     "album2",    "singer1",
                  "lyricist2", "composer2", "arranger2"};
    Song song4 = {"song4",     "album2",    "singer1",
                  "lyricist1", "composer2", "arranger2"};
    Song song5 = {"song1",     "album3",    "singer2",
                  "lyricist2", "composer2", "arranger2"};
    create_tables(db);
    clear_all(db);
    add_song(db, &song1);
    add_song(db, &song2);
    add_song(db, &song3);
    add_song(db, &song4);
    add_song(db, &song5);

    filter(db, &arr, (const char *[]) {NULL, "album1", NULL, NULL, NULL, NULL});
    print_song_array(&arr);
    destruct_song_array(&arr);

    filter(db, &arr,
           (const char *[]) {NULL, NULL, "singer1", NULL, NULL, NULL});
    print_song_array(&arr);
    destruct_song_array(&arr);

    filter(db, &arr,
           (const char *[]) {NULL, NULL, NULL, "lyricist1", "composer1", NULL});
    print_song_array(&arr);
    printf("Delete these songs.\n");
    delete_songs(db, &arr);
    destruct_song_array(&arr);

    filter(db, &arr, (const char *[]) {NULL, NULL, NULL, NULL, NULL, NULL});
    print_song_array(&arr);
    destruct_song_array(&arr);
}

int main() {
    pdb db = NULL;
    int rc = connect_db(&db);

    if (rc != 0) {
        exit(-1);
    }

    test_search(db);
    test_search(db);
    test_add_and_delete_song(db);
    test_filter(db);

    close_db(db);
    return 0;
}
