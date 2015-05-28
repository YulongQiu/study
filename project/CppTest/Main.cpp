
//#include "containeradaptor.h"
#include "FunctionReturnValue.h"

class Test
{
public:
    Test(int a)
        : _a(a)
    {
        //_a = a;
    }
private:
    int &_a;
};

int main()
{
    returnLocalAddress();

    return 0;
}
