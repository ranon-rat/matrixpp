#include <iostream>
#include <chrono>
#include <vector>
#include "matrixpp.h++"
#include <cmath>
std::chrono::milliseconds now()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch());
}
void a()
{
    for (int i = 1; i < 15; i++)
    {
        int d = std::pow(2, i);

        std::cout << "dimension: " << d << "\n";
        Matrix m(d, d);
        m.fill_rand();
        Matrix m2t = m.transpose();

        auto initial_t = now();
        Matrix multiplication_brute = m.brute_force_dot(m2t);
        auto final_t = now();
        auto brute_force_time = static_cast<int>((final_t - initial_t).count());
        std::cout << "Time for brute force:" << final_t - initial_t << "\n";
        auto initial_t2 = now();
        Matrix multiplication_stressen;
        multiplication_stressen = m.stressen_dot(m2t);
        std::cout << "finish??";
        auto final_t2 = now();
        auto stressen_time = static_cast<int>((final_t2 - initial_t2).count());
        std::cout << "Time for stressen:   " << final_t2 - initial_t2 << "\n";
        std::cout << "difference: " << multiplication_brute.sum() - multiplication_stressen.sum() << "\n";
        std::cout << "strassen/brute force: " << (stressen_time) / (brute_force_time * 1.0 + 0.1) << "\n\n";
    }
    std::cin.get();
}
int main()
{

    Matrix m(3, 2);
    m.fill_rand();
    Matrix m2(2, 3);
    m2.fill_rand();
    Matrix m3 = m.stressen_dot(m2);
    std::cout << m.stressen_dot(m2);
    std::cout << m.brute_force_dot(m2);

    a();

    return 0;
}
