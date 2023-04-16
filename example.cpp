#include <Windows.h>
#include <iostream>
using namespace std;
int main()
{
    int* var;
    int i = 5;
    var = &i;
    while (true)
    {
        cout << var << " - " << *var << endl;
        Sleep(20);
        if (GetAsyncKeyState(VK_SPACE) & 0x1)
        {
            *var += 5;
        }
    }
}
