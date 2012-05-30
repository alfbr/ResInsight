//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2012 Ceetron AS
//    
//   This library is free software: you can redistribute it and/or modify 
//   it under the terms of the GNU General Public License as published by 
//   the Free Software Foundation, either version 3 of the License, or 
//   (at your option) any later version. 
//    
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY 
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
//   FITNESS FOR A PARTICULAR PURPOSE.   
//    
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>> 
//   for more details. 
//
//##################################################################################################

#pragma once

#include "cvfSystem.h"
#include "cvfVector3.h"
#include "cvfVector4.h"
#include "cvfMatrix3.h"


namespace cvf {


//==================================================================================================
//
// Template matrix class for 4x4 matrices
//
//==================================================================================================
template<typename S>
class Matrix4
{
public:
    Matrix4();
    Matrix4(const Matrix4& other);
    Matrix4(S m00, S m01, S m02, S m03, S m10, S m11, S m12, S m13, S m20, S m21, S m22, S m23, S m30, S m31, S m32, S m33);

    explicit Matrix4(const Matrix3<S>& other);

    template<typename T>
    explicit Matrix4(const T& other);

    inline Matrix4&     operator=(const Matrix4& rhs);
    bool                operator==(const Matrix4& rhs) const;
    bool                operator!=(const Matrix4& rhs) const;

    const Matrix4       operator*(const Matrix4& rhs) const;
    const Vector4<S>    operator*(const Vector4<S>& rhs) const;

    void                setIdentity();
    bool                isIdentity() const;
    void                setZero();
    bool                isZero() const;

    inline S&           operator()(int row, int col);
    inline S            operator()(int row, int col) const;
    inline void         setRowCol(int row, int col, S value);
    inline S            rowCol(int row, int col) const;

    void                setRow(int row, const Vector4<S>& vector);
    Vector4<S>          row(int row) const;
    void                setCol(int column, const Vector4<S>& vector);
    Vector4<S>          col(int column) const;

    template<typename T>
    void                set(const Matrix4<T>& mat);

    Vector3<S>          translation() const;
    void                setTranslation(const Vector3<S>& trans);
    void                translatePreMultiply(const Vector3<S>& trans);
    void                translatePostMultiply(const Vector3<S>& trans);

    bool                invert();
    const Matrix4       getInverted(bool* pInvertible = NULL) const;
    S                   determinant() const;
    void                transpose();
    const Matrix4       getTransposed() const;

	void				setFromMatrix3(const Matrix3<S>& mat3);
	Matrix3<S>          toMatrix3() const;
	void				toMatrix3(Matrix3<S>* mat3) const;

    inline const S*     ptr() const;

    static Matrix4      fromTranslation(const Vector3<S>& trans);
    static Matrix4      fromRotation(Vector3<S> axis, S angle);
    static Matrix4      fromCoordSystemAxes(const Vector3<S>* xAxis, const Vector3<S>* yAxis, const Vector3<S>* zAxis);

public:
    static const Matrix4 IDENTITY;  ///< Identity matrix
    static const Matrix4 ZERO;      ///< Matrix with all zeros

private:
    // Constants for accessing our internal array using standard matrix notation
    static const int e00 = 0;   static const int e01 = 4;   static const int e02 = 8;   static const int e03 = 12;
    static const int e10 = 1;   static const int e11 = 5;   static const int e12 = 9;   static const int e13 = 13;
    static const int e20 = 2;   static const int e21 = 6;   static const int e22 = 10;  static const int e23 = 14;
    static const int e30 = 3;   static const int e31 = 7;   static const int e32 = 11;  static const int e33 = 15;

private:
    S   m_v[16];        
};

template<typename S>
Vector4<S> operator*(const Matrix4<S>& m, const Vector4<S>& v);

typedef Matrix4<float>  Mat4f;
typedef Matrix4<double> Mat4d;

}

#include "cvfMatrix4.inl"