#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <sstream>

// data types
typedef long long T_NUM;
typedef std::vector<T_NUM> T_NUM_VEC;
typedef std::string T_STR;
typedef std::vector<T_STR> T_STR_VEC;
typedef std::pair<T_NUM, T_NUM> T_PAIR;
typedef std::vector<T_PAIR> T_PAIR_VEC;
typedef std::vector<std::vector<std::vector<T_NUM>>> T_3D_NUM_VEC;
struct T_STRUCT {
    T_NUM angle;
    T_NUM angleSize;
    T_NUM sum = 0;
};
typedef std::vector<T_STRUCT> T_STRUCTURE_VECTOR;
struct T_STRUCT_SYMMETRY {
    T_NUM start = -999999999;
    T_NUM center = -999999999;
    T_NUM end = -999999999;
    T_NUM angle = -999999999;
    T_NUM length = -999999999;
    T_NUM sum = 0;
};
typedef std::vector<T_STRUCT_SYMMETRY> T_SYMMETRY_VECTOR;
struct T_SORT_STRUCT {
    inline bool operator()(const T_STRUCT &s1, const T_STRUCT &s2) {
        return (s1.angle < s2.angle);
    }
};
struct T_HISTOGRAM_SORT_STRUCT {
    inline bool operator()(const T_PAIR &s1, const T_PAIR &s2) {
        return (s1.first < s2.first);
    }
};

// data files
std::ofstream fileHistogram;
std::ofstream fileSumsSymmetry;
std::ofstream fileSumsSymmetry2;
std::ofstream filePython;

// helpers
int charToInt(char c) {
    int result = c;
    switch (result) {
        case 48:
            return 0;
        case 49:
            return 1;
        case 50:
            return 2;
        case 51:
            return 3;
        case 52:
            return 4;
        case 53:
            return 5;
        case 54:
            return 6;
        case 55:
            return 7;
        case 56:
            return 8;
        case 57:
            return 9;
        default:
            return 0;
    }
}

T_STR getNumbersFromTri(double num, bool ommit, unsigned long length) {
    T_STR result;
    T_STR str = std::to_string(num);

    for (size_t i = 0; i < str.length(); i++) {
        if (str.at(i) == '-') {
            continue;
        }
        if (ommit) {
            if (str.at(i) != '.') {
                continue;
            } else {
                ommit = false;
                continue;
            }
        } else {
            if (str.at(i) == '.') {
                continue;
            }
        }

        result += str.at(i);

        if (result.length() == length) {
            break;
        }
    }

    return result;
}

T_NUM sumVector(T_NUM_VEC vec) {
    T_NUM result = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        result += vec.at(i);
    }
    return result;
}

T_NUM_VEC createVectorFromTri(T_STR sinus, T_STR cosinus, T_STR tangent) {
    T_NUM_VEC result = T_NUM_VEC();
    for (int i = 0; i < sinus.length(); i++) {
        result.push_back(charToInt(sinus.at(i)));
    }
    for (int i = 0; i < cosinus.length(); i++) {
        result.push_back(charToInt(cosinus.at(i)));
    }
    for (int i = 0; i < tangent.length(); i++) {
        result.push_back(charToInt(tangent.at(i)));
    }
    return result;
}

T_STR createCell(T_NUM tabLength, T_STR content) {
    T_NUM subtract = (tabLength * 4) - (T_NUM) content.length();
    auto numTabs = (T_NUM) ceil((int) subtract / (int) 4);

    if (((int) subtract) % 4 != 0) {
        numTabs++;
    }

    for (T_NUM i = 0; i < numTabs; i++) {
        content.append("\t");
    }

    return content;
}

void writeHistogramToFile(T_STRUCTURE_VECTOR arr) {
    T_PAIR_VEC histogram;
    T_NUM allSum = 0;

    for (size_t i = 0; i < arr.size(); i++) {
        bool found = false;
        T_NUM sum = arr[i].sum;

        allSum += sum;

        for (size_t j = 0; j < histogram.size(); j++) {
            if (histogram[j].first == sum) {
                histogram[j].second++;
                found = true;
            }
        }

        if (!found) {
            histogram.push_back(std::make_pair(sum, 1));
        }
    }

    std::sort(histogram.begin(), histogram.end(), T_HISTOGRAM_SORT_STRUCT());
    auto it = std::unique(histogram.begin(), histogram.end());
    histogram.resize(std::distance(histogram.begin(), it));

    for (size_t j = 0; j < histogram.size(); j++) {
        fileHistogram << histogram[j].first << ":\t" << histogram[j].second << "\n";
    }
}

