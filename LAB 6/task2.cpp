/*
 * CS-250 Data Structures and Algorithms - Lab 6
 * Task 2: Expression Validators Using Stack
 *
 * Part A - isValidBrackets(s) : balanced / correctly nested (), {}, []
 * Part B - validate_markup(s) : simplified HTML/XML tag nesting check
 *
 * The stack (stack.h) is implemented from scratch as a linked list.
 *
 * Compile : g++ -std=c++17 -Wall -Wextra task2_validators.cpp -o task2
 * Run     : ./task2
 */

#include <cctype>
#include <iostream>
#include <string>

#include "stack.h"
#include "stringlist.h"

using namespace std;

// ---------------------------------------------------------------------------
// Part A - Bracket validator
// ---------------------------------------------------------------------------

// True if every (), {} and [] is balanced and correctly nested. Characters
// that are not brackets are ignored.  O(n) time, O(n) space.
bool isValidBrackets(const string& s) {
    Stack<char> st;
    for (size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch == '(' || ch == '{' || ch == '[') {
            st.push(ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (st.isEmpty()) return false;
            char open = st.pop();
            bool match = (ch == ')' && open == '(') || (ch == '}' && open == '{') ||
                         (ch == ']' && open == '[');
            if (!match) return false;
        }
    }
    return st.isEmpty();
}

// ---------------------------------------------------------------------------
// Part B - HTML / XML tag validator
// ---------------------------------------------------------------------------
struct MarkupResult {
    bool valid;
    StringList errors;
};

struct TagEntry {
    string name;
    int line, col;
    TagEntry(const string& n, int l, int c) : name(n), line(l), col(c) {}
};

// HTML "void" elements never have a closing tag (accepted when allowVoid).
static bool isVoidTag(const string& name) {
    string low;
    for (size_t i = 0; i < name.size(); ++i)
        low += static_cast<char>(tolower(static_cast<unsigned char>(name[i])));
    return low == "br" || low == "hr" || low == "img" || low == "input" || low == "meta" ||
           low == "link";
}

// Convert a string index into a human friendly line and column.
static void lineColInts(const string& s, size_t index, int& line, int& col) {
    line = 1;
    col = 1;
    for (size_t i = 0; i < index; ++i) {
        if (s[i] == '\n') {
            ++line;
            col = 1;
        } else {
            ++col;
        }
    }
}

// Tag names start with a letter or '_', then letters/digits/-/_/./:
static bool validName(const string& name) {
    if (name.empty()) return false;
    if (!(isalpha(static_cast<unsigned char>(name[0])) || name[0] == '_')) return false;
    for (size_t i = 0; i < name.size(); ++i) {
        char c = name[i];
        if (!(isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' ||
              c == ':'))
            return false;
    }
    return true;
}

// Index of the '>' that closes the tag starting at s[start] == '<'.
// A '>' inside a quoted attribute value is skipped. npos if missing.
static size_t findTagEnd(const string& s, size_t start) {
    char quote = 0;
    for (size_t i = start + 1; i < s.size(); ++i) {
        char c = s[i];
        if (quote) {
            if (c == quote) quote = 0;
        } else if (c == '"' || c == '\'') {
            quote = c;
        } else if (c == '>') {
            return i;
        }
    }
    return string::npos;
}

