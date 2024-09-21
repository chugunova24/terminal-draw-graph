#include "evaluator.h"
#include "datatypes/unit.h"
#include <math.h>
#define EVAL_FAIL ListFree(stack); return False;

/*
 * EvalExpressionEx - bulk eval expression. Usability function.
 * Computes values for range(from, to, step)
 * Allocates it's output
 */
bool EvalExpressionEx(List* expression, List** results, double from, double to, double step){
    *results = ListAlloc(sizeof(double));
    if (!*results) return False;
    double arg = from;
    while (arg < to){
        double result;
        if (!EvalExpression(expression, arg, &result)){
            ListFree(*results);
            return False;
        }
        if (!ListPush(*results, &result)){
            ListFree(*results);
            return False;
        }
        arg += step;
    }
    return True;
}

/*
 * EvalExpression - evaluate an expression for x = arg. Store in result
 * Since expression is in postfix notation. This is just a stack machine
 * Store values on stack, then, when operation is met, pop from stack values to compute and put on the stack again
 */
bool EvalExpression(List *expression, double arg, double* result) {
    List* stack = ListAlloc(sizeof(double));
    for (uint64_t i = 0; i < ListLength(expression); i++){
        Unit u;
        ListGet(expression, i, &u);
        if (u.type == VALUE) {
            if (!ListPush(stack, &u.data.value)) { EVAL_FAIL }
        } else if (u.type == ARG) {
            if (!ListPush(stack, &arg)) { EVAL_FAIL }
        } else if (u.type == OPERATOR) {
            double opResult;
            double arg1, arg2;
            // arg1 always presents
            if (!ListPop(stack, &arg1)) { EVAL_FAIL }
            // arg2 in +-/*
            switch (u.data.opType){
                case ADD:
                case SUB:
                case DIV:
                case MUL:
                    if (!ListPop(stack, &arg2)) { EVAL_FAIL }
                    break;
                default:
                    break;
            }
            // evaluation
            switch (u.data.opType) {
                case ADD:
                    opResult = arg1 + arg2;
                    break;
                case SUB:
                    opResult = arg2 - arg1;
                    break;
                case DIV:
                    opResult = arg2 / arg1;
                    break;
                case MUL:
                    opResult = arg1 * arg2;
                    break;
                case USUB:
                    opResult = -arg1;
                    break;
                case SIN:
                    opResult = sin(arg1);
                    break;
                case COS:
                    opResult = cos(arg1);
                    break;
                case TAN:
                    opResult = tan(arg1);
                    break;
                case CTG:
                    opResult = 1/tan(arg1);
                    break;
                case SQRT:
                    opResult = sqrt(arg1);
                    break;
                case LN:
                    opResult = log(arg1);
                    break;
                default:
                    EVAL_FAIL
            }
            if (!ListPush(stack, &opResult)) { EVAL_FAIL }
        }
    }

    /*
     * This check is very important, because the expression is correct ONLY is after computation there is only 1 element left on the stack
     * Otherwise we must fail
     */
    if (ListLength(stack) != 1) { EVAL_FAIL }
    ListPop(stack, result);
    ListFree(stack);
    return True;
}
