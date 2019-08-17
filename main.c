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

typedef struct student {
    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct student *best_colleage;
} student_t;


int main(int argc, char **argv) {
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));

    static field_info_t emp_fields[] = {
        FIELD_INFO(employe_t, emp_name, CHAR, 0),
        FIELD_INFO(employe_t, emp_id, UINT32, 0),
        FIELD_INFO(employe_t, age, UINT32, 0),
        FIELD_INFO(employe_t, mgr, OBJ_PTR, employe_t),
        FIELD_INFO(employe_t, salary, FLOAT, 0)
    };
    REGISTER_STRUCTURE(struct_db, employe_t, emp_fields);

    static field_info_t stud_fields[] = {
        FIELD_INFO(student_t, stud_name, CHAR, 0),
        FIELD_INFO(student_t, rollno, UINT32, 0),
        FIELD_INFO(student_t, age, UINT32, 0),
        FIELD_INFO(student_t, aggregate, FLOAT, 0),
        FIELD_INFO(student_t, best_colleage, OBJ_PTR, student_t)
    };
    REGISTER_STRUCTURE(struct_db, student_t, stud_fields)

    print_structure_db(struct_db);

    // Working with object database
    // Step 1 : Initialize a new Object database
    object_db_t *object_db = calloc(1, sizeof(object_db_t));
    object_db->struct_db = struct_db;

    // Step 2 : Create some sample objects, equivalent to standard
    // calloc(1, sizeof(student_t))
    student_t *john = xcalloc(object_db, "student_t", 1);
    mld_set_dynamic_object_as_root(object_db, john);

    student_t *david = xcalloc(object_db, "student_t", 1);
    strncpy(david->stud_name, "mersed", strlen("mersed"));

    employe_t *joseph = xcalloc(object_db, "employe_t", 2);
    print_object_db(object_db);

    run_mld_algorithm(object_db);
    report_leaked_objects(object_db);

    return 0;
}