/**
 * @file VerboseStringCalculator.cpp
 * @brief Extended Mathematical Expression Calculator
 *
 * A comprehensive and verbose implementation of a mathematical expression calculator.
 * This program demonstrates advanced C++ concepts, including class design, templates, 
 * advanced error handling, and modular programming.
 *
 * Universitatea Tehnica a Moldovei
 * Written by: Vladislav Cernega
 */

#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <cmath>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <type_traits>

namespace MathCalc {

    /**
     * @brief Mathematical constants and utility functions
     */
    namespace Constants {
    const double PI = std::acos(-1.0);  ///< Value of π (pi) at runtime
    const double E = std::exp(1.0);    ///< Value of Euler's number e at runtime

    /**
     * @brief Utility function to round a double value to specified decimal places.
     * @tparam Precision Number of decimal places to round to.
     * @param value Input double value.
     * @return Rounded value.
     */
    template <std::size_t Precision>
    constexpr double roundTo(double value) {
        static_assert(Precision > 0, "Precision must be greater than zero.");
        const double multiplier = 1.0 * []() {
            double m = 1.0;
            for (std::size_t i = 0; i < Precision; ++i) m *= 10;
            return m;
        }();
        return std::round(value * multiplier) / multiplier;
    }
}

    /**
     * @brief Token types used in expression parsing.
     */
    enum class TokenType {
        Number,       ///< A numeric value
        Operator,     ///< An arithmetic operator (+, -, *, /, ^)
        Function,     ///< A mathematical function (sin, cos, etc.)
        Parenthesis   ///< Parentheses (either '(' or ')')
    };

    /**
     * @brief Token structure representing a single entity in the expression.
     */
    struct Token {
        TokenType type;     ///< Type of the token
        std::string value;  ///< Literal value of the token
    };

    /**
     * @brief Exception class for handling expression errors.
     */
    class ExpressionError : public std::runtime_error {
    public:
        explicit ExpressionError(const std::string& message)
            : std::runtime_error("Expression Error: " + message) {}
    };

    /**
     * @brief Class to evaluate mathematical expressions.
     */
    class Calculator {
    private:
        std::stack<double> values_;          ///< Stack for numeric values
        std::stack<Token> operators_;        ///< Stack for operators and functions
        std::unordered_map<std::string, std::function<double(double)>> functions_; ///< Supported functions

        /**
         * @brief Initializes the supported functions.
         */
        void initializeFunctions() {
            functions_ = {
                {"sin", [](double x) { return Constants::roundTo<7>(std::sin(x)); }},
                {"cos", [](double x) { return Constants::roundTo<7>(std::cos(x)); }},
                {"tan", [](double x) { 
                    if (Constants::roundTo<7>(std::cos(x)) == 0) 
                        throw ExpressionError("Undefined tangent (division by zero).");
                    return Constants::roundTo<7>(std::tan(x)); 
                }},
                {"exp", [](double x) { return std::exp(x); }},
                {"log", [](double x) { 
                    if (x <= 0) throw ExpressionError("Logarithm domain error (non-positive argument).");
                    return std::log(x); 
                }},
                {"sqrt", [](double x) { 
                    if (x < 0) throw ExpressionError("Square root of negative number.");
                    return std::sqrt(x); 
                }},
                {"abs", [](double x) { return std::abs(x); }}
            };
        }

        /**
         * @brief Determines the precedence of an operator.
         * @param op Operator character.
         * @return Integer representing precedence (higher is stronger).
         */
        int getPrecedence(const std::string& op) const {
            if (op == "+" || op == "-") return 1;
            if (op == "*" || op == "/") return 2;
            if (op == "^") return 3;
            if (functions_.count(op)) return 4; // Functions have highest precedence
            return 0; // Parentheses or unknowns
        }

        /**
         * @brief Executes a single operation.
         * Pops operands and operator from the stacks, computes the result, and pushes it back.
         */
        void executeOperation() {
            if (operators_.empty()) throw ExpressionError("Operator stack underflow.");

            auto op = operators_.top();
            operators_.pop();

            if (functions_.count(op.value)) {
                if (values_.empty()) throw ExpressionError("Missing operand for function " + op.value);
                double arg = values_.top();
                values_.pop();
                values_.push(functions_.at(op.value)(arg));
                return;
            }

            if (values_.size() < 2) throw ExpressionError("Missing operands for operator " + op.value);

            double b = values_.top(); values_.pop();
            double a = values_.top(); values_.pop();
            double result;

            if (op.value == "+") result = a + b;
            else if (op.value == "-") result = a - b;
            else if (op.value == "*") result = a * b;
            else if (op.value == "/") {
                if (b == 0) throw ExpressionError("Division by zero.");
                result = a / b;
            } else if (op.value == "^") result = std::pow(a, b);
            else throw ExpressionError("Unknown operator " + op.value);

            values_.push(result);
        }

    public:
        /**
         * @brief Constructor.
         */
        Calculator() {
            initializeFunctions();
        }

        /**
         * @brief Parses and evaluates a mathematical expression.
         * @param expression Input expression as a string.
         * @return Result of the evaluation.
         */
        double evaluate(const std::string& expression) {
            std::istringstream stream(expression);
            std::string token;

            while (stream >> token) {
                if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
                    values_.push(std::stod(token));
                } else if (functions_.count(token) || token == "(" || token == ")") {
                    operators_.push({TokenType::Operator, token});
                } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
                    while (!operators_.empty() &&
                           getPrecedence(token) <= getPrecedence(operators_.top().value)) {
                        executeOperation();
                    }
                    operators_.push({TokenType::Operator, token});
                } else {
                    throw ExpressionError("Unknown token: " + token);
                }
            }

            while (!operators_.empty()) {
                executeOperation();
            }

            if (values_.size() != 1) throw ExpressionError("Expression resulted in multiple values.");
            return values_.top();
        }
    };
} // namespace MathCalc

int main() {
    try {
        std::cout << "Enter a mathematical expression:\n";
        std::string input;
        std::getline(std::cin, input);

        MathCalc::Calculator calculator;
        double result = calculator.evaluate(input);

        std::cout << "Result: " << std::fixed << std::setprecision(7) << result << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}