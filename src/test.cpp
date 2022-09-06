#include <iostream>

#include "magnum/threadsafe/queue.h"

int main()
{
    threadsafe::queue<int> q;

    for (int i = 0; i < 3; i++)
    {
        q.push(i);
    }

    for (int i = 0; i < 5; i++)
    {
        auto p = q.try_pop();
        if (p != nullptr)
        {
            std::cout << *p << std::endl;
        }
        else
        {
            std::cout << "q is empty" << std::endl;
        }
    }
}