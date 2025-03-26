#include <iostream>
#include <chrono>
#include <vector>
#include "matrixpp.h++"
void a()
{
    std::vector<int> dimensions = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
    for (int d : dimensions)
    {
        std::cout << "dimension: " << d << "\n";
        Matrix m(d, d);
        m.fill_rand();
        Matrix m2t = m.transpose();

        auto initial_t = std::chrono::high_resolution_clock::now();
        Matrix multiplication_brute = m.brute_force_dot(m2t);
        auto final_t = std::chrono::high_resolution_clock::now();
        auto brute_force_time=static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(final_t - initial_t).count());
        std::cout << "Time for brute force:" << final_t - initial_t << "\n";
        auto initial_t2 = std::chrono::high_resolution_clock::now();
        Matrix multiplication_stressen = m.stressen_dot(m2t);
        auto final_t2 = std::chrono::high_resolution_clock::now();
        auto stressen_time=static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(final_t2 - initial_t2).count());

        std::cout << "Time for stressen:   " << final_t2 - initial_t2 << "\n";
        std::cout<<"strassen/brute force: "<<(stressen_time)/(brute_force_time*1.0+0.1)<<"\n\n";
    }
    std::cin.get();
}
int main()
{

    a();

    return 0;
}
