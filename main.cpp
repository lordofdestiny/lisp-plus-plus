#include <string>

using namespace std::literals;

program main(variable([] { return "Hello"s; }),
             statement([](std::string& s) { std::cout << s << std::endl; }),
             statement([](std::string& s) { s = s + " world!"; }),
             statement([](std::string& s) { std::cout << s << std::endl; }),
             scope(statement([] { std::cout << "A\n"; }),
                   variable([] { return 1; }),
                   statement([](int& a) { a++; }),
                   variable([](int& a) { return a + 2; }),
                   variable([](int& a, int& b) { return a + b + 1.5; }),
                   statement([](int& a, int& b, double& c) {
                     std::cout << "B: " << a * b + c << "\n";
                   })),
             variable([](auto& q) { return 1; }),
             scope(variable([] { return 1.5; }),
                   variable([](double& a) { return 2.5; }),
                   statement([](double& a, double& b) {
                     std::cout << a * b * 0.5 << std::endl;
                   })),
             scope(statement([] { std::cout << "Hello world!\n"; })));
