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

    static field_info_t field_info[] = {
        FIELD_INFO(employe_t, emp_name, CHAR, 0),
        FIELD_INFO(employe_t, emp_id, UINT32, 0),
        FIELD_INFO(employe_t, age, UINT32, 0),
        FIELD_INFO(employe_t, mgr, OBJ_PTR, employe_t),
        FIELD_INFO(employe_t, salary, FLOAT, 0)
    };

    REGISTER_STRUCTURE(struct_db, employe_t, field_info);

    print_structure_db(struct_db);

    return 0;
}