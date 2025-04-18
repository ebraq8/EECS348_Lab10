#include "calculator.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

bool isValidDouble(const std::string& s) {
    if (s.empty()) return false;
    int i = 0, n = s.size();
    if (s[i] == '+' || s[i] == '-') i++;
    bool hasDigit = false, hasDot = false;
    for (; i < n; i++) {
        if (isdigit(s[i])) {
            hasDigit = true;
        } else if (s[i] == '.') {
            if (hasDot) return false;
            hasDot = true;
            if (i == n - 1) return false;
        } else {
            return false;
        }
    }
    return hasDigit;
}

std::string removeLeadingZeros(const std::string& str) {
    int i = 0;
    while (i < str.size() - 1 && str[i] == '0') i++;
    return str.substr(i);
}

std::string normalizeInteger(const std::string& str) {
    std::string cleaned = removeLeadingZeros(str);
    return cleaned.empty() ? "0" : cleaned;
}

std::string padRight(const std::string& s, size_t len) {
    std::string result = s;
    while (result.length() < len) result += '0';
    return result.substr(0, len); // also trims extra digits
}

int compareStrings(const std::string& a, const std::string& b) {
    if (a.size() > b.size()) return 1;
    if (a.size() < b.size()) return -1;
    return a.compare(b);
}

std::string subtractStrings(const std::string& a, const std::string& b) {
    std::string result = "";
    int borrow = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        int digitA = a[i] - '0';
        int digitB = b[i] - '0' + borrow;
        if (digitA < digitB) {
            digitA += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result = char((digitA - digitB) + '0') + result;
    }
    return removeLeadingZeros(result);
}

std::string addStrings(const std::string& a, const std::string& b) {
    std::string result = "";
    int carry = 0;
    for (int i = a.size() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / 10;
        result = char((sum % 10) + '0') + result;
    }
    if (carry) result = '1' + result;
    return result;
}

std::string addDoubles(const std::string& a, const std::string& b) {
    auto split = [](const std::string& num, std::string& sign, std::string& intPart, std::string& fracPart) {
        sign = "+";
        size_t i = 0;
        if (num[0] == '+' || num[0] == '-') {
            sign = num.substr(0, 1);
            i = 1;
        }
        size_t dot = num.find('.', i);
        if (dot == std::string::npos) {
            intPart = num.substr(i);
            fracPart = "000";
        } else {
            intPart = num.substr(i, dot - i);
            fracPart = num.substr(dot + 1);
        }
    };

    std::string signA, intA, fracA;
    std::string signB, intB, fracB;
    split(a, signA, intA, fracA);
    split(b, signB, intB, fracB);

    fracA = padRight(fracA, 3);
    fracB = padRight(fracB, 3);
    while (intA.length() < intB.length()) intA = '0' + intA;
    while (intB.length() < intA.length()) intB = '0' + intB;

    std::string resultInt, resultFrac, result;
    if (signA == signB) {
        resultFrac = addStrings(fracA, fracB);
        int carry = 0;
        if (resultFrac.length() > fracA.length()) {
            carry = 1;
            resultFrac = resultFrac.substr(1);
        }
        resultInt = addStrings(intA, intB);
        if (carry) resultInt = addStrings(resultInt, "1");
        resultInt = normalizeInteger(resultInt);
        resultFrac = padRight(resultFrac, 3);
        result = resultInt + "." + resultFrac;
        if (signA == "-") result = "-" + result;
    } else {
        int cmpInt = compareStrings(intA, intB);
        int cmpFrac = compareStrings(fracA, fracB);
        bool aGreater = (cmpInt > 0) || (cmpInt == 0 && cmpFrac >= 0);

        if (aGreater) {
            resultFrac = cmpFrac >= 0 ? subtractStrings(fracA, fracB)
                                      : subtractStrings("1" + fracA, fracB);
            resultInt = cmpFrac >= 0 ? subtractStrings(intA, intB)
                                     : subtractStrings(subtractStrings(intA, intB), "1");
            resultInt = normalizeInteger(resultInt);
            resultFrac = padRight(resultFrac, 3);
            result = resultInt + "." + resultFrac;
            if (signA == "-") result = "-" + result;
        } else {
            resultFrac = cmpFrac < 0 ? subtractStrings(fracB, fracA)
                                     : subtractStrings("1" + fracB, fracA);
            resultInt = cmpFrac <= 0 ? subtractStrings(intB, intA)
                                     : subtractStrings(subtractStrings(intB, intA), "1");
            resultInt = normalizeInteger(resultInt);
            resultFrac = padRight(resultFrac, 3);
            result = resultInt + "." + resultFrac;
            if (signB == "-") result = "-" + result;
        }
    }
    return result;
}
