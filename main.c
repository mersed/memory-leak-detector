#include <stdio.h>
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


int main() {
    printf("TESTIRANJE");
    return 0;
}