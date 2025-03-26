#include <iostream>
#include <chrono>

#include "matrixpp.h++"

int main()
{

    
    Matrix m(200, 200);
    m.  fill_rand();
    Matrix m2t=m.transpose();

 
    
    auto initial_t=std::chrono::high_resolution_clock::now();
    Matrix multiplication_brute=m.brute_force_dot(m2t);
    auto final_t=std::chrono::high_resolution_clock::now();
    
    std::cout<<"Time for brute force:"<<  final_t-initial_t<<"\n";
    
    auto initial_t2=std::chrono::high_resolution_clock::now();
    Matrix multiplication_stressen=m.stressen_dot(m2t);
    auto final_t2=std::chrono::high_resolution_clock::now();


    std::cout<<"Time for stressen: "<<final_t2-initial_t2<<"\n";
   
    return 0;
}
