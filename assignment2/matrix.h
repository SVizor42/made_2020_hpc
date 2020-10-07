#ifndef matrix_h
#define matrix_h

#include <vector>
#include <iostream>

class Matrix {
public:
	class Proxy {
	public:
		Proxy(std::vector<int> row) : row_(row) {};
		int& operator[](const size_t index);
		const int operator[](const size_t index) const;
		size_t getSize() const;

	private:
		std::vector<int> row_;
	};

	Matrix(size_t rows, size_t cols, int value = 0) : data_(rows, Proxy(std::vector<int> (cols, 0))) {};
	size_t getRows() const;
	size_t getColumns() const;
	Proxy& operator[](const size_t index);
	const Proxy& operator[](const size_t index) const;
	Matrix& operator*=(const int value);
	Matrix operator*(const Matrix& other) const;
	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;
	
private:
	std::vector<Proxy> data_;
};

#endif
