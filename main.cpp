program main(scope(statement([] { std::cout << "A\n"; }),
                   variable([] { return 1; }),
                   variable([](int& a) { return a + 2; }),
                   variable([](int& a, int& b) { return a + b + 1.5; }),
                   statement([](int& a, int& b, double& c) {
                     std::cout << "B: " << a * b + c << "\n";
                   })),
             scope(variable([] { return 1.5; }),
                   variable([](double& a) { return 2.5; }),
                   statement([](double& a, double& b) {
                     std::cout << a * b * 0.5 << std::endl;
                   })));
