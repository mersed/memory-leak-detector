#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
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
    add_object_to_object_db(object_db, ptr, units, struct_rec, MLD_FALSE);
    return ptr;
}


void add_object_to_object_db(
        object_db_t *object_db,
        void *ptr,
        int units,
        struct_db_rec_t *struct_rec,
        mld_boolean_t is_root
    ) {
    object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);
    /*Dont add same object twice*/
    assert(!obj_rec);

    object_db_rec_t *object_rec = calloc(1, sizeof(object_db_rec_t));
    object_rec->ptr = ptr;
    object_rec->units = units;
    object_rec->struct_rec = struct_rec;
    object_rec->is_visited = MLD_FALSE;
    object_rec->is_root = is_root;
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

    char *is_root = obj_rec->is_root ? "TRUE" : "FALSE";

    printf(ANSI_COLOR_MAGENTA"----------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf(ANSI_COLOR_YELLOW "%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s | is_root = %s\n",
           i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name, is_root);
    printf(ANSI_COLOR_MAGENTA "---------------------------------------------------------------------------------------------------------------------------------------------------|\n");
}


void print_object_db(object_db_t *object_db) {
    object_db_rec_t *head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN "Printing OBJECT DATABASE\n");
    for(; head; head = head->next){
        print_object_rec(head, i++);
    }
}


void mld_dump_object_rec_detail(object_db_rec_t *obj_rec) {
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




void mld_register_root_object(object_db_t *object_db, void *objptr, char *struct_name, unsigned int units) {
    struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
    assert(struct_rec);

    add_object_to_object_db(object_db, objptr, units, struct_rec, MLD_TRUE);
}

/**
 * The global object of the application which is not created by xcalloc
 * should be registered with MLD using bellow API
 */
void mld_set_dynamic_object_as_root(object_db_t *object_db, void *obj_ptr) {
    object_db_rec_t *obj_rec = object_db_look_up(object_db, obj_ptr);
    assert(obj_rec);

    obj_rec->is_visited = MLD_TRUE;
    obj_rec->is_root = MLD_TRUE;
}

void run_mld_algorithm(object_db_t *object_db) {
    // Step 1: Mark all objects in objects database as unvisited */
    init_mld_algorithm(object_db);

    /*
     * Step 2: Get the first root object from the object db, it could be
     * present anywhere in objectdb. If there are multiple roots in object db
     * return the first one, we can start mld algorithm from any root ojects
     */
    object_db_rec_t *root_obj = get_next_root_object(object_db, NULL);
    while(root_obj) {
        if(root_obj->is_visited) {
            /**
             * It means, all objects reachable from this root_obj has already been
             * explored, no need to do it again, else you will end up in infinite loop.
             * Remember, Application Data Structures are cyclic graphs.
             */
             root_obj = get_next_root_object(object_db, root_obj);
            continue;
        }

        // root objects are always reachable since application holds the global
        // variable to it
        root_obj->is_visited = MLD_TRUE;

        // Explore all reachable objects from this root_obj recursively
        mld_explore_objects_recursively(object_db, root_obj);

        root_obj = get_next_root_object(object_db, root_obj);
    }
}

static void init_mld_algorithm(object_db_t *object_db) {
    object_db_rec_t *obj_rec = object_db->head;

    while(obj_rec) {
        obj_rec->is_visited = MLD_FALSE;
        obj_rec = obj_rec->next;
    }
}


static object_db_rec_t* get_next_root_object(object_db_t *object_db, object_db_rec_t* root_obj) {
    object_db_rec_t *first = root_obj ? root_obj->next : object_db->head;
    while(first) {
        if(first->is_root) {
            return first;
        }
        first = first->next;
    }

    return NULL;
}

static void mld_explore_objects_recursively(object_db_t* object_db, object_db_rec_t *parent_obj) {
    unsigned int i, n_fields;
    char *parent_obj_ptr = NULL;
    char *child_obj_offset = NULL;
    void *child_obj_address = NULL;

    field_info_t *field_info = NULL;
    object_db_rec_t *child_object_rec = NULL;
    struct_db_rec_t *parent_struct_rec = parent_obj->struct_rec;

    // Parent object must have already visited
    assert(parent_obj->is_visited);


    for(i = 0; i<parent_obj->units; i++) {
        parent_obj_ptr = (char *)(parent_obj->ptr) + (i * parent_struct_rec->ds_size);

        for(n_fields=0; n_fields<parent_struct_rec->n_fields; n_fields++) {
            field_info = &parent_struct_rec->fields[n_fields];

            switch(field_info->dtype) {
                case UINT8:
                case UINT32:
                case INT32:
                case CHAR:
                case FLOAT:
                case DOUBLE:
                case OBJ_STRUCT:
                    break;
                case OBJ_PTR:
                default:
                    ;

                // child_obj_offset is the memory location inside parent object
                // here address of next level object is stored
                child_obj_offset = parent_obj_ptr + field_info->offset;
                memcpy(&child_obj_address, child_obj_offset, sizeof(void *));

                // child_object_address now stores the address of the nxt object in the
                // graph. It could be NULL, handle that as well
                if(!child_obj_address) continue;

                child_object_rec = object_db_look_up(object_db, child_obj_address);
                assert(child_object_rec);

                // Since we are able to reach this child object "child_object_rec
                // from parent object "parent_obj_ptr", mark this
                // child object as already visited, then do nothing - avoid infinite loops
                if(!child_object_rec->is_visited) {
                    child_object_rec->is_visited = MLD_TRUE;
                    mld_explore_objects_recursively(object_db, child_object_rec);
                }
            }
        }
    }
}

void report_leaked_objects(object_db_t *object_db) {
    int i = 0;
    object_db_rec_t *head;

    printf("Dumping Leaked Objects\n");

    for(head = object_db->head; head; head = head->next) {
        if(!head->is_visited) {
            print_object_rec(head, i++);
            mld_dump_object_rec_detail(head);
            printf("\n\n");
        }
    }
}