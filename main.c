#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "mld.h"

typedef struct employe {
    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct employe *mgr;
    float salary;
} employe_t;


int main(int argc, char **argv) {
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));

    // Define structure for our mld
    struct_db_rec_t *emp_rec = calloc(1, STRUCT_DB_T_SIZE + 5*FIELD_INFO_T_SIZE);
    emp_rec->next = NULL;
    strncpy(emp_rec->struct_name, "emp_t", strlen("emp_t"));
    emp_rec->ds_size = sizeof(employe_t);
    emp_rec->n_fields = 5;

    // Define field 1
    strncpy(emp_rec->fields[0].fname, "emp_name", strlen("emp_name"));
    emp_rec->fields[0].dtype = CHAR;
    emp_rec->fields[0].size = 30;
    emp_rec->fields[0].offset = offsetof(employe_t, emp_name);
    strncpy(emp_rec->fields[0].nested_str_name, "nil", strlen("nil"));

    // Define field 2
    strncpy(emp_rec->fields[1].fname, "emp_id", strlen("emp_id"));
    emp_rec->fields[1].dtype = UINT32;
    emp_rec->fields[1].size = sizeof(unsigned int);
    emp_rec->fields[1].offset = offsetof(employe_t, emp_id);
    strncpy(emp_rec->fields[1].nested_str_name, "nil", strlen("nil"));

    // Define field 3
    strncpy(emp_rec->fields[2].fname, "age", strlen("age"));
    emp_rec->fields[2].dtype = UINT32;
    emp_rec->fields[2].size = sizeof(unsigned int);
    emp_rec->fields[2].offset = offsetof(employe_t, age);
    strncpy(emp_rec->fields[2].nested_str_name, "nil", strlen("nil"));

    // Define field 4
    strncpy(emp_rec->fields[3].fname, "mgr", strlen("mgr"));
    emp_rec->fields[3].dtype = OBJ_PTR;
    emp_rec->fields[3].size = sizeof(void *);
    emp_rec->fields[3].offset = offsetof(employe_t, mgr);
    strncpy(emp_rec->fields[3].nested_str_name, "employe_t", strlen("employe_t"));

    // Define field 5
    strncpy(emp_rec->fields[4].fname, "salary", strlen("salary"));
    emp_rec->fields[4].dtype = FLOAT;
    emp_rec->fields[4].size = sizeof(float);
    emp_rec->fields[4].offset = offsetof(employe_t, salary);
    strncpy(emp_rec->fields[4].nested_str_name, "nil", strlen("nil"));

    add_structure_to_struct_db(struct_db, emp_rec);
    print_structure_db(struct_db);

    return 0;
}