# Mathematical Expression Calculator

## Description
This repository contains a program that evaluates mathematical expressions. It's designed to support basic arithmetic operations, as well as trigonometric and exponential functions. The program is developed as a part of the coursework for "Programarea Calculatoarelor. Bazele limbajului C." at Universitatea Tehnica a Moldovei.

## Features
- Basic arithmetic operations: addition, subtraction, multiplication, division, and exponentiation.
- Trigonometric functions: sine, cosine, tangent, and cotangent.
- Exponential function and absolute value computation.
- Proper error handling for invalid expressions and mathematical errors (e.g., division by zero).

## Technical Overview and Logic

### Data Structures
The program primarily uses two stacks:
1. `numbers`: To store the numerical values parsed from the input.
2. `operations`: To store the operators and functions.

### Parsing and Evaluation
The program reads the input character by character:
- Numbers and constants (like `pi`) are pushed onto the `numbers` stack.
- Operators and functions are pushed onto the `operations` stack, but not before evaluating any higher-priority operations already on the stack.

### Operator Precedence
The program respects the standard operator precedence. For instance, multiplication and division are evaluated before addition and subtraction. This is achieved by the `getOperationPriority` function, which assigns a priority level to each operator.

### Evaluation Mechanism
When an operator needs to be evaluated:
1. The required number of operands are popped from the `numbers` stack.
2. The operation is performed.
3. The result is pushed back onto the `numbers` stack.

For functions like `sin` or `cos`, the program reads the next three characters to determine the specific function and then processes the value accordingly.

### Error Handling
The program checks for common errors like division by zero or invalid arguments for certain functions (e.g., tangent of 90 degrees). In such cases, an error message is displayed to the user.

### Parentheses Handling
The program supports the use of parentheses to override the default operator precedence. When an opening parenthesis is encountered, it's pushed onto the `operations` stack. When a closing parenthesis is found, operations are popped and evaluated until the corresponding opening parenthesis is encountered.

## Usage
1. Compile the program using a C++ compiler.
2. Run the executable.
3. Enter the mathematical expression you want to evaluate.
4. The program will display the result or an error message if the expression is invalid.

## Author
Cernega Vladislav

## Acknowledgements
Special thanks to Dumitru Prijilevschi for reviewing the code.

## License
This project is open-source and available under the MIT License.
