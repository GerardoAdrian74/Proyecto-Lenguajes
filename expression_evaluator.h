
// expression_evaluator.h
#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include <string>
#include <vector>

std::string infixToPostfix(const std::string& infix);
double evaluatePostfix(const std::string& postfix);
bool validateExpression(const std::string& expression);

#endif // EXPRESSION_EVALUATOR_H