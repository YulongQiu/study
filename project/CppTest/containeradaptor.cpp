#include "containeradaptor.h"
#include <iostream>

void createStack()
{
    // 1. default construct on top of deque
    stack<int> intStack;

    // 2. construct stack on top of vector
    VECTOR_STACK intStack2;

}

void operateStack()
{
    stack<int> intStack;

    if (intStack.empty())
    {
        intStack.push(1);
        intStack.push(2);
        intStack.push(3);
    }

    cout << intStack.top() << endl;

    if (intStack.size() > 0)
    {
        intStack.pop();
    }
}
