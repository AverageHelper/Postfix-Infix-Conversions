#include "ExpressionManager.h"

const string SYNTAX_ERROR = "invalid";

ExpressionManager::ExpressionManager() {
    // yup
}

/// Returns `true` if `character` is one of {,(, or [.
bool isOpenParen(const char character) {
    switch (character) {
        case '{':
        case '(':
        case '[':
            return true;
            
        default:
            return false;
    }
}

bool isOpenParen(const string str) {
    if (str.size() > 1) { return false; }
    return isOpenParen(str[0]);
}

/// Returns `true` if `character` is one of },), or ].
bool isCloseParen(const char character) {
    switch (character) {
        case '}':
        case ')':
        case ']':
            return true;
            
        default:
            return false;
    }
}

bool isCloseParen(const string str) {
    if (str.size() > 1) { return false; }
    return isCloseParen(str[0]);
}

/// Returns `true` if `character` is one of {,},(,),[, or ].
bool isParen(const char character) {
    return isOpenParen(character) || isCloseParen(character);
}

bool isParen(const string str) {
    if (str.size() > 1) { return false; }
    return isParen(str[0]);
}

/// Returns `true` if `open` and `close` form a parenthetic pair.
bool isPair(const char open, const char close) {
    if (!isParen(open) || !isParen(close)) {
        return false;
    }
    
    return  (open == '{' && close == '}') ||
            (open == '(' && close == ')') ||
            (open == '[' && close == ']');
}

bool isPair(const string open, const string close) {
    if (open.size() > 1 || close.size() > 1) { return false; }
    return isPair(open[0], close[0]);
}

/// Returns `true` if `character` is one of +, -, *, /, or %.
bool isOperator(const char character) {
    switch (character) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return true;
            
        default:
            return false;
    }
}

bool isOperator(const string str) {
    if (str.size() > 1) { return false; }
    return isOperator(str[0]);
}

bool isNumber(const string num) {
    
    for (int i = 0; i < num.size(); i++) {
        if (!isdigit(num[i])) {
            return false;
        }
    }
    
    return num.size() > 0;
}

int precedence(const string op) {
    if (isCloseParen(op)) {
        return 3;
        
    } else if (op == "*" ||
               op == "/" ||
               op == "%") {
        return 2;
        
    } else if (op == "+" ||
               op == "-") {
        return 1;
        
    } else if (isOpenParen(op)) {
        return 0;
    }
    
    return -1;
}

/// Returns the result of eveluating two operands with the given of +, -, *, /, or %.
string evaluateOperation(const int leftOpd, const int rightOpd, const char operatorChar) {
    if (operatorChar == '+') {
        return to_string(leftOpd + rightOpd);
        
    } else if (operatorChar == '-') {
        return to_string(leftOpd - rightOpd);
        
    } else if (operatorChar == '*') {
        return to_string(leftOpd * rightOpd);
        
    } else if (operatorChar == '/' && rightOpd != 0) {
        return to_string(leftOpd / rightOpd);
        
    } else if (operatorChar == '%') {
        return to_string(leftOpd % rightOpd);
    }
    
    return SYNTAX_ERROR;
}

