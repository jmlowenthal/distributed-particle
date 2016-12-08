// Copyright (c) 2016 James Lowenthal - all rights reserved

#pragma once

#include <exception>
#include <initializer_list>

typedef unsigned int uint;

/**
	@brief		n x m matrix
	@param n	number of rows
	@param m	number of columns
	@param T	data type of values
	A template class for representing mathematical matricies.
*/
template<uint n, uint m, typename T = float>
class Matrix {
private:
	/**
		@brief		matrix internal data
		Internal array for holding matrix data values.
	*/
	T data[n][m];

public:
	/**
		@brief		basic matrix constructor
		Returns a zero-initialised matrix.
	*/
	Matrix();

	/**
		@brief		initialiser list constructor
		@param l	initialiser list
		Produces a matrix of correct dimensions given `n x m` items in the
		list.
	*/
	Matrix(std::initializer_list<T> l);

	/**
		@brief		matrix accessor
		@param i	row
		@param j	column
		@return		reference to value within matrix array
		Accessor for getting a location in the matrix by reference. Index
		starts from one (rather than zero) as is convention in matrices. Second
		parameter default allows easier access to single-column matricies
		(vectors).
	*/
	T& at(uint i, uint j = 1);

	/**
		@brief		matrix accessor
		@param i	row
		@param j	column
		@return		constant value from matrix array
		Accessor for getting a location in the constant matrix. Index
		starts from one (rather than zero) as is convention in matrices. Second
		parameter default allows easier access to single-column matricies
		(vectors).
	*/
	const T& at(uint i, uint j = 1) const;

	/**
		@brief		matrix accessor
		@param i	row (zero-indexed)
		@param j	column (zero-indexed)
		@return		reference to value within matrix array
		Accessor for getting a location in the matrix by reference. Index
		starts from zero, by programming convention. Second parameter default
		allows easier access to single-column matricies (vectors).
	*/
	T& at_(uint i, uint j = 0);

	/**
		@brief		matrix accessor
		@param i	row (zero-indexed)
		@param j	column (zero-indexed)
		@return		constant value within matrix array
		Accessor for getting a location in the constant matrix. Index
		starts from zero, by programming convention. Second parameter default
		allows easier access to single-column matricies (vectors).
	*/
	const T& at_(uint i, uint j = 0) const;

	/**
		@brief		matrix transpose
		@return		transposed matrix
		Transpose rows with columns of matrix.
	*/
	Matrix<m, n, T> transpose();

	/**
		@brief		produce a resized matrix
		@return		a matrix of specified size
		Produce of a matrix of given size with values copied from their
		respective positions in the original matrix.
	*/
	template<uint a, uint b> Matrix<a, b, T> resize();

	/**
		@brief		produce negated matrix
	*/
	Matrix<n, m, T> operator-();

	/**
		@brief		matrix multiplication
		@param rhs	matrix to multiply by
		@return		resulting matrix
	*/
	template<uint k> Matrix<n, k, T> operator*(Matrix<m, k, T>& rhs);

	/**
		@brief		matrix addition
		@param rhs	matrix to add
		Adds each element of this matrix to the element in the corresponding
		location in the `rhs` matrix.
	*/
	Matrix<n, m, T> operator+(const Matrix<n, m, T>& rhs) const;
	Matrix<n, m, T>& operator+=(const Matrix<n, m, T>& rhs);

	/**
		@brief		matrix subtraction
		@param rhs	matrix to subtract
		Subtracts each element of the `rhs` element from the corresponding
		location in this matrix.
	*/
	Matrix<n, m, T> operator-(const Matrix<n, m, T>& rhs) const;
	Matrix<n, m, T>& operator-=(const Matrix<n, m, T>& rhs);

	/**
		@brief		matrix scaling
		@param rhs	scaling parameter
		Multiply each element of this matrix by the scaling parameter.
	*/
	Matrix<n, m, T> operator*(const T& rhs) const;
	Matrix<n, m, T>& operator*=(const T& rhs);

	/**
		@brief		matrix scaling
		@param rhs	scaling parameter
		Divide each element of this matrix by the scaling parameter.
	*/
	Matrix<n, m, T> operator/(const T& rhs) const;
	Matrix<n, m, T>& operator/=(const T& rhs);

	/**
		@brief		identity matrix
		@return		identity matrix for template dimensions
		Produces the identity matrix. This function is only defined for square
		matricies.
	*/
	static Matrix<n, m, T> identity();
};

template<unsigned int n, typename T = float>
using Vector = Matrix<n, 1, T>;

#pragma region Function definitions

