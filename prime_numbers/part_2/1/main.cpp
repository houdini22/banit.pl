#include <iostream>
#include <cmath>

const double PI = std::acos(-1);

double degreeToRadian(double degree) {
    return degree * PI / 180.0;
}

double radianToDegree(double radians) {
    return radians * 180 / PI;
}

double _sin(double x) {
    return std::sin(degreeToRadian(x));
}

double _cos(double x) {
    return std::cos(degreeToRadian(x));
}

double _tan(double x) {
    return std::tan(degreeToRadian(x));
}

int main() {
    std::cout.precision(20);

    std::cout << "sin(89) = " << _sin(89) << std::endl;
    std::cout << "cos(89) = " << _cos(89) << std::endl;
    std::cout << "tan(89) = " << _tan(89) << std::endl;

    std::cout << "sin(91) = " << _sin(91) << std::endl;
    std::cout << "cos(91) = " << _cos(91) << std::endl;
    std::cout << "tan(91) = " << _tan(91) << std::endl;

    return 0;
}