#include <iostream>
#include <exception>
#include <assert.h>
#include "bigint.hpp"

// деструктор
BigInt::~BigInt() {
    destroy();
}

// конструктор по умолчанию
BigInt::BigInt() : size(1), data(new int32_t[1]), neg(false) {
    data[0] = 0;
}

// конструктор с int32_t
BigInt::BigInt(int32_t n) {
    size = (std::to_string(std::abs(n)).size() + base_len - 1) / base_len;
    data = new int32_t[size];
    data[0] = 0;
    neg = (n < 0);
    size_t i = 0;
    n = std::abs(n);
    while (n > 0) {
        data[i] = n % base;
        n /= base;
        i++;
    }
}

// конструктор с std::string
BigInt::BigInt(const std::string& s) {
    size_t base_len = BigInt::base_len;
    std::string cur;
    if (s.size() > 0 && s[0] == '-') {
        size = (s.size() + base_len - 2) / base_len;
        neg = true;
        cur = s.substr(1);
    } else {
        size = (s.size() + base_len - 1) / base_len;
        neg = false;
        cur = s;
    }
    data = new int32_t[size];
    data[0] = 0;

    for (size_t i = 0; i < size; i++) {
        size_t offset = ((cur.size() > (i + 1) * base_len) ? (cur.size() - (i + 1) * base_len) : 0);
        size_t len = std::min(base_len, cur.size() - i * base_len);
        data[i] = std::stoi(cur.substr(offset, len));
    }
}

// конструктор копирования
BigInt::BigInt(const BigInt& other) : size(other.size), data(new int32_t[other.size]), neg(other.neg) {
    for (size_t i = 0; i < other.size; i++) data[i] = other.data[i];
}

// конструктор перемещения
BigInt::BigInt(BigInt&& other) : size(other.size), neg(other.neg) {
    data = other.data;
    other.data = nullptr;
}

// оператор присваивания (копирование)
BigInt& BigInt::operator=(const BigInt& other) {
    if (this == &other) return *this;
    destroy();
    size = other.size;
    neg = other.neg;
    data = new int32_t[size];
    for (size_t i = 0; i < size; i++) data[i] = other.data[i];
    return *this;
}

// оператор присваивания (перемещение)
BigInt& BigInt::operator=(BigInt&& other) {
    if (this == &other) return *this;
    destroy();
    size = other.size;
    neg = other.neg;
    data = other.data;
    other.data = nullptr;
    return *this;
}

// метод для освобождения памяти в случае ошибок
void BigInt::destroy() {
    delete[] data;
}

// метод для выполнения сложения BigInt как в столбик
BigInt BigInt::add(const BigInt& a, const BigInt& b) {
    std::string ans = "";
    bool carry = false;
    size_t i = 0, j = 0;

	//проход по числам с учетом переноса
    while (i < a.size && j < b.size) {
        int32_t x = a.data[i] + b.data[j] + (carry ? 1 : 0);
        carry = x / base;
        ans = std::to_string(x % base) + ans;
        i++; j++;
    }

	// обработка оставшихся цифр числа а
    for (; i < a.size; i++) {
        int32_t x = a.data[i] + (carry ? 1 : 0);
        carry = x / base;
        ans = std::to_string(x % base) + ans;
    }

	//обработка оставшихся цифр числа b
    for (; j < b.size; j++) {
        int32_t x = b.data[j] + (carry ? 1 : 0);
        carry = x / base;
        ans = std::to_string(x % base) + ans;
    }

	//если остался перенос нужно добавить его
    if (carry) ans = "1" + ans;
    return BigInt(ans);
}

// метод для выполнения вычитания BigInt
BigInt BigInt::subs(const BigInt& a, const BigInt& b) {
    std::string ans = "";
    bool borrow = false;
    size_t i = 0, j = 0;

	// проход но с учетом заема
    while (i < a.size && j < b.size) {
        int32_t x = a.data[i] - b.data[j] - (borrow ? 1 : 0);
        borrow = (x < 0 ? true : false);
        ans = std::to_string((x + base) % base) + ans;
        i++; j++;
    }
	// обработка оставшися цийр а
    for (; i < a.size; i++) {
        int32_t x = a.data[i] - (borrow ? 1 : 0);
        borrow = (x < 0 ? true : false);
        ans = std::to_string((x + base) % base) + ans;
    }

	//обработка оставшихся цифр b
    for (; j < b.size; j++) {
        int32_t x = b.data[j] - (borrow ? 1 : 0);
        borrow = (x < 0 ? true : false);
        ans = std::to_string((x + base) % base) + ans;
    }

	//если остался заем то выбрасываем исключение
    if (borrow) throw std::runtime_error("abs(a) < abs(b)");
    return BigInt(ans);
}