template<uint n, uint m, typename T>
inline Matrix<n, m, T>::Matrix() {
	for (int i = 0; i < n * m; ++i) {
		//at_(i) = 0;
		new(&at_(0, i)) T();
	}
	/*for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			at_(i, j) = 0;
		}
	}*/
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T>::Matrix(std::initializer_list<T> l) {
#ifdef _DEBUG
	if (l.size() != n * m)
		throw std::exception("Initialiser list is not the correct size");
#endif
	uint i = 0, j = 0;
	for (const T* itr = l.begin(); itr != l.end() && j < m; ++itr) {
		at_(i, j) = *itr;
		++i;
		if (i >= n) {
			i = 0;
			++j;
		}
	}
}

template<uint n, uint m, typename T>
inline T& Matrix<n, m, T>::at(uint i, uint j) {
	return data[--i][--j];
}

template<uint n, uint m, typename T>
inline const T& Matrix<n, m, T>::at(uint i, uint j) const {
	return data[--i][--j];
}

template<uint n, uint m, typename T>
inline T& Matrix<n, m, T>::at_(uint i, uint j) {
	return data[i][j];
}

template<uint n, uint m, typename T>
inline const T& Matrix<n, m, T>::at_(uint i, uint j) const {
	return data[i][j];
}

template<uint n, uint m, typename T>
inline Matrix<m, n, T> Matrix<n, m, T>::transpose() {
	Matrix<m, n, T> ret;
	for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			ret.at_(j, i) = at_(i, j);
		}
	}
	return ret;
}

template<uint n, uint m, typename T>
template<uint a, uint b>
inline Matrix<a, b, T> Matrix<n, m, T>::resize() {
	Matrix<a, b, T> ret;
	for (int i = 0; i < n * m; ++i) {
		ret.at_(0, i) = at_(0, i);
	}
	/*for (uint i = 0; i < n && i < a; ++i) {
		for (uint j = 0; j < m && j < b; ++j) {
			ret.at_(i, j) = at_(i, j);
		}
	}*/
	return ret;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T> Matrix<n, m, T>::operator-() {
	Matrix<n, m, T> ret;
	for (int i = 0; i < n * m; ++i) {
		ret.at_(0, i) = at_(0, i);
	}
	/*for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			ret.at_(i, j) = -at_(i, j);
		}
	}*/
	return ret;
}

template<uint n, uint m, typename T>
template<uint k>
inline Matrix<n, k, T> Matrix<n, m, T>::operator*(Matrix<m, k, T>& rhs) {
	Matrix<n, k, T> ret;
	for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < k; ++j) {
			for (uint l = 0; l < m; ++l) {
				ret.at_(i, j) += at_(i, l) * rhs.at_(l, j);
			}
		}
	}
	return ret;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T> Matrix<n, m, T>::operator+(const Matrix<n, m, T>& rhs) const {
	Matrix<n, m, T> A;
	for (int i = 0; i < n * m; ++i) {
		A.at_(0, i) = at_(0, i) + rhs.at_(0, i);
	}
	/*for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			A.at_(i, j) = at_(i, j) + rhs.at_(i, j);
		}
	}*/
	return A;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T>& Matrix<n, m, T>::operator+=(const Matrix<n, m, T>& rhs) {
	*this = *this + rhs;
	return *this;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T> Matrix<n, m, T>::operator-(const Matrix<n, m, T>& rhs) const {
	Matrix<n, m, T> A;
	for (int i = 0; i < n * m; ++i) {
		A.at_(0, i) = at_(0, i) - rhs.at_(0, i);
	}
	/*for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			A.at_(i, j) = at_(i, j) - rhs.at_(i, j);
		}
	}*/
	return A;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T>& Matrix<n, m, T>::operator-=(const Matrix<n, m, T>& rhs) {
	*this = *this - rhs;
	return *this;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T> Matrix<n, m, T>::operator*(const T& rhs) const {
	Matrix<n, m, T> A;
	for (int i = 0; i < n * m; ++i) {
		A.at_(0, i) = at_(0, i) * rhs;
	}
	/*for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			A.at_(i, j) = at_(i, j) * rhs;
		}
	}*/
	return A;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T>& Matrix<n, m, T>::operator*=(const T& rhs) {
	*this = *this * rhs;
	return *this;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T> Matrix<n, m, T>::operator/(const T& rhs) const {
	Matrix<n, m, T> A;
	for (int i = 0; i < n * m; ++i) {
		A.at_(0, i) = at_(0, i) / rhs;
	}
	/*for (uint i = 0; i < n; ++i) {
		for (uint j = 0; j < m; ++j) {
			A.at_(i, j) = at_(i, j) / rhs;
		}
	}*/
	return A;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T>& Matrix<n, m, T>::operator/=(const T& rhs) {
	*this = *this / rhs;
	return *this;
}

template<uint n, uint m, typename T>
inline Matrix<n, m, T> Matrix<n, m, T>::identity() {
	static_assert(n == m, "Identity matrix is only defined for a square matrices");
	static Matrix<n, m, T> I;
	if (I.at_(0, 0) == 0) {
		for (uint i = 0; i < n; ++i) {
			I.at_(i, i) = 1;
		}
	}
	return I;
}

#pragma endregion