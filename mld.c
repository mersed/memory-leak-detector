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
        printf("structure Number : %d (%p)\n", i++, struct_db_rec);
        print_structure_rec(struct_db_rec);
        struct_db_rec = struct_db_rec->next;
    }
}


int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec) {
    struct_db_rec_t *head = struct_db->head;

    if(!head){
        struct_db->head = struct_rec;
        struct_rec->next = NULL;
        struct_db->count++;
        return 0;
    }

    struct_rec->next = head;
    struct_db->head = struct_rec;
    struct_db->count++;
    return 0;
}

struct_db_rec_t* struct_db_look_up(struct_db_t *struct_db, char *struct_name) {
    if(!struct_db || !(struct_db->head))
        return NULL;

    struct_db_rec_t *next = struct_db->head;
    while(next) {
        if(strcmp(next->struct_name, struct_name) == 0) return next;
        next = next->next;
    }

    return NULL;
}

void xfree(object_db_t *object_db, void *ptr) {
    if(!ptr) return;

    object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);
    assert(obj_rec);
    assert(obj_rec->ptr);
    free(obj_rec->ptr);
    obj_rec->ptr = NULL;

    remove_object_from_object_db(object_db, obj_rec);
}

void remove_object_from_object_db(object_db_t *object_db, object_db_rec_t *obj_rec) {
    if(object_db->count == 1) {
        if(object_db->head == obj_rec) {
            free(object_db->head);
            object_db->head = NULL;
        }
        return;
    }

    object_db_rec_t *current = object_db->head->next;
    object_db_rec_t *prev = object_db->head;
    assert(current);

    while(current) {
        if(current == obj_rec) {
            prev->next = current->next;
            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }
}

void* xcalloc(object_db_t *object_db, char *struct_name, int units) {
    struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
    assert(struct_rec);

    void *ptr = calloc(units, struct_rec->ds_size);
    add_object_to_object_db(object_db, ptr, units, struct_rec);
    return ptr;
}

void add_object_to_object_db(object_db_t *object_db, void *ptr, int units, struct_db_rec_t *struct_rec) {
    object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);
    /*Dont add same object twice*/
    assert(!obj_rec);

    object_db_rec_t *object_rec = calloc(1, sizeof(object_db_rec_t));
    object_rec->ptr = ptr;
    object_rec->units = units;
    object_rec->struct_rec = struct_rec;
    object_rec->next = NULL;

    object_db_rec_t *head = object_db->head;
    if(!head) {
        object_db->head = object_rec;
        object_db->count++;
        return;
    }

    object_db->head = object_rec;
    object_rec->next = head;
    object_db->count++;
}

void* object_db_look_up(object_db_t *object_db, void *ptr) {
    object_db_rec_t *object_rec = object_db->head;
    if(!object_rec) return NULL;

    while(object_rec) {
        if(object_rec->ptr == ptr) {
            return object_rec;
        }

        object_rec = object_rec->next;
    }

    return NULL;
}

/*Dumping Functions for Object database*/
void print_object_rec(object_db_rec_t *obj_rec, int i) {
    if(!obj_rec) return;
    printf(ANSI_COLOR_MAGENTA"-----------------------------------------------------------------------------------|\n");
    printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s |\n",
           i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name);
    printf(ANSI_COLOR_MAGENTA "-----------------------------------------------------------------------------------|\n");
}

void print_object_db(object_db_t *object_db) {
    object_db_rec_t *head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN "Printing OBJECT DATABASE\n");
    for(; head; head = head->next){
        print_object_rec(head, i++);
    }
}

void mld_dump_object_rec_detail (object_db_rec_t *obj_rec) {
    struct_db_rec_t *struct_rec = obj_rec->struct_rec;

    for(int i=0; i<obj_rec->units; i++) {
        void *current_obj_ptr = obj_rec->ptr + i*struct_rec->ds_size;

        for(int j=0; j<struct_rec->n_fields; j++) {
            field_info_t field = struct_rec->fields[j];

            switch(field.dtype) {
                case UINT8:
                case UINT32:
                case INT32:
                    printf("%s[%d]->%s = %d\n", struct_rec->struct_name, i, field.fname, *(int *)(current_obj_ptr + field.offset));
                    break;
                case CHAR:
                    printf("%s[%d]->%s = %s\n", struct_rec->struct_name, i, field.fname, (char *)(current_obj_ptr + field.offset));
                    break;
                case FLOAT:
                    printf("%s[%d]->%s = %f\n", struct_rec->struct_name, i, field.fname, *(float *)(current_obj_ptr + field.offset));
                    break;
                case DOUBLE:
                    printf("%s[%d]->%s = %f\n", struct_rec->struct_name, i, field.fname, *(double *)(current_obj_ptr + field.offset));
                    break;
                case OBJ_PTR:
                    printf("%s[%d]->%s = %p\n", struct_rec->struct_name, i, field.fname, (current_obj_ptr + field.offset));
                    break;
                case OBJ_STRUCT:
                    // later
                    break;
                default:
                    break;

            }
        }
    }
}