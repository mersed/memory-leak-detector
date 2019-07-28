#include "array_of_structures.h"

void initialize_array_of_structures() {
    student_t students[2] = {
            {.stud_name = "John", .rollno = 1, .age = 19},
            {.stud_name = "Maria", .rollno = 2, .age = 20},
            {.stud_name = "Dan", .rollno = 3, .age = 21}
    };
}