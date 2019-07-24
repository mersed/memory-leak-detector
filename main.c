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


typedef struct _student {
    char stud_name[32];
    int rollno;
    int age;
} student_t;

int main() {
    printf(
            "Offsets for \"struct emp_\":\n emp_name=%ld \n emp_id=%ld \n age=%ld \n mgr=%ld \n salary=%ld\n",
            (long) offsetof(emp_t, emp_name),
            (long) offsetof(emp_t, emp_id),
            (long) offsetof(emp_t, age),
            (long) offsetof(emp_t, mgr),
            (long) offsetof(emp_t, salary)
    );

    printf("---------------------------\n");
    student_t students[50] = {{.stud_name = "John", .rollno = 1, .age = 19}};

    return 0;
}