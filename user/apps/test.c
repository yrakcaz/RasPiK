#include "../sdk/libraspik.h"

int main()
{
    if (test("blop!\n") == 42)
        test("bonjour!\n");
    else
        test("lol!\n");
}
