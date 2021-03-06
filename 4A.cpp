#include <iostream>
#include <string>
#include <vector>

class BigInteger {
    private:
    std::vector<int> numeric;
    bool negative = false;

    void NextNum();

    void Zeros();

    void dif(BigInteger &a, const BigInteger &b);

    void sum(BigInteger &a, const BigInteger &b);

    int compare(const BigInteger &a, int k = 1) const;
    
public:
    BigInteger();
    BigInteger(int a);
    BigInteger(std::string s);

    bool operator<(BigInteger const &b) const;
    bool operator>(BigInteger const &b) const;
    bool operator<=(BigInteger const &b) const;
    bool operator>=(BigInteger const &b) const;
    bool operator==(BigInteger const &b) const;
    bool operator!=(BigInteger const &b) const;

    BigInteger &operator*=(const BigInteger &b);
    BigInteger &operator+=(const BigInteger &c);
    BigInteger &operator%=(const BigInteger &b);
    BigInteger &operator/=(const BigInteger &c);


    BigInteger &operator++();
    const BigInteger operator++(int);
    BigInteger &operator--();
    const BigInteger operator--(int);

    BigInteger abs() const;

    const BigInteger operator-();
    BigInteger &operator-=(const BigInteger &c);

    const std::string toString() const;

    explicit operator bool();

    friend std::istream &operator>>(std::istream &, BigInteger &);
    friend std::ostream &operator<<(std::ostream &, const BigInteger &);

    friend const BigInteger operator*(const BigInteger &b, const BigInteger &a);
    friend const BigInteger operator/(const BigInteger &a, const BigInteger &b);
    friend const BigInteger operator%(const BigInteger &a, const BigInteger &b);
    friend const BigInteger operator+(const BigInteger &a, const BigInteger &c);
    friend const BigInteger operator-(const BigInteger &a, const BigInteger &c);
};


BigInteger::BigInteger() {
    negative = false;
}


BigInteger::BigInteger(int a) {
    if (a < 0) {
        negative = true;
        a *= -1;
    } else if (a == 0) {
        numeric.push_back(0);
        negative = false;
    }
    while (a != 0) {
        numeric.push_back(a % 10);
        a /= 10;
    }

}


BigInteger::BigInteger(std::string s) {
    negative = false;
    for (size_t i = s.length() - 1; i > 0; --i)
        numeric.push_back(s[i] - '0');
    if (s[0] == '-')
        negative = true;
    else
        numeric.push_back(s[0] - '0');
}


const BigInteger operator-(const BigInteger &a, const BigInteger &c) {
    BigInteger dif = a;
    return dif -= c;
}


const BigInteger operator+(const BigInteger &a, const BigInteger &c) {
    BigInteger sum = a;
    return sum += c;
}


const BigInteger operator*(const BigInteger &b, const BigInteger &a) {

    size_t length = a.numeric.size() + b.numeric.size() + 1;
    BigInteger c;
    c.numeric.resize(length, 0);

    for (size_t i = 0; i < a.numeric.size(); ++i)
        for (size_t j = 0; j < b.numeric.size(); ++j)
            c.numeric[i + j] += a.numeric[i] * b.numeric[j];

    for (size_t i = 0; i < length; ++i) {
        if (i < length - 1)
            c.numeric[i + 1] += c.numeric[i] / 10;
        c.numeric[i] %= 10;
    }


    c.negative = a.negative == b.negative;
    c.Zeros();
    return c;
}

std::istream &operator>>(std::istream &str, BigInteger &a) {
    std::string s;
    str >> s;
    BigInteger b(s);
    a = b;
    return str;
}


std::ostream &operator<<(std::ostream &str, const BigInteger &a) {
    std::string s;
    s = a.toString();
    str << s;
    return str;
}


bool BigInteger::operator<(BigInteger const &b) const {
    return compare(b) == -1;
}


bool BigInteger::operator>(BigInteger const &b) const {
    return compare(b) == 1;
}


bool BigInteger::operator<=(BigInteger const &b) const {
    int comp = compare(b);
    return comp == -1 || comp == 0;
}


bool BigInteger::operator>=(BigInteger const &b) const {
    int comp = compare(b);
    return comp == 1 || comp == 0;
}


bool BigInteger::operator==(BigInteger const &b) const {
    return compare(b) == 0;
}


bool BigInteger::operator!=(BigInteger const &b) const {
    return compare(b) != 0;
}


