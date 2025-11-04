// EECS 348 Lab 10
// Elliott Erhard

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

// Helper: remove leading zeros (for both integer and fractional parts)
string removeLeadingZeros(const string& str) {
    string s = str;
    int pos = 0;
    while (pos < (int)s.size() - 1 && s[pos] == '0') pos++;
    return s.substr(pos);
}

// Validate if a string is a proper double (with optional +/-, decimal, etc.)
bool isValidDouble(const string& s) {
    if (s.empty()) return false;

    int i = 0, n = s.size();
    if (s[i] == '+' || s[i] == '-') i++;
    bool hasDigits = false, hasDecimal = false;
    int digitsBefore = 0, digitsAfter = 0;

    for (; i < n; i++) {
        char c = s[i];
        if (isdigit(c)) {
            hasDigits = true;
            if (!hasDecimal) digitsBefore++;
            else digitsAfter++;
        } else if (c == '.') {
            if (hasDecimal) return false; // multiple decimals
            hasDecimal = true;
        } else return false;
    }

    if (hasDecimal) return digitsBefore > 0 && digitsAfter > 0;
    return hasDigits;
}

// Normalize by removing leading zeros and handling + sign
string normalize(string s) {
    bool neg = false;
    if (s[0] == '-') { neg = true; s = s.substr(1); }
    else if (s[0] == '+') s = s.substr(1);

    string intPart = s, fracPart = "";
    size_t dot = s.find('.');
    if (dot != string::npos) {
        intPart = s.substr(0, dot);
        fracPart = s.substr(dot + 1);
    }
    intPart = removeLeadingZeros(intPart);
    while (!fracPart.empty() && fracPart.back() == '0') fracPart.pop_back();

    string result = (fracPart.empty()) ? intPart : intPart + "." + fracPart;
    if (result.empty()) result = "0";
    if (neg && result != "0") result = "-" + result;
    return result;
}

// Add two positive string numbers (integer parts only)
string addStrings(string a, string b) {
    string result = "";
    int carry = 0;
    int i = a.size() - 1, j = b.size() - 1;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        result += (sum % 10) + '0';
    }

    reverse(result.begin(), result.end());
    return result;
}

// Add two valid non-negative string doubles
string addDoubles(string a, string b) {
    // Split into integer and fractional parts
    string a_int, a_frac, b_int, b_frac;
    size_t dotA = a.find('.'), dotB = b.find('.');

    if (dotA != string::npos) { a_int = a.substr(0, dotA); a_frac = a.substr(dotA + 1); }
    else a_int = a;

    if (dotB != string::npos) { b_int = b.substr(0, dotB); b_frac = b.substr(dotB + 1); }
    else b_int = b;

    // Pad fractional parts
    int maxFrac = max((int)a_frac.size(), (int)b_frac.size());
    a_frac.append(maxFrac - a_frac.size(), '0');
    b_frac.append(maxFrac - b_frac.size(), '0');

    // Add fractional part
    string fracSum = addStrings(a_frac, b_frac);
    bool carry = false;
    if ((int)fracSum.size() > maxFrac) {
        carry = true;
        fracSum.erase(fracSum.begin()); // remove carry digit
    }

    // Add integer part (with carry if needed)
    string intSum = addStrings(a_int, b_int);
    if (carry) intSum = addStrings(intSum, "1");

    // Trim trailing zeros in fraction
    while (!fracSum.empty() && fracSum.back() == '0') fracSum.pop_back();

    return fracSum.empty() ? intSum : intSum + "." + fracSum;
}

void processFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string a, b;
    int lineNum = 1;
    while (file >> a >> b) {
        cout << "Line " << lineNum++ << ": " << a << " , " << b << endl;
        if (!isValidDouble(a)) { cout << "Invalid number: " << a << "\n\n"; continue; }
        if (!isValidDouble(b)) { cout << "Invalid number: " << b << "\n\n"; continue; }

        a = normalize(a);
        b = normalize(b);

        cout << "Valid numbers: " << a << " + " << b << endl;
        string result = addDoubles(a, b);
        cout << "Sum: " << result << "\n\n";
    }
}

int main() {
    cout << "=== EECS 348 Lab 10: String-Based Calculator ===\n";
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    processFile(filename);
    return 0;
}
