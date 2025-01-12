#include <pybind11/pybind11.h>
#include <string>
#include <iostream>

void print_test(const std::string& message) {
    std::cout << "C++ say: " << message << std::endl;
}

PYBIND11_MODULE(bindmodule, m) {
    m.doc() = "Test module for pybind11"; 
    m.def("print_test", &print_test, "A function that prints a message");
}
