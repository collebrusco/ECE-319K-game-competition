// LinAlgTester.cpp
// main to test Mat and Vec classes
// NOT MADE FOR USE IN KEIL
// use regular console as this is outputting tests to console

#include <iostream>
#include "Mat4.h"

using namespace std;

void outMat4(Mat4 mat){
    cout << "| ";
    for (uint32_t i = 0; i < 16; i++){
        if (i%4==0 && i){
            cout << "|\n| ";
        }
        cout << mat.at(i) << " ";
    }
    cout << "|\n";
}

void outVec4(Vec4 vec){
    cout << "( ";
    for (int i = 0; i < 4; i++){
        cout << vec.at(i) << " ";
    }
    cout << ")\n";
}

int main(){
    
    Mat4 imatrix;
    Vec4 vector(10,20,30,40);
    cout << "testing matrix vector multip:\n";
    cout << "multiplying mat:\n";
    outMat4(imatrix);
    cout << "with vector:\n";
    outVec4(vector);
    cout << "result:\n";

    Vec4 result = imatrix * vector;

    outVec4(result);
    cout << "Success! Now testing mat-mat multip:\n";
    Mat4 mmtestmat1;
    Mat4 mmtestmat2;
    Mat4 mmresult;
    int32_t mmt1[16] = {3, 0, 5, 7, 5, 4, 1, 1, 0, 0, 6, 4, 2, 6, 9, 1};
    int32_t mmt2[16] = {8, 5, 7, 3, 4, 4, 9, 9, 0, 0, 0, 3, 2, 6, 0, 0};
    int32_t mmr[16] = {38,57,21,24,58,47,71,54,8,24,0,18,42,40,68,87};
    for (int i = 0; i < 16; i++){
        mmtestmat1.at(i) = mmt1[i];
        mmtestmat2.at(i) = mmt2[i];
        mmresult.at(i) = mmr[i];
    }

    mmresult = mmtestmat1 * mmtestmat2;
    cout << "multiplying mat\n";
    outMat4(mmtestmat1);
    cout << "with\n";
    outMat4(mmtestmat2);
    cout << "result should be:\n";
    outMat4(mmresult);
    cout << "result is:\n";
    outMat4(mmresult);
    
    return 0;
}