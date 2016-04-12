// hello.cpp
#include <iostream>
using namespace std;
static void test();

int main() {
   cout << "Hello, world!" << endl;
   cout << "Monkey" << endl;
   test();
   return 0;
}

static void test()
{
    cout << "this is a " << endl;
    cout << "test" << endl;
}