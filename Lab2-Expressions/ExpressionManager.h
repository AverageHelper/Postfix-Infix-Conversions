#ifndef ExpressionManager_h
#define ExpressionManager_h

#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include "ExpressionManagerInterface.h"

class ExpressionManager : public ExpressionManagerInterface {
public:
    ExpressionManager();
    bool isBalanced(string expression);
    string postfixEvaluate(string postfixExpression);
    string postfixToInfix(string postfixExpression);
    string infixToPostfix(string infixExpression);
};

#endif /* ExpressionManager_h */
