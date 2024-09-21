#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include "datatypes/list.h"

List* ParseExpression(char* expr);

#ifdef DEBUG
void PrintExpression(List* val);
#endif




#endif //GRAPH_PARSER_H
