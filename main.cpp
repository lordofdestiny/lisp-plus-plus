#include <iostream>

int main() {
  scope(statement([] { std::cout << "A\n"; }), variable([] { return 1; }),
        variable([] { return 2; }), variable([] { return 3; }),
        statement([](int &a, int &b, int &c) {
          std::cout << "B: " << a + b + c << "\n";
        }))();
}
