# String Calculator

## Overview

The **String Calculator** is a robust C++ program designed to evaluate mathematical expressions entered as a string by the user. It handles a wide variety of arithmetic operations, trigonometric functions, and parentheses with operator precedence. This program demonstrates advanced programming concepts such as tokenization, operator precedence parsing, stack-based evaluation, and function handling.

---

## Features

- **Basic Arithmetic Operations**: Supports addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), and exponentiation (`^`).
- **Trigonometric Functions**: Includes support for functions such as `sin`, `cos`, and `tan`.
- **Additional Mathematical Functions**:
  - `sqrt`: Square root
  - `log`: Natural logarithm
  - `abs`: Absolute value
- **Constants**:
  - `PI` (`3.14159...`)
  - `E` (`2.71828...`)
- **Parentheses**: Supports nested parentheses to enforce specific evaluation orders.
- **Operator Precedence**: Correctly evaluates operations according to standard precedence rules (e.g., multiplication and division before addition and subtraction).
- **Error Handling**: Comprehensive error handling for invalid tokens, mismatched parentheses, division by zero, and domain errors.

---

## How It Works

1. **Tokenization**:
   - The input string is first broken into tokens (numbers, operators, parentheses, and functions).
   - Example: The input `2+2*2` is tokenized as:
     ```
     Token 1: Number (2)
     Token 2: Operator (+)
     Token 3: Number (2)
     Token 4: Operator (*)
     Token 5: Number (2)
     ```

2. **Shunting-Yard Algorithm**:
   - Operators and functions are pushed onto a stack, respecting precedence and associativity rules.
   - Parentheses are handled to allow nested expressions.

3. **Evaluation**:
   - The program uses two stacks:
     - A **values stack** to store numbers.
     - An **operators stack** to store operators and functions.
   - Operators are applied to operands as they are popped from the stacks, and the result is pushed back onto the values stack.

4. **Output**:
   - After all tokens are processed, the result is printed to the console.

---

## Usage

### Compilation

To compile the program, ensure you have a C++20-compliant compiler such as `g++`. Use the following command:

```bash
g++ -std=c++20 StringCalculator.cpp -o StringCalculator
```

### Execution

Run the compiled program:

```bash
./StringCalculator
```

The program will prompt you to enter a mathematical expression. For example:

```plaintext
Enter a mathematical expression:
2+2*2
Result: 6.0000000
```

### Input Format

- Numbers: Enter numbers using standard decimal notation (e.g., `2`, `3.14`).
- Operators: Use the following symbols:
  - `+` for addition
  - `-` for subtraction
  - `*` for multiplication
  - `/` for division
  - `^` for exponentiation
- Functions: Enter supported functions with parentheses. Examples:
  - `sin(PI/2)`
  - `sqrt(16)`
  - `log(E)`
- Parentheses: Use `(` and `)` to group expressions.

---

## Example Inputs and Outputs

### Basic Arithmetic
| Expression  | Expected Output |
|-------------|-----------------|
| `2+2*2`     | `6.0000000`     |
| `(2+2)*2`   | `8.0000000`     |
| `2^3+5`     | `13.0000000`    |

### Trigonometric Functions
| Expression         | Expected Output |
|--------------------|-----------------|
| `sin(PI/2)`        | `1.0000000`     |
| `cos(0)`           | `1.0000000`     |
| `tan(PI/4)`        | `1.0000000`     |

### Nested Expressions
| Expression       | Expected Output |
|------------------|-----------------|
| `(2+3)*(5-1)`    | `20.0000000`    |
| `sqrt(16)+log(E)`| `5.0000000`     |

### Invalid Inputs
| Input               | Error Message                     |
|---------------------|------------------------------------|
| `2++2`              | `Error: Unknown token: +`         |
| `(2+2`              | `Error: Mismatched parentheses.`  |
| `log(-1)`           | `Error: Logarithm domain error.`  |
| `1/0`               | `Error: Division by zero.`        |

---

## Code Highlights

### Tokenizer
The `tokenize` function splits the input string into valid tokens such as numbers, operators, and parentheses.

```cpp
std::vector<Token> tokenize(const std::string& expression) {
    std::vector<Token> tokens;
    std::string current;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (std::isspace(c)) continue;

        if (std::isdigit(c) || c == '.') {
            current += c;
            if (i == expression.size() - 1 || !std::isdigit(expression[i + 1]) && expression[i + 1] != '.') {
                tokens.push_back({TokenType::Number, current});
                current.clear();
            }
        } else if (std::isalpha(c)) {
            current += c;
            if (i == expression.size() - 1 || !std::isalpha(expression[i + 1])) {
                tokens.push_back({TokenType::Function, current});
                current.clear();
            }
        } else if (c == '(' || c == ')') {
            tokens.push_back({TokenType::Parenthesis, std::string(1, c)});
        } else if (precedence_.count(std::string(1, c))) {
            tokens.push_back({TokenType::Operator, std::string(1, c)});
        } else {
            throw std::runtime_error(std::string("Invalid character: ") + c);
        }
    }

    return tokens;
}
```

### Operator Precedence
The program respects operator precedence using the `precedence_` map:

```cpp
std::unordered_map<std::string, int> precedence_ = {
    {"+", 1}, {"-", 1},
    {"*", 2}, {"/", 2},
    {"^", 3}
};
```

### Error Handling
The program provides comprehensive error handling for:
- Invalid tokens
- Division by zero
- Mismatched parentheses
- Invalid function arguments (e.g., `sqrt(-1)`)

---

## Future Improvements

1. **Extended Functionality**:
   - Add support for hyperbolic functions (e.g., `sinh`, `cosh`).
   - Include additional constants like `π` and `e` for easy reference.
2. **Variable Assignment**:
   - Enable users to define variables (e.g., `x = 5`) for reuse in expressions.
3. **Graphical Interface**:
   - Provide a GUI for entering and visualizing expressions.

---

## Contributing

Contributions are welcome! Feel free to fork this repository and submit pull requests.

---

## License

This project is open-source and licensed under the MIT License. See the `LICENSE` file for details.

---

## Author

Developed by **Vladislav Cernega** with an emphasis on robust error handling and precise operator precedence.

