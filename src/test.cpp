#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "STL/utils.h"
#include "STL/functional.h"
#include "STL/algobase.h"
#include "pylike/pystr.h"
#include "magnum/timer.h"

int main()
{
    const int num = 100000;
    timer timer;

    timer.start();
    std::string str1 = "";
    for (int i = 0; i < num; i++)
    {
        str1 += "a";
    }
    timer::millisecond cost_time = timer.end();

    std::cout << "str+=a 所耗费的时间：" << cost_time << " ms" << std::endl;

}
