#pragma once
#include <iostream>

class BigInt
{
private:
	size_t size;
	int32_t* data;
	bool neg;
private:
	// метод освобождения памяти
	void destroy();

	//статические методы выполнения арифметических операций
	static BigInt add(const BigInt& a, const BigInt& b);
	static BigInt subs(const BigInt& a, const BigInt& b);
	static BigInt mul(const BigInt& a, const int32_t b, size_t n_zeros = 0);

	// методы для сравнения больших чисел на равенство и на меньше для больше и меньше
	static bool dataEquals(const BigInt& a, const BigInt& b);
	static bool dataLower(const BigInt& a, const BigInt& b);
public:
	// константы для прставления чисел в системе счисления с фиксированной базой
	static const size_t base = 10000;
	static const size_t base_len = 4;
public:
	// конструкторы и деструктор
	BigInt();
	BigInt(int32_t n);
	BigInt(const std::string& s);
	BigInt(const BigInt& other);
	BigInt(BigInt&& other);
	~BigInt();

	//операторы присваивания и вывода
	BigInt& operator=(const BigInt& other);
	BigInt& operator=(BigInt&& other);
	friend std::ostream& operator << (std::ostream& os, const BigInt& o);

	//арифм операторы
	BigInt operator +(const BigInt& other) const;
	BigInt operator -(const BigInt& other) const;
	BigInt operator-() const;
	BigInt operator*(const BigInt& other) const;
	BigInt operator +(int32_t other) const;
	BigInt operator -(int32_t other) const;

	// операторы сравнения
	bool operator==(const BigInt& other) const;
	bool operator !=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;
};