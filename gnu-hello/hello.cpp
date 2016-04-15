// hello.cpp
#include <iostream>
#include <boost/format.hpp>

using namespace std;
static void test();

int main() {
   cout << "Hello, world!" << endl;
   cout << "Monkey" << endl;
   test();
   return 0;
}

// This function is completely useless :)
static void test()
{
    cout << "this is a " << endl;
    cout << "test" << endl;
}dhmt