static double degreeToRadian(double d) {
    return (d / 180.0) * ((double) M_PI);
}

double sind(double x) {
    if (!isfinite(x)) {
        return sin(x);
    }
    if (x < 0.0) {
        return -sind(-x);
    }
    int quo;
    double x90 = remquo(fabs(x), 90.0, &quo);
    switch (quo % 4) {
        case 0:
            // Use * 1.0 to avoid -0.0
            return sin(degreeToRadian(x90) * 1.0);
        case 1:
            return cos(degreeToRadian(x90));
        case 2:
            return sin(degreeToRadian(-x90) * 1.0);
        case 3:
            return -cos(degreeToRadian(x90));
    }
    return 0.0;
}

double cosd(double x /* degrees */) {
    if (!isfinite(x)) {
        return std::cos(x);
    }
    int quo;
    double x90 = std::remquo(std::fabs(x), 90.0, &quo);
    double xr = degreeToRadian(x90);
    switch (quo % 4) {
        case 0:
            return std::cos(xr);
        case 1:
            // Use + 0.0 to avoid -0.0
            return std::sin(-xr + 0.0);
        case 2:
            return -std::cos(xr);
        case 3:
            return std::sin(xr + 0.0);
    }
    return 0.0;
}

double _tand(double x /* degrees */) {
    if (!isfinite(x)) {
        return tan(x);
    } else if (x < 0.0) {
        return -_tand(-x);
    }
    int quo;
    double x45 = remquo(fabs(x), 90.0, &quo);
    switch (quo % 4) {
        case 0:
            return tan(degreeToRadian(x45));
        case 1:
            return 1.0 / tan(degreeToRadian(-x45));
        case 2:
            return -tan(degreeToRadian(-x45));
        case 3:
            return -1.0 / tan(degreeToRadian(x45));
    }
    return 0.0;
}

double tand(double x) {
    double result = _tand(x);
    if (!isfinite(result)) {
        return 0.0;
    }
    return result;
}

const T_NUM ___BEGIN___ = 999;

