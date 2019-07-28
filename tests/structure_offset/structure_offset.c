#include "structure_offset.h"

void test_structure_offset() {
    printf("---------------------------\n");
    printf(
            "Offsets for \"struct emp_\":\n emp_name=%ld \n emp_id=%ld \n age=%ld \n mgr=%ld \n salary=%ld\n",
            (long) offsetof(emp_t, emp_name),
            (long) offsetof(emp_t, emp_id),
            (long) offsetof(emp_t, age),
            (long) offsetof(emp_t, mgr),
            (long) offsetof(emp_t, salary)
    );
    printf("---------------------------\n");
}