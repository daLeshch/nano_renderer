#include <iostream>
#include <cmath>
#include <array>
#include "math_core.h"

using namespace std;

template<size_t N>
void printMatrix(const Matrix<N,N,double>& m) {
    for(int i=0;i<N;++i){
        for(int j=0;j<N;++j) cout << m[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    // ------------------ 2x2 ------------------
    cout << "--- MATRIX 2x2 TESTS ---\n";
    Matrix<2,2,double> m2a{{1,2},{3,4}};
    Matrix<2,2,double> m2b{{5,6},{7,8}};

    cout << "Original m2a:\n"; printMatrix(m2a);
    cout << "Original m2b:\n"; printMatrix(m2b);

    auto m2_add = m2a + m2b;
    auto m2_sub = m2a - m2b;
    auto m2_mul = m2a * m2b;
    auto m2_scalar = m2a * 2.0;

    cout << "m2a + m2b:\n"; printMatrix(m2_add);
    cout << "m2a - m2b:\n"; printMatrix(m2_sub);
    cout << "m2a * m2b:\n"; printMatrix(m2_mul);
    cout << "m2a * 2:\n"; printMatrix(m2_scalar);
    cout << "det(m2a) = " << m2a.det() << "\n\n";

    // ------------------ 3x3 ------------------
    cout << "--- MATRIX 3x3 TESTS ---\n";
    Matrix<3,3,double> m3a{{1,2,3},{0,1,4},{5,6,0}};
    Matrix<3,3,double> m3b{{-2,1,0},{3,0,1},{4,5,6}};

    cout << "Original m3a:\n"; printMatrix(m3a);
    cout << "Original m3b:\n"; printMatrix(m3b);

    auto m3_add = m3a + m3b;
    auto m3_sub = m3a - m3b;
    auto m3_mul = m3a * m3b;
    auto m3_scalar = m3a * 3.0;

    cout << "m3a + m3b:\n"; printMatrix(m3_add);
    cout << "m3a - m3b:\n"; printMatrix(m3_sub);
    cout << "m3a * m3b:\n"; printMatrix(m3_mul);
    cout << "m3a * 3:\n"; printMatrix(m3_scalar);
    cout << "det(m3a) = " << m3a.det() << "\n\n";

    // ------------------ 4x4 ------------------
    cout << "--- MATRIX 4x4 TESTS ---\n";
    Matrix<4,4,double> m4a{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    Matrix<4,4,double> m4b{{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};

    cout << "Original m4a:\n"; printMatrix(m4a);
    cout << "Original m4b:\n"; printMatrix(m4b);

    auto m4_add = m4a + m4b;
    auto m4_sub = m4b - m4a;
    auto m4_mul = m4a * m4b;
    auto m4_scalar = m4b * 0.5;

    cout << "m4a + m4b:\n"; printMatrix(m4_add);
    cout << "m4b - m4a:\n"; printMatrix(m4_sub);
    cout << "m4a * m4b:\n"; printMatrix(m4_mul);
    cout << "m4b * 0.5:\n"; printMatrix(m4_scalar);
    cout << "det(m4b) = " << m4b.det() << "\n\n";

    // ------------------ ROTATION ------------------
    double angle = M_PI/4; // 45 degrees

    cout << "--- ROTATION MATRIX 2D ---\n";
    Matrix<2,2,double> rot2{{cos(angle), -sin(angle)}, {sin(angle), cos(angle)}};
    printMatrix(rot2);

    array<double,2> v2{{1,0}};
    cout << "Original vec2: (" << v2[0] << ", " << v2[1] << ")\n";

    array<double,2> rotated_v2{};
    for(int i=0;i<2;++i){
        rotated_v2[i] = 0;
        for(int j=0;j<2;++j) rotated_v2[i] += rot2[i][j] * v2[j];
    }
    cout << "rotated vec2: (" << rotated_v2[0] << ", " << rotated_v2[1] << ")\n\n";

    cout << "--- ROTATION MATRIX 3D ---\n";
    Matrix<3,3,double> rot3{{cos(angle), -sin(angle),0},
                            {sin(angle), cos(angle),0},
                            {0,0,1}};
    printMatrix(rot3);

    array<double,3> v3{{1,0,0}};
    cout << "Original vec3: (" << v3[0] << ", " << v3[1] << ", " << v3[2] << ")\n";

    array<double,3> rotated_v3{};
    for(int i=0;i<3;++i){
        rotated_v3[i] = 0;
        for(int j=0;j<3;++j) rotated_v3[i] += rot3[i][j] * v3[j];
    }
    cout << "rotated vec3: (" << rotated_v3[0] << ", " << rotated_v3[1] << ", " << rotated_v3[2] << ")\n\n";

    vec4d vector1(1, 2, 3, 4);
    cout << "orig vec: " << vector1.x <<  ", " << vector1.y <<  ", " <<vector1.z <<  ", " << vector1.w << "\n";
    vector1 = m4b * vector1;
    cout << "matrix: " << "\n";
    printMatrix(m4b);
    cout << "mult vec: " << vector1.x <<  ", " << vector1.y <<  ", " <<vector1.z <<  ", " << vector1.w << "\n";

    
// orig: 0.103434,0.63749,0.102854
// after view: 0.103434,0.63749,0.102854,0.691438
// after persp: 0.103434,0.63749,0.102854,0.588584
// orig: 0.120409,0.670105,0.118388
// after view: 0.120409,0.670105,0.118388,0.644836
// after persp: 0.120409,0.670105,0.118388,0.526448
// orig: 0.113204,0.661161,0.119513
// after view: 0.113204,0.661161,0.119513,0.641461
// after persp: 0.113204,0.661161,0.119513,0.521948
// input screen: 0.103434,0.63749,0.102854
// output screen: -4.73921e+08,-1.55697e+08,140.614
// input screen: 0.120409,0.670105,0.118388
// output screen: -4.81212e+08,-1.41689e+08,142.594
// input screen: 0.113204,0.661161,0.119513
// output screen: -4.78117e+08,-1.4553e+08,142.738

// screen matrix:
// 400 0 0 400
// 0 -400 0 400
// 0 0 127.5 127.5
// 0 0 0 1








    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();

    return 0;
}
