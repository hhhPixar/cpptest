#include "template-com.h"
#include <iostream>

int main() {
    Complex<double> complex_double(1.0, 2.0);
    Complex<int> complex_int(3, 4);

    std::cout << complex_double.imag() << '\n';
    std::cout << complex_int.real() << '\n';
}
