#ifndef MEMORY_LEAK_DETECTOR_MLD_H

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

#define OFFSETOF(struct_name, field_name) \
    (unsigned int)&(((struct_name *)0)->field_name)

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
    field_info_t fields[0]; // Info of structure fields
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
void print_structure_db(struct_db_t *struct_db);
void print_structure_rec(struct_db_rec_t *struct_db_rec);

#endif //MEMORY_LEAK_DETECTOR_MLD_H