static string trim(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

/*
 * Validate tag nesting in a simplified HTML/XML string.
 * Returns {valid, errors}; each error is "line:col message".
 * Handles attributes, closing tags, self-closing tags (<br/>), comments,
 * <!DOCTYPE>, <?xml ?> and (optionally) HTML void tags like <br>.
 * Tag names are case-sensitive (XML rules).
 *
 * Stack entries hold (name, line, col) so unclosed tags can be reported with
 * the position where they were opened.
 */
MarkupResult validate_markup(const string& s, bool allowVoid = true) {
    MarkupResult res;
    res.valid = true;
    Stack<TagEntry> st;
    size_t i = 0;

    while (i < s.size()) {
        if (s[i] != '<') {
            ++i;
            continue;
        }
        int line, col;
        lineColInts(s, i, line, col);
        string pos = to_string(line) + ":" + to_string(col);

        // --- comments <!-- ... -->
        if (s.compare(i, 4, "<!--") == 0) {
            size_t end = s.find("-->", i + 4);
            if (end == string::npos) {
                res.errors.add(pos + " unterminated comment");
                break;
            }
            i = end + 3;
            continue;
        }

        // --- the tag itself
        size_t end = findTagEnd(s, i);
        if (end == string::npos) {
            res.errors.add(pos + " unterminated tag (missing '>')");
            break;
        }
        string body = s.substr(i + 1, end - i - 1);
        i = end + 1;

        // <!DOCTYPE ...> and <?xml ... ?> are not nesting tags
        if (!body.empty() && (body[0] == '!' || body[0] == '?')) continue;

        // --- closing tag </name>
        if (!body.empty() && body[0] == '/') {
            string name = trim(body.substr(1));
            if (!validName(name)) {
                res.errors.add(pos + " invalid closing tag '</" + name + ">'");
                continue;
            }
            if (st.isEmpty()) {
                res.errors.add(pos + " unexpected closing tag </" + name + "> with no open tag");
            } else if (st.peek().name == name) {
                st.pop();
            } else if (st.contains([&](const TagEntry& t) { return t.name == name; })) {
                // Tags opened after <name> were never closed: report them.
                while (st.peek().name != name) {
                    TagEntry t = st.pop();
                    res.errors.add(to_string(t.line) + ":" + to_string(t.col) + " tag <" + t.name +
                                   "> is not closed before </" + name + "> at " + pos);
                }
                st.pop();
            } else {
                res.errors.add(pos + " mismatched closing tag </" + name +
                               "> (currently open: <" + st.peek().name + ">)");
            }
            continue;
        }

        // --- self-closing <name ... />
        bool selfClosing = !body.empty() && body[body.size() - 1] == '/';
        if (selfClosing) body.erase(body.size() - 1);

        size_t j = 0;
        while (j < body.size() && !isspace(static_cast<unsigned char>(body[j]))) ++j;
        string name = body.substr(0, j);

        if (!validName(name)) {
            res.errors.add(pos + " invalid tag name '<" + name + ">'");
            continue;
        }
        if (selfClosing) continue;
        if (allowVoid && isVoidTag(name)) continue;
        st.push(TagEntry(name, line, col));
    }

    // Anything left on the stack was never closed
    while (!st.isEmpty()) {
        TagEntry t = st.pop();
        res.errors.add(to_string(t.line) + ":" + to_string(t.col) + " tag <" + t.name +
                       "> is never closed");
    }
    res.valid = (res.errors.size() == 0);
    return res;
}

// ---------------------------------------------------------------------------
// Driver / tests
// ---------------------------------------------------------------------------
static int failures = 0;

// Show newlines as \n when echoing a test string.
static string shown(const string& s) {
    string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\n') out += "\\n";
        else out += s[i];
    }
    return out;
}

static void testBrackets() {
    cout << string(60, '=') << "\nPart A - isValidBrackets\n" << string(60, '=') << "\n";
    struct Case {
        const char* text;
        bool expected;
    };
    const Case cases[] = {{"()", true},      {"()[]{}", true},     {"{[()]}", true},
                          {"a*(b+[c-{d/e}])", true}, {"", true},   {"(]", false},
                          {"([)]", false},   {"((", false},        {"))", false},
                          {"{[}", false},    {"(a+b", false}};
    for (const Case& c : cases) {
        bool got = isValidBrackets(c.text);
        bool ok = (got == c.expected);
        if (!ok) ++failures;
        cout << "\"" << c.text << "\" -> " << (got ? "true " : "false") << "  "
             << (ok ? "PASS" : "FAIL") << "\n";
    }
}

static void testMarkup() {
    cout << "\n" << string(60, '=') << "\nPart B - validate_markup\n" << string(60, '=') << "\n";
    struct Case {
        const char* text;
        bool expected;
    };
    const Case cases[] = {
        {"<html><body><p>Hi</p></body></html>", true},
        {"<a href=\"x>y\">link</a>", true},
        {"<div><br/><img src='a.png'></div>", true},
        {"<?xml version='1.0'?><!DOCTYPE note><note><to>A</to></note>", true},
        {"<!-- <b> not a tag --><p>ok</p>", true},
        {"<div>\n  <p>text\n</div>", false},
        {"<b><i>text</b></i>", false},
        {"</p>", false},
        {"<div><span>text</div>", false},
        {"<a><b>", false},
        {"<p>never closed", false},
        {"<p unterminated", false},
        {"<1bad>x</1bad>", false},
        {"<!-- never ends <p>x</p>", false},
    };
    for (const Case& c : cases) {
        MarkupResult r = validate_markup(c.text);
        bool ok = (r.valid == c.expected);
        if (!ok) ++failures;
        cout << shown(c.text) << "\n   valid=" << (r.valid ? "true" : "false") << "  ["
             << (ok ? "PASS" : "FAIL") << "]\n";
        for (int k = 0; k < r.errors.size(); ++k) cout << "   - " << r.errors.at(k) << "\n";
    }

    MarkupResult r = validate_markup("<br>", false);
    cout << "\n'<br>' with allowVoid=false -> valid=" << (r.valid ? "true" : "false") << "\n";
    for (int k = 0; k < r.errors.size(); ++k) cout << "   - " << r.errors.at(k) << "\n";
    if (r.valid) ++failures;
}

int main() {
    testBrackets();
    testMarkup();
    cout << "\n" << (failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    return failures == 0 ? 0 : 1;
}