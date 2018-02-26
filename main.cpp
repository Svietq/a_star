#include <iostream>
#include <ctime>
#include "a_star.h"
#include <iomanip>

int main()
{
    A_star a;
    clock_t begin = clock();

    a.run_algorithm();

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time: " << std::setprecision(15) << elapsed_secs << '\n';

    return 0;
}
