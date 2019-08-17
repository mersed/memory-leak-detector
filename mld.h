#ifndef MEMORY_LEAK_DETECTOR_MLD_H

#include <stdint.h>
#include <assert.h>

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

#define FIELD_SIZE(struct_name, field_name) \
    sizeof(((struct_name *)0)->field_name)

#define OFFSETOF(struct_name, field_name) \
    (uintptr_t)&(((struct_name *)0)->field_name)

const unsigned int FIELD_INFO_T_SIZE;
const unsigned int STRUCT_DB_T_SIZE;

/**
 * Enumeration for data types
 */
typedef enum {
    UINT8,
    UINT32,
    INT32,
    CHAR,
    OBJ_PTR,
    FLOAT,
    DOUBLE,
    OBJ_STRUCT
} data_type_t;

typedef enum {
    MLD_FALSE,
    MLD_TRUE
} mld_boolean_t;

/**
 * Structure to store the information of one field of a
 * C structure
 */
typedef struct field_info {
    char fname[MAX_FIELD_NAME_SIZE]; // Name of the field
    data_type_t dtype;               // Data type of the field
    unsigned int size;               // Field size
    unsigned int offset;             // Offset of the field
    char nested_str_name[MAX_STRUCTURE_NAME_SIZE];  // Field meaningful only if dtype = OBJ_PTR, Or OBJ_STRUCT
} field_info_t;

/**
 * Structure which will represent one node in the
 * linked list for structure storage
 */
typedef struct struct_db_rec {
    struct struct_db_rec *next; // Pointer to the next structure in the linked list
    char struct_name[MAX_STRUCTURE_NAME_SIZE]; // key
    unsigned int ds_size;   // Size of the structure
    unsigned int n_fields;  // No of fields in the structure
    field_info_t *fields; // Info of structure fields
} struct_db_rec_t;

/**
 * This structure represent beginning of the
 * linked list. It is the head of the list.
 */
typedef struct struct_db {
    struct_db_rec_t *head; // Start of linked list
    unsigned int count;           // Total number of elements in linked list
} struct_db_t;


int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_db_rec);
struct_db_rec_t* struct_db_look_up(struct_db_t *struct_db, char *struct_name);
void print_structure_db(struct_db_t *struct_db);
void print_structure_rec(struct_db_rec_t *struct_db_rec);

#define FIELD_INFO(struct_name, fl_name, fl_data_type, nested_struct_name)    \
    {#fl_name, fl_data_type, FIELD_SIZE(struct_name, fl_name), OFFSETOF(struct_name, fl_name), #nested_struct_name}

#define REGISTER_STRUCTURE(struct_db_t, st_name, fields_array)                          \
    do {                                                                                \
        struct_db_rec_t *record = calloc(1, STRUCT_DB_T_SIZE + 5*FIELD_INFO_T_SIZE);    \
        strncpy(record->struct_name, #st_name, MAX_STRUCTURE_NAME_SIZE);                \
        record->next = NULL;                                                            \
        record->ds_size = sizeof(st_name);                                              \
        record->n_fields = sizeof(fields_array) / sizeof(field_info_t);                 \
        record->fields = fields_array;                                                  \
        if(add_structure_to_struct_db(struct_db_t, record)) {                           \
            assert(0);                                                                  \
        }                                                                               \
    }                                                                                   \
    while(0);











/** OBJECT DATABASE STRUCTURE IMPLEMENTATION STARTS HERE */
typedef struct object_db_rec {
    struct object_db_rec *next;
    void *ptr;
    unsigned int units;
    struct_db_rec_t *struct_rec;
    mld_boolean_t is_visited;   // Used for Graph traversal
    mld_boolean_t is_root;      // Is this object root object
} object_db_rec_t;

typedef struct object_db {
    struct_db_t *struct_db;
    object_db_rec_t *head;
    unsigned int count;
} object_db_t;


void print_object_rec(object_db_rec_t *obj_rec, int i);
void* xcalloc(object_db_t *object_db, char *struct_name, int units);
void xfree(object_db_t *object_db, void *ptr);
void remove_object_from_object_db(object_db_t *object_db, object_db_rec_t *object_db_rec);
void add_object_to_object_db(object_db_t*, void*, int, struct_db_rec_t*, mld_boolean_t);
void* object_db_look_up(object_db_t *object_db, void *ptr);
void print_object_db(object_db_t *object_db);
void mld_dump_object_rec_detail (object_db_rec_t *obj_rec);
void mld_register_root_object(object_db_t *object_db, void *objptr, char *struct_name, unsigned int units);
void mld_set_dynamic_object_as_root(object_db_t*, void*);

void run_mld_algorithm(object_db_t*);
static void init_mld_algorithm(object_db_t*);
static object_db_rec_t* get_next_root_object(object_db_t*, object_db_rec_t*);
static void mld_explore_objects_recursively(object_db_t*, object_db_rec_t*);
void report_leaked_objects(object_db_t*);

#endif //MEMORY_LEAK_DETECTOR_MLD_H