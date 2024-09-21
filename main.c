#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "datatypes/list.h"
#include "parser.h"
#include "evaluator.h"

// If you want to compute more accurately, you may change the WIDTH and HEIGHT here
#define WIDTH 80
#define HEIGHT 25
// And if you want to make graph line slightly more distinct, increase this (1 is minimum)
#define GRAPH_THICKNESS 1

void __attribute__((noreturn)) fail(const char* msg);
void DrawGraph(List* values);


void __attribute__((noreturn)) fail(const char* msg){
    fprintf(stderr, "%s\n", msg);
    exit(1);
}


void DrawGraph(List* values){
    int64_t* valueIndexes = calloc(sizeof(int64_t), ListLength(values));
    for (uint64_t i = 0; i < ListLength(values); i++){
        double value;
        ListGet(values, i, &value);
        valueIndexes[i] = (int64_t)round((value+1)*HEIGHT/2);
    }
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < ListLength(values); j++){
            printf("%s", (llabs(valueIndexes[j] - i) == (GRAPH_THICKNESS-1)) ? "*" : ".");
        }
        printf("\n");
    }
    free(valueIndexes);
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s \"expression\"\n", argv[0]);
        exit(1);
    }

    //Expression should be parsed into RPN
    List* expression = ParseExpression(argv[1]);
    if (!expression) fail("Something went wrong");
    List* values;
    if (!EvalExpressionEx(expression, &values, 0, 4*M_PI, 4*M_PI/WIDTH)) {
        ListFree(expression);
        fail("Something went wrong");
    }
    DrawGraph(values);
    ListFree(expression);
    ListFree(values);
}
