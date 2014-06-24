#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
//#include "Binary.h"
#include "Math/Solution.h"
#include "Logic/RW.h"
using namespace std;

#ifdef RW_BUILD_LEVELS
void define_mif();
void define_junior();

int main()
{
    std::cout << "MIF generation" << std::endl;

    define_mif();
    std::cout << "==============================" << std::endl;
    return 0;
}
#else
int main()
{

    RW::init();
    return 0;
}
#endif


