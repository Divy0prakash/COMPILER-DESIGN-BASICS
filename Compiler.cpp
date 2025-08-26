#include <iostream>
#include <cctype>
#include <string>
using namespace std;

class SimpleCompiler {
    string input;
    int pos;
    char currentChar;

public:
    SimpleCompiler(string expr) : input(expr), pos(0) {
        currentChar = input[pos];
    }

    // Advance to next character
    void advance() {
        pos++;
        if (pos < input.size())
            currentChar = input[pos];
        else
            currentChar = '\0'; // End of input
    }

    // Skip whitespace
    void skipWhitespace() {
        while (isspace(currentChar))
            advance();
    }

    // Parse numbers
    int integer() {
        int result = 0;
        while (isdigit(currentChar)) {
            result = result * 10 + (currentChar - '0');
            advance();
        }
        return result;
    }

    // Grammar:
    // expr   → term ((+|-) term)*
    // term   → factor ((*|/) factor)*
    // factor → NUMBER | (expr)

    int factor() {
        skipWhitespace();
        int result;
        if (isdigit(currentChar)) {
            result = integer();
        } else if (currentChar == '(') {
            advance();
            result = expr();
            if (currentChar == ')')
                advance();
            else
                throw runtime_error("Error: Missing closing parenthesis");
        } else {
            throw runtime_error("Error: Invalid factor");
        }
        return result;
    }

    int term() {
        int result = factor();
        while (currentChar == '*' || currentChar == '/') {
            char op = currentChar;
            advance();
            int right = factor();
            if (op == '*')
                result *= right;
            else {
                if (right == 0) throw runtime_error("Error: Division by zero");
                result /= right;
            }
        }
        return result;
    }

    int expr() {
        int result = term();
        while (currentChar == '+' || currentChar == '-') {
            char op = currentChar;
            advance();
            int right = term();
            if (op == '+')
                result += right;
            else
                result -= right;
        }
        return result;
    }

    int parse() {
        int result = expr();
        if (currentChar != '\0')
            throw runtime_error("Error: Unexpected character in expression");
        return result;
    }
};

int main() {
    cout << "Simple Arithmetic Compiler\n";
    cout << "Enter an expression (or type 'exit' to quit):\n";

    string expression;
    while (true) {
        cout << "\n> ";
        getline(cin, expression);
        if (expression == "exit")
            break;

        try {
            SimpleCompiler compiler(expression);
            int result = compiler.parse();
            cout << "Result = " << result << endl;
        } catch (exception &e) {
            cout << e.what() << endl;
        }
    }
    return 0;
}
