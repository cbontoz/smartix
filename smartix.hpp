/**
 * \file smartix.hpp
 *
 * \brief A library to assist matrix allocation and perform cell-wise
 * calculations between matrices or calculations between matrices and
 * scalar values.
 *
 * MIT License
 *
 * Copyright (c) 2017 Christos Bontozoglou
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * \author Christos Bontozoglou
 * \email cbontoz@gmail.com
 */

#ifndef _smartix_HPP_
#define _smartix_HPP_

#include <cassert>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

template<typename To, typename From> To container_cast(From &&from) {
	using std::begin;
	using std::end;
	return To(begin(from), end(from));
}

/*! \brief Simple Matrix allocation and access class.
 */
template<typename T> class smartix {
private:
	std::vector<size_t> _dim; //!< Holds size of all dimensions
	std::vector<T> _full;     //!< Matrix

public:
	smartix(smartix<T> const &src);
	template<typename R> smartix(smartix<R> const &src);
	smartix(std::vector<size_t> Dimensions);
	smartix(size_t width, size_t height);
	smartix(size_t width, size_t height, size_t depth);
	smartix(size_t width, size_t height, size_t depth, size_t cube);
	~smartix();

	smartix<T> &operator=(smartix<T> const &copyme);
	template<typename R> smartix<T> &operator=(smartix<R> const &copyme);
	template<typename R> smartix<T> &operator=(R const &copyme);

	template<typename R> smartix<T> &operator+=(smartix<R> const &addend);
	template<typename R> smartix<T> &operator-=(smartix<R> const &subtrahend);
	template<typename R> smartix<T> &operator*=(smartix<R> const &multiplicand);
	template<typename R> smartix<T> &operator/=(smartix<R> const &divisor);

	template<typename R> smartix<T> &operator+=(R const &addend);
	template<typename R> smartix<T> &operator-=(R const &subtrahend);
	template<typename R> smartix<T> &operator*=(R const &multiplicand);
	template<typename R> smartix<T> &operator/=(R const &divisor);

	template<typename R1> smartix<T> operator+(smartix<R1> const &addend);
	template<typename R1> smartix<T> operator-(smartix<R1> const &subtrahend);
	template<typename R1> smartix<T> operator*(smartix<R1> const &multiplicand);
	template<typename R1> smartix<T> operator/(smartix<R1> const &divisor);

	template<typename R1> smartix<T> operator+(R1 const &addend);
	template<typename R1> smartix<T> operator-(R1 const &subtrahend);
	template<typename R1> smartix<T> operator*(R1 const &multiplicand);
	template<typename R1> smartix<T> operator/(R1 const &divisor);

	template<typename L>
	friend smartix<T> operator+(L const &lhand, smartix<T> const &src) {
		smartix<T> toret(src.getDimensions());
		toret = lhand;
		toret += src;
		return toret;
	}

	template<typename L>
	friend smartix<T> operator-(L const &lhand, smartix<T> const &src) {
		smartix<T> toret(src.getDimensions());
		toret = lhand;
		toret -= src;
		return toret;
	}

	template<typename L>
	friend smartix<T> operator*(L const &lhand, smartix<T> const &src) {
		smartix<T> toret(src.getDimensions());
		toret = lhand;
		toret *= src;
		return toret;
	}

	template<typename L>
	friend smartix<T> operator/(L const &lhand, smartix<T> const &src) {
		smartix<T> toret(src.getDimensions());
		toret = lhand;
		toret /= src;
		return toret;
	}

	T &uacc(size_t loc);
	T uacc_const(size_t loc) const;
	std::vector<size_t> getDimensions() const;
	std::vector<T> getData() const;
};

template<typename T> T &smartix<T>::uacc(size_t loc) {
	return _full.data()[loc];
}

template<typename T> T smartix<T>::uacc_const(size_t loc) const {
	return _full.data()[loc];
}

// Constructors/Destructors
template<typename T>
smartix<T>::smartix(smartix<T> const &src) :
		_dim(src.getDimensions()), _full(src.getData()) {
}

template<typename T>
template<typename R>
smartix<T>::smartix(smartix<R> const &src) :
		_dim(src.getDimensions()) {
	_full.clear();
	size_t total = src.getData().size();
	_full.resize(total);
	_full = container_cast<std::vector<T> >(src.getData());
}

template<typename T>
smartix<T>::smartix(std::vector<size_t> Dimensions) :
		_dim(Dimensions) {
	size_t total = _dim[0];
	for (size_t i = 1; i < _dim.size(); ++i) {
		total *= _dim[i];
	}
	_full.resize(total);
}

