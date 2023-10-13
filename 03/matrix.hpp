#pragma once
#include <iostream>
#include <exception>

class Matrix
{
public:
	class ProxyRow
	{
	private:
		int* data;
		size_t size;
	public:
		ProxyRow();
		ProxyRow(size_t cols);
		~ProxyRow();
		void destroy();
		void init(size_t size, int data = 0);
		int& operator [] (const size_t j);
		const int& operator [] (const size_t j) const;
	};
private:
	ProxyRow* data;
	size_t shape[2];
private:
	void init(int data = 0);
	void destroy();
public:
	Matrix();
	Matrix(size_t s1, size_t s2, int data = 0);

	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix();

	size_t getRows() const;
	size_t getColumns() const;

	// инициализируем операторы
	friend std::ostream& operator << (std::ostream& os, const Matrix& o);

	ProxyRow& operator [] (const size_t i);

	const ProxyRow& operator [] (const size_t i) const;

	Matrix operator + (const Matrix& other) const;

	Matrix operator - (const Matrix& other) const;

	void operator *= (const double other);

	bool operator == (const Matrix& other) const;

	bool operator != (const Matrix& other) const;
};