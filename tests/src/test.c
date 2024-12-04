#include <stdio.h>
#include <utils.h>

void test_create_tables(pdb db) {
    create_tables(db);
}

void test_add_song(pdb db) {
    add_song("song1", "artist1", );
}

int main() {
    pdb db = connect_db();
    test_create_tables(db);
    return 0;
}
