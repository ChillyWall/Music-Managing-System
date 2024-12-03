#include <stdio.h>
#include <utils.h>

void test_create_tables(pdb db) {
    create_tables(db);
}

int main() {
    pdb db = connect_db();
    test_create_tables(db);
    return 0;
}