template<typename T>
smartix<T>::smartix(size_t width, size_t height) :
		_dim(2) {
	_full.resize(width * height);
}

template<typename T>
smartix<T>::smartix(size_t width, size_t height, size_t depth) :
		_dim(3) {
	_full.resize(width * height * depth);
}

template<typename T>
smartix<T>::smartix(size_t width, size_t height, size_t depth, size_t cube) :
		_dim(4) {
	_full.resize(width * height * depth * cube);
}

template<typename T> smartix<T>::~smartix() {
}

// Operators
template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator+=(smartix<R> const &addend) {
	assert(_dim == addend.getDimensions());
	for (size_t i = 0; i < _full.size(); ++i) {
		uacc(i) += addend.uacc_const(i);
	}
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator-=(smartix<R> const &subtrahend) {
	assert(_dim == subtrahend.getDimensions());
	for (size_t i = 0; i < _full.size(); ++i) {
		uacc(i) -= subtrahend.uacc_const(i);
	}
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator*=(smartix<R> const &multiplicand) {
	assert(_dim == multiplicand.getDimensions());
	for (size_t i = 0; i < _full.size(); ++i) {
		uacc(i) *= multiplicand.uacc_const(i);
	}
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator/=(smartix<R> const &divisor) {
	assert(_dim == divisor.getDimensions());
	for (size_t i = 0; i < _full.size(); ++i) {
		uacc(i) /= divisor.uacc_const(i);
	}
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator+=(R const &addend) {
	typename std::vector<T>::iterator it;
	for (it = _full.begin(); it != _full.end(); ++it)
		*it += addend;
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator-=(R const &subtrahend) {
	typename std::vector<T>::iterator it;
	for (it = _full.begin(); it != _full.end(); ++it)
		*it -= subtrahend;
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator*=(R const &multiplicand) {
	typename std::vector<T>::iterator it;
	for (it = _full.begin(); it != _full.end(); ++it)
		*it *= multiplicand;
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator/=(R const &divisor) {
	typename std::vector<T>::iterator it;
	for (it = _full.begin(); it != _full.end(); ++it)
		*it /= divisor;
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator=(smartix<R> const &copyme) {
	assert(_dim == copyme.getDimensions());
	for (size_t i = 0; i < _full.size(); ++i)
		uacc(i) = copyme.uacc_const(i);
	return *this;
}

template<typename T>
smartix<T> &smartix<T>::operator=(smartix<T> const &copyme) {
	assert(_dim == copyme.getDimensions());
	_full = copyme.getData();
	return *this;
}

template<typename T>
template<typename R>
smartix<T> &smartix<T>::operator=(R const &copyme) {
	std::fill(_full.begin(), _full.end(), copyme);
	return *this;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator+(smartix<R1> const &addend) {
	assert(_dim == addend.getDimensions());
	smartix<T> toret = *this;
	toret += addend;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator-(smartix<R1> const &subtrahend) {
	assert(_dim == subtrahend.getDimensions());
	smartix<T> toret = *this;
	toret -= subtrahend;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator*(smartix<R1> const &multiplicand) {
	assert(_dim == multiplicand.getDimensions());
	smartix<T> toret = *this;
	toret *= multiplicand;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator/(smartix<R1> const &divisor) {
	assert(_dim == divisor.getDimensions());
	smartix<T> toret = *this;
	toret /= divisor;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator+(R1 const &addend) {
	smartix<T> toret = *this;
	toret += addend;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator-(R1 const &subtrahend) {
	smartix<T> toret = *this;
	toret -= subtrahend;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator*(R1 const &multiplicand) {
	smartix<T> toret = *this;
	toret *= multiplicand;
	return toret;
}

template<typename T>
template<typename R1>
smartix<T> smartix<T>::operator/(R1 const &divisor) {
	smartix<T> toret = *this;
	toret /= divisor;
	return toret;
}

template<typename T> std::vector<size_t> smartix<T>::getDimensions() const {
	std::vector<size_t> ret(_dim.begin(), _dim.end());
	std::copy(_dim.begin(), _dim.end(), ret.begin());
	return ret;
}

template<typename T> std::vector<T> smartix<T>::getData() const {
	std::vector<T> ret(_full.size());
	ret = _full;
	return ret;
}

#endif //_smartix_HPP_
