#ifndef GRAPH_EVALUATOR_H
#define GRAPH_EVALUATOR_H

#include "datatypes/list.h"

bool EvalExpression(List* expression, double arg, double* result);
bool EvalExpressionEx(List* expression, List** results, double from, double to, double step);


#endif //GRAPH_EVALUATOR_H
