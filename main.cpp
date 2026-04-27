#include <iostream>

int
main()
{
  scope(statement([] { std::cout << "A\n"; }),
        variable([] { return 1; }),
        variable([](int& a) { return a + 2; }),
        variable([](int& a, int& b) { return a + b + 1.5; }),
        statement([](int& a, int& b, double& c) {
          std::cout << "B: " << a * b + c << "\n";
        }))();
}
