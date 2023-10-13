#include <iostream>
#include <exception>
#include <assert.h>
#include "matrix.hpp"

Matrix::ProxyRow::ProxyRow() :data(nullptr), size(0) {}
Matrix::ProxyRow::ProxyRow(size_t cols)
{
	data = new int[cols];
	size = cols;
}
Matrix::ProxyRow::~ProxyRow()
{
	destroy();
}
void Matrix::ProxyRow::destroy()
{
	if (data == nullptr) return;
	delete[] data;
	data = nullptr;
	size = 0;
}
void Matrix::ProxyRow::init(size_t size, int data)
{
	if (this->data != nullptr) destroy();
	this->data = new int[size];
	this->size = size;
	for (size_t i = 0; i < size; i++) this->data[i] = data;
}
int& Matrix::ProxyRow::operator [] (const size_t j)
{
	if (j > size) throw std::out_of_range("Out of range");
	return data[j];
}
const int& Matrix::ProxyRow::operator [] (const size_t j) const
{
	if (j > size) throw std::out_of_range("Out of range");
	return data[j];
}

void Matrix::init(int data)
{
	for (size_t i = 0; i < shape[0]; i++) this->data[i].init(shape[1], data);
}
void Matrix::destroy()
{
	if (data == nullptr) return;
	delete[] data;
	data = nullptr;
	shape[0] = 0; shape[1] = 0;
}

Matrix::Matrix()
{
	data = nullptr;
	shape[0] = 0; shape[1] = 0;
}
Matrix::Matrix(size_t s1, size_t s2, int data)
{
	shape[0] = s1; shape[1] = s2;
	this->data = new Matrix::ProxyRow[s1];
	init(data);
}

Matrix::Matrix(const Matrix& other)
{
	shape[0] = other.getRows(); shape[1] = other.getColumns();

	data = new Matrix::ProxyRow[shape[0]];
	init();
	for (size_t i = 0; i < shape[0]; i++) for (size_t j = 0; j < shape[1]; j++) data[i][j] = other[i][j];
}
Matrix::Matrix(Matrix&& other) noexcept : data(other.data)
{
	other.data = nullptr;
	shape[0] = other.shape[0];
	shape[1] = other.shape[1];
}
Matrix::~Matrix()
{
	destroy();
}

size_t Matrix::getRows() const
{
	return shape[0];
}
size_t Matrix::getColumns() const
{
	return shape[1];
}

std::ostream& operator << (std::ostream& os, const Matrix& o)
{
	os << "[";
	for (size_t i = 0; i < o.getRows(); i++)
	{
		os << "[";
		for (size_t j = 0; j < o.getColumns(); j++)
		{
			os << o[i][j] << (j == (o.shape[1] - 1) ? "]" : ", ");
		}
		os << (i == (o.getRows() - 1) ? "]" : ",\n");
	}
	return os;
}

Matrix::ProxyRow& Matrix::operator [] (const size_t i)
{
	if (i > shape[0]) throw std::out_of_range("Out of range");
	return data[i];
}

const Matrix::ProxyRow& Matrix::operator [] (const size_t i) const
{
	if (i > shape[0]) throw std::out_of_range("Out of range");
	return data[i];
}
// оператор сложения
Matrix Matrix::operator + (const Matrix& other) const
{	// сложение возможно только в том случае, если матрицы строго равны по размеру
	if (shape[0] != other.getRows() || shape[1] != other.getColumns()) throw std::runtime_error("invalid shapes");
	// если равны, инициализируем новую матрицу для результата
	Matrix res(shape[0], shape[1]);
	// вычисляем результирующую матрицу
	for (size_t i = 0; i < shape[0]; i++) for (size_t j = 0; j < shape[1]; j++) res[i][j] = data[i][j] + other[i][j];
	return res;
}
// аналогично сложению, после проверки на равенство по размеру поэлементно вычитаем элементы другой матрицы
Matrix Matrix::operator - (const Matrix& other) const
{
	if (shape[0] != other.getRows() || shape[1] != other.getColumns()) throw std::runtime_error("invalid shapes");
	Matrix res(shape[0], shape[1]);
	for (size_t i = 0; i < shape[0]; i++) for (size_t j = 0; j < shape[1]; j++) res[i][j] = data[i][j] - other[i][j];
	return res;
}
// умножение на число
void Matrix::operator *= (const double other)
{	// проходимся и умножаем каждый элемент на число
	for (size_t i = 0; i < shape[0]; i++) for (size_t j = 0; j < shape[1]; j++) data[i][j] *= other;
}
// сравнение на равенство 
bool Matrix::operator == (const Matrix& other) const
{	// если матрицы не равны по размеру -> возваращаем false
	if (shape[0] != other.getRows() || shape[1] != other.getColumns()) return false;
	// дошли до проверки поэлементно (матрицы равны по размеру), если какая то пара не равна -> возращаем false
	for (size_t i = 0; i < shape[0]; i++) for (size_t j = 0; j < shape[1]; j++) if (data[i][j] != other[i][j]) return false;
	// дошли до сюда -> возвращаем true
	return true;
}
// сравнения на неравенство -> возвращаем инвертированный результат проверки на равенство
bool Matrix::operator != (const Matrix& other) const
{
	return !(*this == other);
}
