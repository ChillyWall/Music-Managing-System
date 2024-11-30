#include "utils.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

pdb connect_db() {
    pdb db;
    int msg = sqlite3_open("../data.sqlite3", &db);
    if (msg) {
        printf("Error: Cannot open or create database.\n");
        exit(msg);
    } else {
        return db;
    }
}
