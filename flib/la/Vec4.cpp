#include "Vec4.h"
// Vec4.cpp
// Vec = < x, y, z, w >

Vec4::Vec4() : x(data[0]), y(data[1]), z(data[2]), w(data[3]){
	x=0;
	y=0;
	z=0;
	w=0;
}

Vec4::Vec4(int32_t all) : x(data[0]), y(data[1]), z(data[2]), w(data[3]){
	x = all;
	y = all;
	z = all;
	w = all;
}

Vec4::Vec4(int32_t xn, int32_t yn, int32_t zn, int32_t wn)
					: x(data[0]), y(data[1]), z(data[2]), w(data[3]){
	x = xn;
	y = yn;
	z = zn;
	w = wn;
}

int32_t Vec4::at(uint32_t i){
	if (i>3){return 0xFFFFFFFF;}
	return data[i];
}

Vec4 Vec4::operator+(const Vec4& alt) const {
	return Vec4(x + alt.x, y + alt.y, z + alt.z, w + alt.w);
}

Vec4 Vec4::operator-(const Vec4& alt) const {
	return Vec4(x - alt.x, y - alt.y, z - alt.z, w - alt.w);
}

Vec4 Vec4::operator*(const Vec4& alt) const {
	return Vec4(x * alt.x, y * alt.y, z * alt.z, w * alt.w);
}

Vec4 Vec4::operator/(const Vec4& alt) const {
	return Vec4(x / alt.x, y / alt.y, z / alt.z, w / alt.w);
}

Vec4 Vec4::operator=(const Vec4& alt) const {
	return Vec4(alt);
}

Vec4 Vec4::operator+(const int32_t& n) const {
	return Vec4(x + n, y + n, z + n, w + n);
}

Vec4 Vec4::operator-(const int32_t& n) const {
	return Vec4(x - n, y - n, z - n, w - n);
}

Vec4 Vec4::operator*(const int32_t& n) const {
	return Vec4(x * n, y * n, z * n, w * n);
}

Vec4 Vec4::operator/(const int32_t& n) const {
	return Vec4(x / n, y / n, z / n, w / n);
}

Vec4 Vec4::operator=(const int32_t& n) const {
	return Vec4(n);
}

int32_t dot(const Vec4& a, const Vec4& b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}
