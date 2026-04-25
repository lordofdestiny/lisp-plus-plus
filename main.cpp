#include <iostream>

int main() {
  scope(statement([] { std::cout << "A\n"; }), variable([] { return 1; }),
        variable([] { return 2; }), statement([](int &a, int &b) {
          std::cout << "B: " << a + b << "\n";
        }))();
}
