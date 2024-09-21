#include <string.h>
#include "parser.h"
#include "datatypes/list.h"
#include "datatypes/unit.h"

bool strToUnit(Unit* unit, char* expr, char** remainder);
int prec(FuncType type);

/*
 * prec - get precedence from type. Higher is more valuable
 */

int prec(FuncType type){
    switch (type){
        case OPENBRACKET:
        case CLOSEBRACKET:
            return 0;
        case SIN:
        case COS:
        case TAN:
        case CTG:
        case SQRT:
        case LN:
        case USUB:
            return 3;
        case MUL:
        case DIV:
            return 2;
        case ADD:
        case SUB:
            return 1;
    }
    return -1;
}

/* strToUnit - Convert expression part to Unit
 * Borrowed the idea from strtol. But unlike strtol, return value is just a status
 * Actual return value is in argument (Unit*).
 */
bool strToUnit(Unit* unit, char* expr, char** remainder){
    uint64_t exprLength = strlen(expr);
    // Check if complex function
    if (exprLength > 5 && !strncmp(expr, "sqrt", 4) && expr[4] == '('){
        unit->type = OPERATOR;
        unit->data.opType = SQRT;
        *remainder = expr+4;
        return True;
    }
    if (exprLength > 4 && expr[3] == '('){
        if (!strncmp(expr, "sin", 3)){
            unit->type = OPERATOR;
            unit->data.opType = SIN;
            *remainder = expr+3;
            return True;
        }
        if (!strncmp(expr, "cos", 3)){
            unit->type = OPERATOR;
            unit->data.opType = COS;
            *remainder = expr+3;
            return True;
        }
        if (!strncmp(expr, "tan", 3)){
            unit->type = OPERATOR;
            unit->data.opType = TAN;
            *remainder = expr+3;
            return True;
        }
        if (!strncmp(expr, "ctg", 3)){
            unit->type = OPERATOR;
            unit->data.opType = CTG;
            *remainder = expr+3;
            return True;
        }
    }
    if (exprLength > 3 && !strncmp(expr, "ln", 2) && expr[3] == '('){
        unit->type = OPERATOR;
        unit->data.opType = LN;
        *remainder = expr+2;
        return True;
    }
    // Maybe this is one of +-*/()?
    switch (*expr){
        case '+':
            unit->type = OPERATOR;
            unit->data.opType = ADD;
            *remainder = expr+1;
            return True;
        case '-':
            unit->type = OPERATOR;
            unit->data.opType = SUB;
            *remainder = expr+1;
            return True;
        case '*':
            unit->type = OPERATOR;
            unit->data.opType = MUL;
            *remainder = expr+1;
            return True;
        case '/':
            unit->type = OPERATOR;
            unit->data.opType = DIV;
            *remainder = expr+1;
            return True;
        case '(':
            unit->type = OPERATOR;
            unit->data.opType = OPENBRACKET;
            *remainder = expr+1;
            return True;
        case ')':
            unit->type = OPERATOR;
            unit->data.opType = CLOSEBRACKET;
            *remainder = expr+1;
            return True;
    }

    // Number?
    if (*expr >= '0' && *expr <= '9'){
        unit->type = VALUE;
        unit->data.value = strtod(expr, remainder);
        return True;
    }

    // Maybe it is an x?
    if (*expr == 'x'){
        unit->type = ARG;
        *remainder = expr + 1;
        return True;
    }

    //Unknown. Fail.
    return False;
}

#define PARSEFAIL ListFree(result); ListFree(stack); return 0;

/*
 * ParseExpression - convert infix expression to postfix
 * Returns List of Units if succeeded, otherwise 0.
 */


List* ParseExpression(char* expr){
    // Allocate result list
    List* result = ListAlloc(sizeof(Unit));
    if (!result) return 0;
    // And allocate secondary list (stack)
    List* stack = ListAlloc(sizeof(Unit));
    if (!stack){
        ListFree(result);
        return 0;
    }
    // This is used to understand if '-' is UNARY or BINARY operation
    UnitType lastUnitType = OPERATOR;
    FuncType lastFuncType = OPENBRACKET;
    const char* exprEnd = expr + strlen(expr);
    // Iterate over expression
    while (expr < exprEnd){
        Unit u;
        // strToUnit have similar arguments to strtol
        if (!strToUnit(&u, expr, &expr)) { PARSEFAIL }
        // Simplest case. Unit is either VALUE or ARG
        if (u.type == VALUE || u.type == ARG){
            // Just push it to the result stack
            if (!ListPush(result, &u)) { PARSEFAIL }
        }
        // Now if unit is an operator
        if (u.type == OPERATOR){
            // Check for unary sub. Sub can be unary if is starts an expression, or goes after operator.
            // Operator here includes opening bracket.
            if (u.data.opType == SUB && lastUnitType == OPERATOR && lastFuncType != CLOSEBRACKET) u.data.opType = USUB;

            if (u.data.opType == OPENBRACKET) {
                if (!ListPush(stack, &u)) { PARSEFAIL }
            } else if (u.data.opType == CLOSEBRACKET){
                while (!ListIsEmpty(stack)){
                    Unit utmp;
                    ListPop(stack, &utmp); //This can't fali
                    if (utmp.type == OPERATOR && utmp.data.opType == OPENBRACKET) break;
                    if (!ListPush(result, &utmp)) { PARSEFAIL }
                }
            } else {
                while (!ListIsEmpty(stack)){
                    Unit utmp;
                    ListPeek(stack, &utmp); //This can't fail
                    if (prec(u.data.opType) > prec(utmp.data.opType)) break;
                    ListPop(stack, &utmp); //This also can't fail
                    if (!ListPush(result, &utmp)) { PARSEFAIL }
                }
                if (!ListPush(stack, &u)) {PARSEFAIL}
            }
        }

        // Save last unit type and opType (if operator)
        lastUnitType = u.type;
        if (lastUnitType == OPERATOR)
            lastFuncType = u.data.opType;
    }

    //Move everything from stack to result
    while (!ListIsEmpty(stack)){
        Unit utmp;
        ListPop(stack, &utmp); //Also can't fail
        if (!ListPush(result, &utmp)) { PARSEFAIL }
    }
    //Cleaning up
    ListFree(stack);
    return result;
}


/*
 * PrintExpression - Use this to see converted expression.
 * Can't fail if used properly
 */
#ifdef DEBUG
void PrintExpression(List *val) {
    for (uint64_t i = 0; i < ListLength(val); i++){
        Unit u;
        ListGet(val, i, &u);
        if (u.type == VALUE) printf("%lf", u.data.value);
        else if (u.type == ARG) printf("x");
        else {
            if (u.data.opType > 12) printf("???");
            else {
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
                        "(",
                        ")",
                };
                printf("%s", ops[u.data.opType]);
            }
        }
        printf(" ");
    }
    printf("\n");

}
#endif


