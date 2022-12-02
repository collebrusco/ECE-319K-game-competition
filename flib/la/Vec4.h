// Vec4.h
// Vector of 32 bit signed integers in R^4
// +, *, /, and = operators defined
// note '*' operator is element wise multiplication,
// use dot(Vec4, Vec4) for dot product
// '/' operator is element wise division
// matrix-vector multiplication is supported with '*'
// that's defined in a different file

#include <stdint.h>

//macros for std basis vecs
#define E_1 1,0,0,0
#define E_2 0,1,0,0
#define E_3 0,0,1,0
#define E_4 0,0,0,1

class Vec4 {
	private:
		int32_t data[4];
	public:
		//data
		int32_t& x;
		int32_t& y;
		int32_t& z;
		int32_t& w;
		//funcs
		Vec4(); 
		Vec4(int32_t all);
		Vec4(int32_t, int32_t, int32_t, int32_t); 
		//index access for iterating
		int32_t at(uint32_t i);
		//vector-scalar operators
		Vec4 operator+(const int32_t& n) const;
		Vec4 operator-(const int32_t& n) const;
		Vec4 operator*(const int32_t& n) const;
		Vec4 operator/(const int32_t& n) const;
		Vec4 operator=(const int32_t& n) const;
		//vector-vector operators
		Vec4 operator+(const Vec4& alt) const;
		Vec4 operator-(const Vec4& alt) const;
		Vec4 operator*(const Vec4& alt) const;
		Vec4 operator/(const Vec4& alt) const;
		Vec4 operator=(const Vec4& alt) const;
};

//dot product
int32_t dot(const Vec4& a, const Vec4& b);
