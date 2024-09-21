#ifndef GRAPH_UNIT_H
#define GRAPH_UNIT_H
#include "types.h"

/*
 * Unit type. It can be one of 3
 */
typedef enum UnitType{
    VALUE, //double value (for ex. 3.5)
    OPERATOR, //operator (may be simple operator (+-*/), or be complex (sin, cos))
    ARG, //argument. Typically x
} UnitType;

/*
 * There is the list of supported operators
 * Notice that in actuality, complex functions are just unary operators.
 */
typedef enum FuncType {
    ADD,
    SUB,
    USUB,
    MUL,
    DIV,
    SIN,
    COS,
    TAN,
    CTG,
    SQRT,
    LN,
    OPENBRACKET,
    CLOSEBRACKET,
} FuncType;

/*
 * Unit type. It is used to be stored in one list.
 */
typedef struct Unit {
    UnitType type;
    union {
        double value;
        FuncType opType;
    } data;
} Unit;

void PrintUnit(const Unit* u);


#endif //GRAPH_UNIT_H
