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

    return 0;
}