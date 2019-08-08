#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mld.h"
#include "css.h"

const unsigned  int FIELD_INFO_T_SIZE = sizeof(field_info_t);
const unsigned int STRUCT_DB_T_SIZE = sizeof(struct_db_t);

char *DATA_TYPE[] = {
        "UINT8", "UINT32", "INT32",
        "CHAR", "OBJ_PTR", "FLOAT",
        "DOUBLE", "OBJ_STRUCT"
};

void print_structure_rec(struct_db_rec_t *struct_db_rec) {
    if(!struct_db_rec) return;
    int j = 0;
    field_info_t *field = NULL;
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, struct_db_rec->struct_name, struct_db_rec->ds_size, struct_db_rec->n_fields);
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    for(j = 0; j < struct_db_rec->n_fields; j++) {
        field = &struct_db_rec->fields[j];
        printf("  %-20s |", "");
        printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n",
               j, field->fname, DATA_TYPE[field->dtype], field->size, field->offset, field->nested_str_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_CYAN "--------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}


void print_structure_db(struct_db_t *struct_db) {
    if(!struct_db) return;

    printf("Printing structure db\n");
    int i = 0;
    struct_db_rec_t *struct_db_rec = struct_db->head;
    printf("Number of Structures Registered = %d\n", struct_db->count);

    while(struct_db_rec) {
        printf("structure Number : %d (%p)\n", i, struct_db_rec);
        print_structure_rec(struct_db_rec);
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