// метод для выполнения умножения BigInt на int32_t
BigInt BigInt::mul(const BigInt& a, const int32_t b, size_t n_zeros) {
    std::string ans = "";
    int32_t carry = 0;
    size_t i = 0;

	// проходимся по а и умножаем на b
    while (i < a.size) {
        int32_t x = a.data[i] + carry;
        x *= b;
        carry = x / base;
        ans = std::to_string(x % base) + ans;
        i++;
    }

	// добавляем перенос
    if (carry) ans = std::to_string(carry) + ans;

	// добавляем нули
    for (size_t i = 0; i < n_zeros; i++) ans += "0";
    return BigInt(ans);
}

// метод для сравнения данных BigInt на равенство
bool BigInt::dataEquals(const BigInt& a, const BigInt& b) {
    if (a.size != b.size) return false;
    for (size_t i = a.size - 1;; i--) {
        if (a.data[i] != b.data[i]) return false;
        if (i == 0) break;
    }
    return true;
}

// метод для сравнения данных BigInt на меньше
bool BigInt::dataLower(const BigInt& a, const BigInt& b) {

	// проверяем размер
    if (a.size < b.size) return true;

	// если равны идем по цифрам
    if (a.size == b.size) {
        for (int32_t i = a.size - 1; i >= 0; i--) {
            if (a.data[i] != b.data[i]) return a.data[i] < b.data[i];
        }
    }

	// а.size >= b.size
    return false;
}

// оператор вывода в поток
std::ostream& operator << (std::ostream& os, const BigInt& o) {
    os << (o.neg ? "-" : "");
    for (size_t i = o.size - 1;; i--) {
        os << o.data[i];
        if (i == 0) break;
    }
    return os;
}

// оператор сложения
BigInt BigInt::operator +(const BigInt& other) const {
    BigInt res;
    if (neg && !other.neg) {
        bool is_negative = dataLower(other, *this);
        res = is_negative ? subs(*this, other) : subs(other, *this);
        res.neg = is_negative;
    } else if (other.neg && !neg) {
        bool is_negative = dataLower(*this, other);
        res = is_negative ? subs(other, *this) : subs(*this, other);
        res.neg = is_negative;
    } else {
        res = add(*this, other);
        res.neg = neg;
    }
    return res;
}

// оператор вычитания
BigInt BigInt::operator -(const BigInt& other) const {
    BigInt c = other;
    c.neg = !c.neg;
    return *this + c;
}

// оператор смены знака
BigInt BigInt::operator-() const {
    BigInt res(*this);
    res.neg = !res.neg;
    return res;
}

// оператор умножения
BigInt BigInt::operator*(const BigInt& other) const {
    BigInt ans(0);
    for (size_t i = 0; i < size; i++) {
        BigInt tmp = mul(other, data[i], i);
        ans = add(ans, tmp);
    }
    BigInt res(ans);
    if (neg ^ other.neg) res.neg = true;
    return res;
}

// оператор сложения с int32_t
BigInt BigInt::operator +(int32_t other) const {
    BigInt b(other);
    return *this + b;
}

// оператор вычитания с int32_t
BigInt BigInt::operator -(int32_t other) const {
    BigInt b(other);
    return *this - b;
}

// оператор сравнения на равенство
bool BigInt::operator==(const BigInt& other) const {
    return (neg == other.neg && dataEquals(*this, other));
}

// оператор сравнения на неравенство
bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

// оператор сравнения на меньше
bool BigInt::operator<(const BigInt& other) const {
    if (*this == other) return false;
    if (neg && !other.neg) return false;
    if (!neg && other.neg) return true;
    if (!neg) return dataLower(*this, other);
    return !dataLower(*this, other);
}

// оператор сравнения на больше
bool BigInt::operator>(const BigInt& other) const {
    if (*this == other) return false;
    return !(*this < other);
}

// оператор сравнения на меньше или равно
bool BigInt::operator<=(const BigInt& other) const {
    return ((*this == other) || (*this < other));
}

// оператор сравнения на больше или равно
bool BigInt::operator>=(const BigInt& other) const {
    return ((*this == other) || (*this > other));
}