vector<string> parseTokens(const string expression) {
    stringstream ss(expression);
    string token;
    vector<string> tokens;
    
    while (getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    
    return tokens;
}


bool ExpressionManager::isBalanced(string expression) {
    stack<char> expStack;
    bool isBalanced = true;
    int index = 0;
    
    while (isBalanced && index < expression.size()) {
        char thisChar = expression[index];
        
        if (isOpenParen(thisChar)) {
            expStack.push(thisChar);
            
        } else if (isCloseParen(thisChar)) {
            if (expStack.empty()) {
                isBalanced = false;
                break;
            }
            
            char topParen = expStack.top();
            expStack.pop();
            
            if (!isPair(topParen, thisChar)) {
                isBalanced = false;
            }
        }
        
        index += 1;
    }
    
    return isBalanced && expStack.empty();
}

string ExpressionManager::postfixEvaluate(string postfixExpression) {
    stack<int> operands;
    vector<string> tokens = parseTokens(postfixExpression);
    
//    cout << "Evaluating " << postfixExpression << endl;
    
    for (unsigned int i = 0; i < tokens.size(); i++) {
        string thisToken = tokens.at(i);
//        cout << "Token: " << thisToken << endl;
        
        if (isNumber(thisToken)) {
            // Push operand
            int digit = stoi(thisToken);
            operands.push(digit);
//            cout << "Is an operand." << endl;
            
        } else if (isOperator(thisToken) && operands.size() >= 2) {
            // Get left and right operands from stack
            int rightOpd = operands.top();
            operands.pop();
            int leftOpd = operands.top();
            operands.pop();
            
//            cout << "Is an operator. Operating..." << endl;
            
            // Evaluate
            string result = evaluateOperation(leftOpd, rightOpd, thisToken[0]);
            if (result == SYNTAX_ERROR) {
//                cout << "BAD SYNTAX" << endl << endl;
                return SYNTAX_ERROR;
            }
            
//            cout << "Got " << result << endl;
            
            operands.push(stoi(result));
            
        } else {
//            cout << "BAD SYNTAX" << endl << endl;
            return SYNTAX_ERROR;
        }
    }
    
    if (operands.empty()) {
//        cout << "Out of operands.\nBAD SYNTAX" << endl;
        return SYNTAX_ERROR;
    }
    
//    cout << "FINAL RESULT: " << operands.top() << endl << endl;
    return to_string(operands.top());
}

string ExpressionManager::postfixToInfix(string postfixExpression) {
    stack<string> expressions;
    vector<string> tokens = parseTokens(postfixExpression);
    
//    cout << "Converting " << postfixExpression << " to infix." << endl;
    
    for (unsigned int i = 0; i < tokens.size(); i++) {
        string token = tokens.at(i);
//        cout << "Token: " << token << endl;
        
        if (isNumber(token)) {
            expressions.push(token);
//            cout << "Is a number." << endl;
            
        } else if (isOperator(token) && expressions.size() >= 2) {
            string rightExp = expressions.top();
            expressions.pop();
            string leftExp = expressions.top();
            expressions.pop();
            
//            cout << "Is an operator. Concatinating: ( ";
//            cout << leftExp << " " << token << " " << rightExp << " )" << endl;
            
            string result = "( " + leftExp + " " + token + " " + rightExp + " )";
            expressions.push(result);
            
        } else {
//            cout << "BAD SYNTAX" << endl << endl;
            return SYNTAX_ERROR;
        }
    }
    
    if (expressions.size() == 1) {
//        cout << "FINAL RESULT: " << expressions.top() << endl << endl;
        return expressions.top();
    }
    
//    cout << "BAD SYNTAX" << endl << endl;
    return SYNTAX_ERROR;
}

bool process_operator(stack<string> &opStack, string &postfix, const string op) {
    if (opStack.empty() || isOpenParen(opStack.top()) || isOpenParen(op)) {
        opStack.push(op);
        return true;
        
    } else if (isCloseParen(op)) {
        while (!isPair(opStack.top(), op)) {
            postfix += opStack.top() + " ";
            opStack.pop();
            
            if (opStack.empty()) {
                return false;
            }
        }
        
        // This will be a matching opening parenthesis
        opStack.pop();
        return true;
        
    } else {
        while (precedence(op) <= precedence(opStack.top())) {
            postfix += opStack.top() + " ";
            opStack.pop();
            
            if (opStack.empty()) {
                break;
            }
        }
        
        opStack.push(op);
        return true;
    }
}

string ExpressionManager::infixToPostfix(string infixExpression) {
    string postfix = "";
    stack<string> operators;
    vector<string> tokens = parseTokens(infixExpression);
    
//    cout << "Converting " << infixExpression << " to postfix..." << endl;
    
    for (unsigned int i = 0; i < tokens.size(); i++) {
        string token = tokens.at(i);
//        cout << "Token: " << token << endl;
        
        if (isNumber(token)) {
            postfix += token + " ";
//            cout << "Is a number. Postfix now '" << postfix << "'" << endl;
            
        } else if (isOperator(token) || isParen(token)) {
//            cout << "Is an operator or paren.";
            if (process_operator(operators, postfix, token) == false) {
//                cout << endl << "BAD SYNTAX" << endl << endl;
                return SYNTAX_ERROR;
            }
            
//            cout << " Postfix now '" << postfix << "'" << endl;
            
        } else {
//            cout << "BAD SYNTAX" << endl << endl;
            return SYNTAX_ERROR;
        }
    }
    
    int remainingOpCount = static_cast<int>(operators.size());
//    cout << "Popping " << remainingOpCount << " remaining operators." << endl;
    
    for (int i = 0; i < remainingOpCount; i++) {
        postfix += operators.top() + " ";
        operators.pop();
        
        if (operators.empty()) {
            postfix.pop_back();
        }
    }
    
    // Remove trailing space
    if (isspace(postfix[postfix.size() - 1])) {
        postfix.pop_back();
    }
    
    string evalResult = postfixEvaluate(postfix);
//    cout << "Evaluating result..." << endl;
    if (evalResult == SYNTAX_ERROR) {
//        cout << "BAD SYNTAX" << endl << endl;
        return SYNTAX_ERROR;
    }
    
//    cout << "Got " << evalResult << endl;
//    cout << "FINAL POSTFIX: " << postfix << endl << endl;
    return postfix;
}
