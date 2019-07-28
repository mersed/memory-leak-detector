#ifndef ARRAY_OF_STRUCTURES_H
#define ARRAY_OF_STRUCTURES_H

typedef struct _student {
    char stud_name[32];
    int rollno;
    int age;
} student_t;

void initialize_array_of_structures();

#endif
