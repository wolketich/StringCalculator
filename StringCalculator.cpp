#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <functional>
#include <cctype>

namespace MathCalc {

    const double PI = std::acos(-1.0);
    const double E = std::exp(1.0);

    enum class TokenType {
        Number,
        Operator,
        Function,
        Parenthesis
    };

    struct Token {
        TokenType type;
        std::string value;
    };

    class Calculator {
    private:
        std::stack<double> values_;  // Stack to store numbers
        std::stack<Token> operators_; // Stack to store operators
        std::unordered_map<std::string, std::function<double(double)>> functions_; // Supported functions

        // Operator precedence map
        std::unordered_map<std::string, int> precedence_ = {
            {"+", 1}, {"-", 1},
            {"*", 2}, {"/", 2},
            {"^", 3}
        };

        void initializeFunctions() {
            functions_ = {
                {"sin", [](double x) { return std::sin(x); }},
                {"cos", [](double x) { return std::cos(x); }},
                {"tan", [](double x) {
                    if (std::cos(x) == 0) throw std::domain_error("Undefined tangent (division by zero).");
                    return std::tan(x);
                }},
                {"sqrt", [](double x) {
                    if (x < 0) throw std::domain_error("Square root of negative number.");
                    return std::sqrt(x);
                }},
                {"log", [](double x) {
                    if (x <= 0) throw std::domain_error("Logarithm domain error.");
                    return std::log(x);
                }},
                {"abs", [](double x) { return std::abs(x); }}
            };
        }

        int getPrecedence(const std::string& op) const {
            if (precedence_.count(op)) {
                return precedence_.at(op);
            }
            return 0; // Unknown or parentheses
        }

        void executeTopOperation() {
            if (operators_.empty()) throw std::runtime_error("Operator stack underflow.");
            Token op = operators_.top();
            operators_.pop();

            if (op.type == TokenType::Function) {
                if (values_.empty()) throw std::runtime_error("Missing operand for function " + op.value);
                double arg = values_.top();
                values_.pop();
                values_.push(functions_.at(op.value)(arg));
                return;
            }

            if (values_.size() < 2) throw std::runtime_error("Missing operands for operator " + op.value);
            double b = values_.top(); values_.pop();
            double a = values_.top(); values_.pop();

            double result = 0;
            if (op.value == "+") result = a + b;
            else if (op.value == "-") result = a - b;
            else if (op.value == "*") result = a * b;
            else if (op.value == "/") {
                if (b == 0) throw std::runtime_error("Division by zero.");
                result = a / b;
            } else if (op.value == "^") result = std::pow(a, b);
            else throw std::runtime_error("Unknown operator " + op.value);

            values_.push(result);
        }

        void processOperator(const Token& op) {
            while (!operators_.empty()) {
                Token top = operators_.top();
                if (top.type != TokenType::Operator) break;
                if (getPrecedence(op.value) > getPrecedence(top.value)) break;
                executeTopOperation();
            }
            operators_.push(op);
        }

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

    public:
        Calculator() {
            initializeFunctions();
        }

        double evaluate(const std::string& expression) {
            auto tokens = tokenize(expression);

            for (const auto& token : tokens) {
                if (token.type == TokenType::Number) {
                    values_.push(std::stod(token.value));
                } else if (token.type == TokenType::Function) {
                    operators_.push(token);
                } else if (token.type == TokenType::Parenthesis) {
                    if (token.value == "(") {
                        operators_.push(token);
                    } else {
                        while (!operators_.empty() && operators_.top().value != "(") {
                            executeTopOperation();
                        }
                        if (operators_.empty() || operators_.top().value != "(") {
                            throw std::runtime_error("Mismatched parentheses.");
                        }
                        operators_.pop();
                    }
                } else if (token.type == TokenType::Operator) {
                    processOperator(token);
                }
            }

            while (!operators_.empty()) {
                executeTopOperation();
            }

            if (values_.size() != 1) {
                throw std::runtime_error("Expression resulted in multiple values.");
            }

            return values_.top();
        }
    };
} // namespace MathCalc

int main() {
    try {
        MathCalc::Calculator calculator;
        std::cout << "Enter a mathematical expression:\n";
        std::string input;
        std::getline(std::cin, input);

        double result = calculator.evaluate(input);
        std::cout << "Result: " << std::fixed << std::setprecision(7) << result << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}