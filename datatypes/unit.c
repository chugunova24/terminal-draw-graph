#include "unit.h"

/*
 * PrintUnit - debug print unit data
 */

void PrintUnit(const Unit *u) {
    printf("Unit: ");
    if (u->type == VALUE) {
        printf("VALUE (%lf)\n", u->data.value);
    }
    if (u->type == ARG) printf("x\n");
    if (u->type == OPERATOR) {
        const char* ops[] = {
                "+",
                "-",
                "_",
                "*",
                "/",
                "sin",
                "cos",
                "tan",
                "ctg",
                "sqrt",
                "ln",
                "brOpen",
                "brClose",
        };
        printf("OPERATOR (%s, %d)\n", ops[u->data.opType], u->data.opType);
    }


}