int main() {
    // initialize
    std::cout.precision(20);

    fileHistogram.open("./data/histogram.txt");
    fileSumsSymmetry.open("./data/sums_symmetry.txt");
    fileSumsSymmetry2.open("./data/sums_symmetry_2.txt");
    filePython.open("./data/python_data.txt");

    // start
    T_NUM_VEC allFound;
    T_STRUCTURE_VECTOR arr; // main data container

    for (T_NUM step = -___BEGIN___; step <= ___BEGIN___; step += 1) {
        T_NUM_VEC vec1 = createVectorFromTri(
                getNumbersFromTri(sind(step), true, std::to_string(abs(step)).length()),
                getNumbersFromTri(cosd(step), true, std::to_string(abs(step)).length()),
                getNumbersFromTri(tand(step), false, std::to_string(abs(step)).length()));

        // create data
        T_STRUCT s1;
        s1.angle = step;
        s1.angleSize = (T_NUM) std::to_string(abs(step)).length();
        s1.sum = sumVector(vec1);

        arr.push_back(s1);
    }

    std::sort(arr.begin(), arr.end(), T_SORT_STRUCT());

    writeHistogramToFile(arr);

    T_SYMMETRY_VECTOR symmetries;

    for (T_NUM base = -___BEGIN___, start = 0; base <= ___BEGIN___; base += 1, start++) {
        if (base == -___BEGIN___) { // not smallest angle
            continue;
        }

        T_NUM range = 1;

        while (true) {
            if (start - range < 0 || start + range > arr.size()) {
                break;
            }

            // select ranges
            T_NUM currentUpperSum = arr[start + range].sum;
            T_NUM currentBelowSum = arr[start - range].sum;
            T_NUM prevUpperSum = arr[start + (range - 1)].sum;
            T_NUM prevBelowSum = arr[start - (range - 1)].sum;

            if (currentUpperSum != currentBelowSum && prevUpperSum == prevBelowSum && range > 1) {
                T_STRUCT_SYMMETRY symmetry;
                symmetry.start = start - (range - 1);
                symmetry.end = start + (range - 1);
                symmetry.center = start;
                symmetry.angle = arr[start].angle;
                symmetry.length = (start + (range - 1)) - (start - (range - 1));

                for (T_NUM i = (start - range); i <= (start + range); i++) {
                    symmetry.sum += arr[i].sum;
                }

                symmetries.push_back(symmetry);
                break;
            }

            range++;
        }
    }

    for (T_NUM base = -___BEGIN___, start = 0; base <= ___BEGIN___; base += 1, start++) {
        if (base == -___BEGIN___) { // not smallest angle
            continue;
        }

        T_STRUCT current = arr[start];

        for (size_t i = 0; i < symmetries.size(); i++) {
            T_STRUCT_SYMMETRY symmetry = symmetries.at(i);
            if (start == symmetry.center) {
                fileSumsSymmetry << createCell(6, "CENTER OF ANGLE " + std::to_string(symmetry.angle));
                fileSumsSymmetry << createCell(4, "LENGTH " + std::to_string(symmetry.length));
                fileSumsSymmetry << createCell(2, "SUM " + std::to_string(symmetry.sum));
                fileSumsSymmetry << std::endl;
            }
            if (start == symmetry.start) {
                fileSumsSymmetry << createCell(6, "START OF ANGLE " + std::to_string(symmetry.angle));
                fileSumsSymmetry << createCell(4, "LENGTH " + std::to_string(symmetry.length));
                fileSumsSymmetry << createCell(2, "SUM " + std::to_string(symmetry.sum));
                fileSumsSymmetry << std::endl;
            }
            if (start == symmetry.end) {
                fileSumsSymmetry << createCell(6, "END OF ANGLE " + std::to_string(symmetry.angle));
                fileSumsSymmetry << createCell(4, "LENGTH " + std::to_string(symmetry.length));
                fileSumsSymmetry << createCell(2, "SUM " + std::to_string(symmetry.sum));
                fileSumsSymmetry << std::endl;
            }
        }

        fileSumsSymmetry << createCell(16, "");
        fileSumsSymmetry << createCell(2, "ANGLE:");
        fileSumsSymmetry << createCell(3, std::to_string(arr[start].angle));
        fileSumsSymmetry << createCell(2, "SUM:");
        fileSumsSymmetry << createCell(2, std::to_string(arr[start].sum));
        fileSumsSymmetry << std::endl;
    }

    for (size_t i = 0; i < symmetries.size(); i++) {
        T_STRUCT_SYMMETRY symmetry = symmetries.at(i);
        fileSumsSymmetry2 << createCell(3, "ANGLE");
        fileSumsSymmetry2 << createCell(2, std::to_string(symmetry.angle)) << std::endl;
        fileSumsSymmetry2 << createCell(3, "START ANGLE");
        fileSumsSymmetry2 << createCell(2, std::to_string(arr[symmetry.start].angle)) << std::endl;
        fileSumsSymmetry2 << createCell(3, "END ANGLE");
        fileSumsSymmetry2 << createCell(2, std::to_string(arr[symmetry.end].angle)) << std::endl;
        fileSumsSymmetry2 << createCell(3, "LENGTH");
        fileSumsSymmetry2 << createCell(2, std::to_string(symmetry.length)) << std::endl;
        fileSumsSymmetry2 << createCell(3, "SUM");
        fileSumsSymmetry2 << createCell(2, std::to_string(symmetry.sum)) << std::endl;
        fileSumsSymmetry2 << "---------------------" << std::endl;

        filePython << std::to_string(arr[symmetry.start].angle) << "," << std::to_string(arr[symmetry.end].angle) << std::endl;
    }

    fileSumsSymmetry.close();
    fileSumsSymmetry2.close();
    return 0;
}