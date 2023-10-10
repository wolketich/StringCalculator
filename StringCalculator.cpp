/**
 * @file StringCalculator.cpp
 * @brief Mathematical Expression Calculator
 * 
 * This program evaluates mathematical expressions provided by the user.
 * It supports basic arithmetic operations, as well as trigonometric and exponential functions.
 * 
 * Universitatea Tehnica a Moldovei
 * Lab Work No. 1 - Computer Programming. Basics of C++ Language.
 * Date: 18.09.2021
 * Author: Vladislav Cernega
 * Reviewed by: Dumitru Prijilevschi
 */

#include <iostream>
#include <stack>
#include <cmath>

using namespace std;

// Constants
const double PI = acos(-1.0);

// Function to round a double to 7 decimal places
double roundTo7Decimal(double value) {
    return round(value * 1e7) / 1e7;
}

// Struct to represent a mathematical digit or operation
struct Digit {
    char type;    // '0' for digits, '+-/*^' for operations, and 'sctgea' for functions
    double value; // Value of the digit or result of the operation
};

// Function to execute mathematical operations
bool executeMathOperation(stack<Digit>& numbers, stack<Digit>& operations, Digit& item);

// Function to get the priority of a mathematical operation
int getOperationPriority(char operation);

int main() {
    cout << "For pi, enter 'p'. For the number e, use 'exp(1)'\n";
    cout << "Enter the expression: ";

    stack<Digit> numbers;      // Stack to store numbers
    stack<Digit> operations;   // Stack to store operations
    Digit item;
    char currentChar;
    bool isNegativeAtStart = true;

    while (true) {
        currentChar = cin.peek();

        if (currentChar == '\n') break;
        if (currentChar == ' ') {
            cin.ignore();
            continue;
        }

        // Handle mathematical functions like sin, cos, tan, etc.
        if (strchr("sctgea", currentChar)) {
            string function;
            cin >> function;

            if (function == "sin") {
                item.type = 's';
            } else if (function == "cos") {
                item.type = 'c';
            } else if (function == "tan") {
                item.type = 't';
            } else if (function == "ctg") {
                item.type = 'g';
            } else if (function == "exp") {
                item.type = 'e';
            } else if (function == "abs") {
                item.type = 'a';
            } else {
                cout << "\nInvalid function entered!\n";
                return 0;
            }

            item.value = 0;
            operations.push(item);
            isNegativeAtStart = true;
            continue;
        }

        // Handle numbers and the constant pi
        if (isdigit(currentChar) || (currentChar == '-' && isNegativeAtStart)) {
            if (currentChar == 'p') {
                cin.ignore();
                numbers.push({ '0', PI });
            } else {
                double value;
                cin >> value;
                numbers.push({ '0', value });
            }
            isNegativeAtStart = false;
            continue;
        }

        // Handle mathematical operations
        if (strchr("+-*/^", currentChar)) {
            while (!operations.empty() && getOperationPriority(currentChar) <= getOperationPriority(operations.top().type)) {
                if (!executeMathOperation(numbers, operations, item)) {
                    return 0;
                }
            }
            operations.push({ currentChar, 0 });
            cin.ignore();
            isNegativeAtStart = (currentChar == '-');
            continue;
        }

        // Handle parentheses
        if (currentChar == '(') {
            operations.push({ '(', 0 });
            cin.ignore();
            isNegativeAtStart = true;
            continue;
        }

        if (currentChar == ')') {
            while (!operations.empty() && operations.top().type != '(') {
                if (!executeMathOperation(numbers, operations, item)) {
                    return 0;
                }
            }
            if (!operations.empty()) operations.pop();
            cin.ignore();
            continue;
        }

        cout << "\nInvalid expression entered!\n";
        return 0;
    }

    while (!operations.empty()) {
        if (!executeMathOperation(numbers, operations, item)) {
            return 0;
        }
    }

    cout << "Result: " << numbers.top().value << endl;
    return 0;
}

bool executeMathOperation(stack<Digit>& numbers, stack<Digit>& operations, Digit& item) {
    if (numbers.empty() || operations.empty()) return false;

    double a = numbers.top().value;
    numbers.pop();

    char operation = operations.top().type;
    operations.pop();

    double result;

    switch (operation) {
        case '+':
            if (numbers.empty()) return false;
            result = numbers.top().value + a;
            numbers.pop();
            break;
        case '-':
            if (numbers.empty()) return false;
            result = numbers.top().value - a;
            numbers.pop();
            break;
        case '*':
            if (numbers.empty()) return false;
            result = numbers.top().value * a;
            numbers.pop();
            break;
        case '/':
            if (a == 0) {
                cerr << "\nError! Division by zero.\n";
                return false;
            }
            if (numbers.empty()) return false;
            result = numbers.top().value / a;
            numbers.pop();
            break;
        case '^':
            if (numbers.empty()) return false;
            result = pow(numbers.top().value, a);
            numbers.pop();
            break;
        case 's':
            result = roundTo7Decimal(sin(a));
            break;
        case 'c':
            result = roundTo7Decimal(cos(a));
            break;
        case 't':
            if (roundTo7Decimal(cos(a)) == 0) {
                cerr << "\nInvalid argument for tangent function!\n";
                return false;
            }
            result = roundTo7Decimal(tan(a));
            break;
        case 'g':
            if (roundTo7Decimal(sin(a)) == 0) {
                cerr << "\nInvalid argument for cotangent function!\n";
                return false;
            }
            result = roundTo7Decimal(1.0 / tan(a));
            break;
        case 'e':
            result = exp(a);
            break;
        case 'a':
            result = abs(a);
            break;
        default:
            cerr << "\nError in operation!\n";
            return false;
    }

    numbers.push({ '0', result });
    return true;
}

int getOperationPriority(char operation) {
    switch (operation) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        case 's':
        case 'c':
        case 't':
        case 'g':
        case 'e':
        case 'a':
            return 4;
        default:
            return 0;
    }
}
