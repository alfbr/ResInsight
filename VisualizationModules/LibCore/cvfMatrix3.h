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

namespace cvf {


//==================================================================================================
//
// Template matrix class for 3x3 matrices
//
//==================================================================================================
template<typename S>
class Matrix3
{
public:
    Matrix3();
    Matrix3(const Matrix3& other);
    Matrix3(S m00, S m01, S m02, S m10, S m11, S m12, S m20, S m21, S m22);

    template<typename T>
    explicit Matrix3(const T& other);

    inline Matrix3&     operator=(const Matrix3& rhs);
    bool                operator==(const Matrix3& rhs) const;
    bool                operator!=(const Matrix3& rhs) const;

    const Matrix3       operator*(const Matrix3& rhs) const;

    void                setIdentity();
    bool                isIdentity() const;
    void                setZero();
    bool                isZero() const;

    inline S&           operator()(int row, int col);
    inline S            operator()(int row, int col) const;
    inline void         setRowCol(int row, int col, S value);
    inline S            rowCol(int row, int col) const;

    bool                invert();
    const Matrix3       getInverted(bool* pInvertible = NULL) const;
    S                   determinant() const;
    void                transpose();

    inline const S*     ptr() const;

    static Matrix3      fromRotation(Vector3<S> axis, S angle);

public:
    static const Matrix3 IDENTITY;  ///< Identity matrix
    static const Matrix3 ZERO;      ///< Matrix with all zeros

private:
    // Constants for accessing our internal array using standard matrix notation
    static const int e00 = 0;   static const int e01 = 3;   static const int e02 = 6;
    static const int e10 = 1;   static const int e11 = 4;   static const int e12 = 7;
    static const int e20 = 2;   static const int e21 = 5;   static const int e22 = 8;

private:
    S   m_v[9];        
};

typedef Matrix3<float>  Mat3f;
typedef Matrix3<double> Mat3d;

}

#include "cvfMatrix3.inl"