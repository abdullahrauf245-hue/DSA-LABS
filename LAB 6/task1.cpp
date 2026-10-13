
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "stack.h"
#include "stringlist.h"

using namespace std;
static bool isOperator(const string& t) {
    return t.size() == 1 && string("+-*/%^").find(t[0]) != string::npos;
}
static int precedence(const string& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;  // '(' and anything else
}
static StringList tokenizeInfix(const string& expr) {
    StringList tokens;
    size_t i = 0;
    while (i < expr.size()) {
        char ch = expr[i];
        if (isspace(static_cast<unsigned char>(ch))) {
            ++i;
        } else if (isalnum(static_cast<unsigned char>(ch)) || ch == '.') {
            size_t j = i;
            while (j < expr.size() &&
                   (isalnum(static_cast<unsigned char>(expr[j])) || expr[j] == '.'))
                ++j;
            tokens.add(expr.substr(i, j - i));
            i = j;
        } else if (string("+-*/%^()").find(ch) != string::npos) {
            tokens.add(string(1, ch));
            ++i;
        } else {
            throw invalid_argument(string("invalid character '") + ch + "' in expression");
        }
    }
    return tokens;
}

// Postfix/prefix: split on spaces if there are any, else one char per token.
static StringList tokenizeOther(const string& expr) {
    StringList tokens;
    bool hasSpace = false;
    for (size_t i = 0; i < expr.size(); ++i)
        if (isspace(static_cast<unsigned char>(expr[i]))) hasSpace = true;

    if (hasSpace) {
        size_t i = 0;
        while (i < expr.size()) {
            while (i < expr.size() && isspace(static_cast<unsigned char>(expr[i]))) ++i;
            size_t j = i;
            while (j < expr.size() && !isspace(static_cast<unsigned char>(expr[j]))) ++j;
            if (j > i) tokens.add(expr.substr(i, j - i));
            i = j;
        }
    } else {
        for (size_t i = 0; i < expr.size(); ++i) tokens.add(string(1, expr[i]));
    }
    return tokens;
}

// "ABC*+" style when every token is 1 char, otherwise space separated.
static string joinTokens(const StringList& t) {
    bool allSingle = true;
    for (int i = 0; i < t.size(); ++i)
        if (t.at(i).size() != 1) allSingle = false;
    string out;
    for (int i = 0; i < t.size(); ++i) {
        if (i > 0 && !allSingle) out += ' ';
        out += t.at(i);
    }
    return out;
}
static StringList toPostfixTokens(const StringList& in, bool reversedMode) {
    StringList out;
    Stack<string> st;
    for (int i = 0; i < in.size(); ++i) {
        const string& tok = in.at(i);
        if (tok == "(") {
            st.push(tok);
        } else if (tok == ")") {
            while (!st.isEmpty() && st.peek() != "(") out.add(st.pop());
            if (st.isEmpty()) throw invalid_argument("mismatched parentheses");
            st.pop();  // discard '('
        } else if (isOperator(tok)) {
            while (!st.isEmpty() && st.peek() != "(") {
                int pt = precedence(st.peek()), pc = precedence(tok);
                bool popIt;
                if (reversedMode)
                    popIt = (pt > pc) || (pt == pc && tok == "^");
                else
                    popIt = (pt > pc) || (pt == pc && tok != "^");
                if (!popIt) break;
                out.add(st.pop());
            }
            st.push(tok);
        } else {
            out.add(tok);  // operand
        }
    }
    while (!st.isEmpty()) {
        string top = st.pop();
        if (top == "(") throw invalid_argument("mismatched parentheses");
        out.add(top);
    }
    return out;
}

static double applyOp(const string& op, double a, double b) {
    switch (op[0]) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("division by zero");
            return a / b;
        case '%':
            if (b == 0) throw runtime_error("modulo by zero");
            return fmod(a, b);
        case '^': return pow(a, b);
    }
    throw invalid_argument("unknown operator " + op);
}

static double toNumber(const string& tok) {
    size_t used = 0;
    double v;
    try {
        v = stod(tok, &used);
    } catch (...) {
        throw invalid_argument("'" + tok + "' is not a number");
    }
    if (used != tok.size()) throw invalid_argument("'" + tok + "' is not a number");
    return v;
}

string infix_to_postfix(string expr) {
    return joinTokens(toPostfixTokens(tokenizeInfix(expr), false));
}

string infix_to_prefix(string expr) {
    StringList tokens = tokenizeInfix(expr);
    tokens.reverse();
    StringList swapped;
    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens.at(i) == "(") swapped.add(")");
        else if (tokens.at(i) == ")") swapped.add("(");
        else swapped.add(tokens.at(i));
    }
    StringList post = toPostfixTokens(swapped, true);
    post.reverse();
    return joinTokens(post);
}

