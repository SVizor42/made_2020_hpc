#include "matrix.h"
#include <stdexcept>

int& Matrix::Proxy::operator[](const size_t index) {
	if (index >= row_.size()) {
		throw std::out_of_range("Column index is out of range!");
	}
	else {
		return row_[index];
	}
}

const int Matrix::Proxy::operator[](const size_t index) const {
	if (index >= row_.size()) {
		throw std::out_of_range("Column index is out of range!");
	}
	else {
		return row_[index];
	}
}

size_t Matrix::Proxy::getSize() const {
	return row_.size();
}

size_t Matrix::getRows() const {
	return data_.size();
}

size_t Matrix::getColumns() const {
	return data_[0].getSize();
}

Matrix::Proxy& Matrix::operator[](const size_t index) {
	if (index >= data_.size()) {
		throw std::out_of_range("Row index is out of range!");
	}
	else {
		return data_[index];
	}
}

const Matrix::Proxy& Matrix::operator[](const size_t index) const {
	if (index >= data_.size()) {
		throw std::out_of_range("Row index is out of range!");
	}
	else {
		return data_[index];
	}
}

Matrix& Matrix::operator*=(const int value) {
	for (size_t i = 0; i < getRows(); i++)
		for (size_t j = 0; j < getColumns(); j++) {
			data_[i][j] *= value;
		}
}

Matrix Matrix::operator*(const Matrix& other) const {
	Matrix matmul(getRows(), other.getColumns());
	for (size_t i = 0; i < matmul.getRows(); i++)
		for (size_t j = 0; j < matmul.getColumns(); j++)
			for (size_t k = 0; k < getColumns(); k++) {
				matmul[i][j] += data_[i][k] * other[k][j];
			}

	return matmul;
}

bool Matrix::operator==(const Matrix& other) const {
	if (this == &other) {
		return true;
	}

	if (getRows() != other.getRows() || getColumns() != other.getColumns()) {
		return false;
	}

	for (size_t i = 0; i < getRows(); i++)
		for (size_t j = 0; j < getColumns(); j++)
			if (data_[i][j] != other.data_[i][j]) {
				return false;
			}

	return true;
}

bool Matrix::operator!=(const Matrix& other) const {
	return !(*this == other);
}
