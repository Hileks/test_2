#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

// Мы храним выражение и текущую позицию чтения
string s;
int pos = 0;

// Пропускаем пробелы
void skipSpaces() {
    while (pos < (int)s.size() && isspace((unsigned char)s[pos])) pos++;
}

// Читаем число (например 12 или 12.5)
double parseNumber() {
    skipSpaces();

    int start = pos;
    bool dotSeen = false;

    if (pos < (int)s.size() && (isdigit((unsigned char)s[pos]) || s[pos] == '.')) {
        if (s[pos] == '.') dotSeen = true;
        pos++;

        while (pos < (int)s.size()) {
            char c = s[pos];
            if (isdigit((unsigned char)c)) {
                pos++;
            }
            else if (c == '.' && !dotSeen) {
                dotSeen = true;
                pos++;
            }
            else {
                break;
            }
        }

        return stod(s.substr(start, pos - start));
    }

    throw runtime_error("Ожидалось число");
}

// Factor: число ИЛИ (Expression) ИЛИ унарный минус -Factor
double parseFactor();

// Term: Factor ((* или /) Factor)*
double parseTerm() {
    double left = parseFactor();

    while (true) {
        skipSpaces();
        if (pos >= (int)s.size()) break;

        char op = s[pos];
        if (op != '*' && op != '/') break;

        pos++; // съели оператор
        double right = parseFactor();

        if (op == '*') left = left * right;
        else {
            if (right == 0) throw runtime_error("Деление на ноль");
            left = left / right;
        }
    }

    return left;
}

// Expression: Term ((+ или -) Term)*
double parseExpression() {
    double left = parseTerm();

    while (true) {
        skipSpaces();
        if (pos >= (int)s.size()) break;

        char op = s[pos];
        if (op != '+' && op != '-') break;

        pos++; // съели оператор
        double right = parseTerm();

        if (op == '+') left = left + right;
        else left = left - right;
    }

    return left;
}

// Factor: число, скобки, или унарный минус
double parseFactor() {
    skipSpaces();

    // Унарный минус: -Factor
    if (pos < (int)s.size() && s[pos] == '-') {
        pos++; // съели '-'
        return -parseFactor();
    }

    // Скобки: (Expression)
    if (pos < (int)s.size() && s[pos] == '(') {
        pos++; // съели '('
        double val = parseExpression();
        skipSpaces();

        if (pos >= (int)s.size() || s[pos] != ')')
            throw runtime_error("Не найдена закрывающая скобка )");

        pos++; // съели ')'
        return val;
    }

    // Иначе должно быть число
    return parseNumber();
}

int main() {
    setlocale(LC_ALL, "");

    cout << "Калькулятор выражений (+ - * / и скобки)\n";
    cout << "Пример: 5*(2+1)\n";
    cout << "Введите выражение: ";

    getline(cin, s);
    pos = 0;

    try {
        double result = parseExpression();
        skipSpaces();

        if (pos != (int)s.size()) {
            throw runtime_error("Лишние символы в конце выражения");
        }

        cout << "Результат: " << result << "\n";
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}