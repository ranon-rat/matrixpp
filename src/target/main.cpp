#include <iostream>
#include "matrixpp.h++"
void a();
int main()
{

    Matrix m(3, 3);
    m.  fill_rand();
    Matrix m2t=m.transpose();
    std::cout<<m<<"\n";
    std::cout<<m2t<<"\n";
    Matrix subtraction=m-m2t;
    Matrix addition=m+m2t;
    Matrix multiplication_brute=m.brute_force_dot(m2t);
    Matrix multiplication_stressen=m.stressen_dot(m2t);
    std::cout<<"m*m2 brute\n"<<(multiplication_brute)<<"\n";
    std::cout<<"m*m2 stressen\n"<<(multiplication_stressen)<<"\n";
    // normal
    std::cout<<"m*m2 "<<(m*m2t)<<"\n";
    std::cout<<"m+m2 "<<(addition)<<"\n";
    std::cout<<"m-m2 "<<(subtraction)<<"\n";
    return 0;
}
