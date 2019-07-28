#ifndef STRUCTURE_OFFSET
#define STRUCTURE_OFFSET

#include <stdio.h>
#include <string.h>
#include <stddef.h>

typedef struct emp_ {
    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
} emp_t;

void test_structure_offset();

#endif