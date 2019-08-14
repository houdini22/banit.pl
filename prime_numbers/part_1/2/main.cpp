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

    std::cout << "sin(101) = " << _sin(101) << std::endl;
    std::cout << "cos(101) = " << _cos(101) << std::endl;
    std::cout << "tan(101) = " << _tan(101) << std::endl;

    std::cout << "sin(103) = " << _sin(103) << std::endl;
    std::cout << "cos(103) = " << _cos(103) << std::endl;
    std::cout << "tan(103) = " << _tan(103) << std::endl;

    return 0;
}