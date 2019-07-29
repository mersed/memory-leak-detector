#include <stdio.h>
#include <stdlib.h>
#include "mld.h"

const unsigned  int FIELD_INFO_T_SIZE = sizeof(field_info_t);
const unsigned int STRUCT_DB_T_SIZE = sizeof(struct_db_t);

void print_structure_db(struct_db_t *struct_db) {
    if(!struct_db) return;

    printf("Printing structure db\n");
    int i = 0;
    struct_db_rec_t *struct_db_rec = struct_db->head;
    printf("Number of Structures Registered = %d\n", struct_db->count);

    while(struct_db_rec) {
        printf("structure Number : %d (%p)\n", i, struct_db_rec);
        //print_structure_rec(struct_rec);
        struct_db_rec = struct_db_rec->next;
    }
}


int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_db_rec) {
    struct_db_rec_t *head = struct_db->head;

    if(!head) {
        struct_db->head = struct_db_rec;
        struct_db_rec->next = NULL;
        struct_db->count++;
        return 0;
    }

    return 0;
}