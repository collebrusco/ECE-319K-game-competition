// Mat4.h
// 4x4 Matrix of 32 bit signed integers
// +, *, /, and = operators defined
// +, -, and / are all element wise
// * is proper matrix multiplication from linear algebra
// matrix-vector multiplication is supported with '*'
// matrix is stored row major internally

#include <stdint.h>
#include "Vec4.h"

class Mat4 {
	private:
		int32_t data[16];
	public:
		Mat4(); // initialize to identity matrix
		Mat4(int32_t all);
		Mat4(Vec4, Vec4, Vec4, Vec4); 
		//access, these DO NOT BOUNDS CHECK for speed, be careful
		int32_t& at(uint32_t i);
		int32_t& at(uint32_t row, uint32_t col);
		Vec4 col(uint32_t j) const;
		Vec4 row(uint32_t i) const;
		//matrix-matrix operators
		Mat4 operator+(const Mat4& alt) const;
		Mat4 operator-(const Mat4& alt) const;
		Mat4 operator*(const Mat4& alt) const;
		Mat4 operator/(const Mat4& alt) const;
		Mat4 operator=(const Mat4& alt) const;
};

Vec4 operator*(const Mat4& mat, const Vec4& vec);
