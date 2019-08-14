#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <map>
#include <algorithm>

// types
typedef long long T_NUM;
typedef std::vector<T_NUM> T_NUM_VEC;
typedef std::string T_STR;
struct T_STRUCT_NUMBER {
    T_NUM number;
    bool isPrime;
    bool isEven;
    bool isOdd;
};
struct T_STRUCT_SYMMETRY {
    T_NUM count = 0;
    std::vector<std::pair<T_NUM, T_NUM>> primes;
};
struct T_NUMBER_SYMMETRY {
    T_STRUCT_NUMBER number;
    T_NUM center;
    T_NUM length;
};
typedef std::map<T_NUM, std::vector<T_NUMBER_SYMMETRY>> T_VEC_NUMBER_SYMMETRIES;

// functions
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

bool isPrime(T_NUM num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++)
        if (num % i == 0) return false;
    return true;
}

// initialization
std::ofstream resultFile;
std::ofstream resultFileDetailed;
std::ofstream resultFileNumberSymmetry;
std::ofstream resultFilePython;
T_NUM limit = 130;

int main() {
    resultFile.open("./data/result.txt");
    resultFileDetailed.open("./data/result_details.txt");
    resultFileNumberSymmetry.open("./data/result_number_symmetry.txt");
    resultFilePython.open("./data/result_python.txt");

    // fill
    std::map<T_NUM, T_STRUCT_NUMBER> numbers;

    for (T_NUM number = 1; number <= limit; number++) {
        T_STRUCT_NUMBER t_struct;
        t_struct.number = number;
        t_struct.isPrime = isPrime(number);
        t_struct.isEven = (number % 2) == 0;
        t_struct.isOdd = (number % 2) == 1;
        numbers[number - 1] = t_struct;

        if (number % 1000 == 0 || number == limit) {
            std::cout << "Initialization progress: " << number << "/" << limit << std::endl;
        }
    }

    std::cout << "Calculating symmetries..." << std::endl;

    std::map<T_NUM, T_STRUCT_SYMMETRY> symmetries;
    T_VEC_NUMBER_SYMMETRIES t_vec_number_symmetries;

    for (T_NUM number = 1; number <= limit; number++) {
        if (number % 1000 == 0 || number == limit) {
            std::cout << "Symmetry check progress: " << number << "/" << limit << std::endl;
        }

        if (numbers[number - 1].isEven) {
            for (T_NUM number2 = numbers[number - 1].number, start = 1;
                 number2 > 0 && number2 <= limit && number2 - start > 0 && number2 + start <= limit; start++) {
                T_STRUCT_NUMBER upper = numbers[number - 1 + start];
                T_STRUCT_NUMBER below = numbers[number - 1 - start];

                if (upper.isPrime && below.isPrime) {
                    if (symmetries.find(upper.number - below.number) == symmetries.end()) {
                        T_STRUCT_SYMMETRY t_struct_symmetry;
                        symmetries[upper.number - below.number] = t_struct_symmetry;
                    }
                    symmetries[upper.number - below.number].count++;
                    symmetries[upper.number - below.number].primes.emplace_back(
                            std::make_pair(below.number, upper.number));

                    T_NUMBER_SYMMETRY below_symmetry;
                    below_symmetry.number = below;
                    below_symmetry.center = (below.number + upper.number) / 2;

                    T_NUMBER_SYMMETRY upper_symmetry;
                    upper_symmetry.number = upper;
                    upper_symmetry.center = (upper.number + below.number) / 2;

                    t_vec_number_symmetries[upper.number].emplace_back(below_symmetry);
                    t_vec_number_symmetries[below.number].emplace_back(upper_symmetry);
                }
            }
        }
    }

    std::cout << "Symmetries size: " << symmetries.size() << std::endl;
    std::cout << "Generating file..." << std::endl;

    std::map<T_NUM, T_STRUCT_SYMMETRY>::iterator it = symmetries.begin();
    while (it != symmetries.end()) {
        T_NUM size = it->first;
        T_NUM count = it->second.count;

        resultFile << createCell(2, "Size:") << createCell(2, std::to_string(size)) << std::endl;
        resultFile << createCell(2, "Count:") << createCell(2, std::to_string(count)) << std::endl;
        resultFile << "---------" << std::endl;

        resultFileDetailed << createCell(2, "Size:") << createCell(2, std::to_string(size)) << std::endl;
        resultFileDetailed << createCell(2, "Count:") << createCell(2, std::to_string(count)) << std::endl;
        resultFileDetailed << createCell(2, "___DETAILS___") << std::endl;

        for (size_t i = 0; i < it->second.primes.size(); i++) {
            resultFileDetailed << createCell(2, "Below:") << it->second.primes.at(i).first << std::endl;
            resultFileDetailed << createCell(2, "Center:")
                               << std::to_string(((it->second.primes.at(i).first + it->second.primes.at(i).second) / 2))
                               << std::endl;
            resultFileDetailed << createCell(2, "Upper:") << it->second.primes.at(i).second << std::endl << "---"
                               << std::endl;
        }

        resultFileDetailed << "---------" << std::endl << std::endl;

        it++;
    }

    std::cout << "File generated." << std::endl;
    std::vector<T_NUM> allPrimes;

    for (T_NUM number = 1; number <= limit; number++) {
        T_STRUCT_NUMBER t_struct_number = numbers[number - 1];
        if (t_struct_number.isPrime) {
            allPrimes.push_back(t_struct_number.number);
        }
    }

    std::cout << "All Primes Count: " << allPrimes.size() << std::endl;

    auto it2 = t_vec_number_symmetries.begin();
    T_NUM count = 0;
    T_NUM size = (T_NUM) t_vec_number_symmetries.size();

    while (it2 != t_vec_number_symmetries.end()) {
        T_NUM number = it2->first;
        std::vector<T_NUMBER_SYMMETRY> t_vec_number_symmetry = it2->second;

        resultFileNumberSymmetry << createCell(2, "NUMBER:") << createCell(3, std::to_string(number)) << std::endl;

        for (size_t i = 0; i < t_vec_number_symmetry.size(); i += 1) {
            T_NUMBER_SYMMETRY t_number_symmetry = t_vec_number_symmetry.at(i);
            resultFileNumberSymmetry << createCell(1, "") << createCell(1, "C")
                                     << createCell(3, std::to_string(t_number_symmetry.center));
            resultFileNumberSymmetry << createCell(1, "") << createCell(1, "P")
                                     << createCell(3, std::to_string(t_number_symmetry.number.number));
            resultFileNumberSymmetry << createCell(1, "") << createCell(1, "L")
                                     << createCell(3, std::to_string((
                                                                             std::max(number,
                                                                                      t_number_symmetry.number.number) -
                                                                             std::min(number,
                                                                                      t_number_symmetry.number.number)
                                                                     ) / 2));

            resultFilePython << std::to_string(t_number_symmetry.center);
            resultFilePython << ",";
            resultFilePython << (std::max(number, t_number_symmetry.number.number) -
                                 std::min(number, t_number_symmetry.number.number)) / 2;
            resultFilePython << std::endl;


            resultFileNumberSymmetry << std::endl;
        }
        resultFileNumberSymmetry << "---------" << std::endl;

        it2++;
        count++;

        if (count % 50 == 0) {
            std::cout << count << " / " << size << std::endl;
        }
    }

    resultFile.close();
    resultFileDetailed.close();
    resultFileNumberSymmetry.close();
    resultFilePython.close();

    return 0;
}