BigInteger &BigInteger::operator*=(const BigInteger &b) {
    BigInteger rez = *this;
    return *this = rez * b;
}


BigInteger &BigInteger::operator+=(const BigInteger &c) {
    if ((negative && c.negative) || (!negative && !c.negative)) {
        sum(*this, c);
        return *this;
    }
    if ((negative && !c.negative) || (!negative && c.negative)) {
        if (this->compare(c, 0) >= 0) {
            dif(*this, c);
            return *this;
        } else {
            BigInteger tmpa = *this;
            BigInteger tmpb = c;
            dif(tmpb, tmpa);
            return *this = tmpb;
        }
    }
    return *this;
}


void BigInteger::NextNum() {
    numeric.insert(numeric.begin(), 0);
}


BigInteger &BigInteger::operator%=(const BigInteger &b) {
    return *this = *this % b;
}



BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}


const BigInteger BigInteger::operator++(int) {
    BigInteger tmp = *this;
    ++(*this);
    return tmp;
}


BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}


const BigInteger BigInteger::operator--(int) {
    BigInteger tmp = *this;
    --(*this);
    return tmp;
}


BigInteger BigInteger::abs() const {
    BigInteger b = (*this);
    b.negative = true;
    return b;
}


const BigInteger BigInteger::operator-() {
    BigInteger b = *this;
    b.negative = !this->negative;
    if (numeric.size() == 1 && numeric[0] == 0) {
        b.negative = true;
        b.numeric[0] = 0;
        return b;
    } else
        return b;
}


BigInteger &BigInteger::operator-=(const BigInteger &c) {
    if ((negative && c.negative) || (!negative && !c.negative)) {
        if (this->compare(c, 0) >= 0) {
            dif(*this, c);
            return *this;
        } else {
            BigInteger tmpa = *this;
            BigInteger tmpb = c;
            dif(tmpb, tmpa);
            tmpb.negative = !tmpb.negative;
            return *this = tmpb;

        }
    } else {
        sum(*this, c);
        return *this;
    }
}


const std::string BigInteger::toString() const {
    std::string s = negative ? "" : "-";
    if (numeric[0] == 0 && numeric.size() == 1) {
        s = '0';
        return s;
    }
    bool forwNuls = true;
    for (int i = int(numeric.size()) - 1; i >= 0; --i) {
        if (numeric[i] == 0 && forwNuls) {
            continue;
        } else {
            forwNuls = false;
            s += numeric[i] + '0';
        }

    }
    if (s.empty() || (s[0] == '-' && s.length() == 1))
        s = '0';
    return s;
}


BigInteger::operator bool() {
    return *this != 0;
}


void BigInteger::Zeros() {
    while (!numeric.empty() && *numeric.rbegin() == 0) {
        numeric.pop_back();
    }
    if (numeric.empty()) {
        negative = false;
        numeric.push_back(0);
    }
}


void BigInteger::dif(BigInteger &a, const BigInteger &b) {
    int carry = 0;
    for (size_t i = 0; i < b.numeric.size() || carry; ++i) {
        a.numeric[i] -= carry + (i < b.numeric.size() ? b.numeric[i] : 0);
        carry = a.numeric[i] < 0;
        if (carry) a.numeric[i] += 10;
    }
    a.Zeros();
}


void BigInteger::sum(BigInteger &a, const BigInteger &b) {
    int carry = 0;
    for (size_t i = 0; i < std::max(a.numeric.size(), b.numeric.size()) || carry; ++i) {
        if (i == a.numeric.size())
            a.numeric.push_back(0);
        a.numeric[i] += carry + (i < b.numeric.size() ? b.numeric[i] : 0);
        carry = a.numeric[i] / 10;
        a.numeric[i] %= 10;
    }
    a.Zeros();
}


int BigInteger::compare(const BigInteger &a, int k) const {
    int check = 1;
    if (k == 1) {
        if (negative && !a.negative) return 1;
        if (!negative && a.negative) return -1;


        if (!negative && !a.negative) check = -1;
    }

    if (numeric.size() < a.numeric.size()) return -1 * check;
    if (numeric.size() > a.numeric.size()) return check;
    for (size_t i(numeric.size()); i > 0; --i) {
        if (numeric[i - 1] < a.numeric[i - 1]) return -1 * check;
        if (numeric[i - 1] > a.numeric[i - 1]) return check;
    }

    return 0; // ==
}
