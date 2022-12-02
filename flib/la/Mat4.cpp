#include "Mat4.h"
// Mat4.cpp
// Vec = < x, y, z, w >

Mat4::Mat4(){
	for (int i = 0; i < 16; i++){
		data[i] = ((i % 5) == 0); // nice
	}
}

Mat4::Mat4(int32_t all){
	for (int i = 0; i < 16; i++){
		data[i] = all;
	}
}

Mat4::Mat4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4){
	int i = 0;
	for (; i<4; i++){
		data[i] = r1.at(i);
	}
	for (; i<8; i++){
		data[i] = r2.at(i-4);
	}
	for (; i<12; i++){
		data[i] = r3.at(i-8);
	}
	for (; i<16; i++){
		data[i] = r4.at(i-12);
	}
}

int32_t& Mat4::at(uint32_t i){
	return data[i];
}

int32_t& Mat4::at(uint32_t row, uint32_t col){
	return data[(4*row)+col];
}

Vec4 Mat4::col(uint32_t j) const {
	return Vec4(data[j], data[4+j], data[8+j], data[12+j]);
}

Vec4 Mat4::row(uint32_t i) const {
	return Vec4(data[(4*i)], data[(4*i)+1], 
							data[(4*i)+2], data[(4*i)+3]);
}

Mat4 Mat4::operator+(const Mat4& alt) const {
	Mat4 res;
	for(int i = 0; i < 16; i++){
		res.at(i) = data[i] + alt.data[i];
	}
	return res;
}

Mat4 Mat4::operator-(const Mat4& alt) const {
	Mat4 res;
	for(int i = 0; i < 16; i++){
		res.at(i) = data[i] - alt.data[i];
	}
	return res;
}

Mat4 Mat4::operator*(const Mat4& alt) const {
	Mat4 res;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			res.at(i,j) = dot(this->row(i), alt.col(j));
		}
	}
	return res;
}

Mat4 Mat4::operator/(const Mat4& alt) const {
	Mat4 res;
	for(int i = 0; i < 16; i++){
		res.at(i) = data[i] / alt.data[i];
	}
	return res;
}

Mat4 Mat4::operator=(const Mat4& alt) const {
	return Mat4(alt);
}

Vec4 operator*(const Mat4& mat, const Vec4& vec){
	return Vec4(dot(mat.row(0), vec),
							dot(mat.row(1), vec),
							dot(mat.row(2), vec),
							dot(mat.row(3), vec));
}