// ABC*+  ->  (A+(B*C))   (fully parenthesised, so it is unambiguous)
string postfix_to_infix(string expr) {
    StringList tokens = tokenizeOther(expr);
    Stack<string> st;
    for (int i = 0; i < tokens.size(); ++i) {
        const string& tok = tokens.at(i);
        if (isOperator(tok)) {
            if (st.size() < 2) throw invalid_argument("malformed postfix expression");
            string b = st.pop();
            string a = st.pop();
            st.push("(" + a + tok + b + ")");
        } else {
            st.push(tok);
        }
    }
    if (st.size() != 1) throw invalid_argument("malformed postfix expression");
    return st.pop();
}

// Scan left to right: operands are pushed, an operator pops b then a and
// pushes (a OP b).
double evaluate_postfix(string expr) {
    StringList tokens = tokenizeOther(expr);
    Stack<double> st;
    for (int i = 0; i < tokens.size(); ++i) {
        const string& tok = tokens.at(i);
        if (isOperator(tok)) {
            if (st.size() < 2) throw invalid_argument("malformed postfix expression");
            double b = st.pop();
            double a = st.pop();
            st.push(applyOp(tok, a, b));
        } else {
            st.push(toNumber(tok));
        }
    }
    if (st.size() != 1) throw invalid_argument("malformed postfix expression");
    return st.pop();
}

// Scan right to left: an operator pops a then b and pushes (a OP b).
double evaluate_prefix(string expr) {
    StringList tokens = tokenizeOther(expr);
    Stack<double> st;
    for (int i = tokens.size() - 1; i >= 0; --i) {
        const string& tok = tokens.at(i);
        if (isOperator(tok)) {
            if (st.size() < 2) throw invalid_argument("malformed prefix expression");
            double a = st.pop();
            double b = st.pop();
            st.push(applyOp(tok, a, b));
        } else {
            st.push(toNumber(tok));
        }
    }
    if (st.size() != 1) throw invalid_argument("malformed prefix expression");
    return st.pop();
}

static int failures = 0;

static void demo() {
    cout << string(60, '=') << "\nSymbolic conversions\n" << string(60, '=') << "\n";
    const char* symbolic[] = {"A+B*C",          "(A+B)*C",         "A+B*C-D/E", "A^B^C",
                              "A-B-C",          "((A+B)*(C-D))/E", "A*(B+C)*D"};
    for (const char* e : symbolic) {
        string post = infix_to_postfix(e);
        cout << "infix   : " << e << "\n"
             << "postfix : " << post << "\n"
             << "prefix  : " << infix_to_prefix(e) << "\n"
             << "post->in: " << postfix_to_infix(post) << "\n"
             << string(60, '-') << "\n";
    }

    cout << string(60, '=') << "\nNumeric evaluation (postfix and prefix must agree)\n"
         << string(60, '=') << "\n";
    struct Case {
        const char* expr;
        double expected;
    };
    const Case cases[] = {{"2+3*4", 14},     {"(2+3)*4", 20},      {"10-4-3", 3},
                          {"2^3^2", 512},    {"100/10/5", 2},      {"12+34*2", 80},
                          {"(5+3)*(10-4)/2", 24}, {"7%4+2", 5}};
    for (const Case& c : cases) {
        string post = infix_to_postfix(c.expr);
        string pre = infix_to_prefix(c.expr);
        double vp = evaluate_postfix(post);
        double vr = evaluate_prefix(pre);
        bool ok = fabs(vp - c.expected) < 1e-9 && fabs(vr - c.expected) < 1e-9;
        if (!ok) ++failures;
        cout << c.expr << "\n   postfix: " << post << "   prefix: " << pre << "\n   = " << vp
             << " / " << vr << "   " << (ok ? "PASS" : "FAIL") << "\n";
    }

    cout << "\nError handling\n";
    const char* bad[] = {"(A+B", "A+B)", "A+*B"};
    for (const char* b : bad) {
        try {
            postfix_to_infix(infix_to_postfix(b));
            cout << "'" << b << "': no error?!\n";
            ++failures;
        } catch (const invalid_argument& ex) {
            cout << "'" << b << "': invalid_argument - " << ex.what() << "\n";
        }
    }
    try {
        evaluate_postfix("4 0 /");
        ++failures;
    } catch (const runtime_error& ex) {
        cout << "'4 0 /': runtime_error - " << ex.what() << "\n";
    }
}

int main() {
    demo();
    cout << "\n" << (failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    return failures == 0 ? 0 : 1;
}