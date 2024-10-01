#ifndef N4_25_EXAMPLE_UTILS
#define N4_25_EXAMPLE_UTILS

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>


void log_info(const std::string& message) {
  std::cout << message << std::endl;
}


void log_error(const std::string& message) {
  std::cout << "Ошибка: " << message << std::endl;
}


template<typename T>
T get_input(T min, T max)
{
    std::cout << ">>> ";
    T input = 0;
    std::cin >> input;

    while (input < min || input > max) {
        std::cout << "Ошибка: введите число от " << min << " до " << max << std::endl << ">>> ";
        std::cin >> input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return static_cast<T>(input);
}


std::vector<std::string> split_string(const char* str)
{
    size_t start = 0;
    size_t i = 0;
    std::vector<std::string> strings;
    while (str[i] != '\0') {
        if (str[i] == ';') {
            if (start != i) {
                strings.emplace_back(str + start, i - start);
            }
            start = i + 1;
        }
        i++;
    }
    if (start != i) {
        strings.emplace_back(str + start, i - start);
    }
    return strings;
}


#endif // N4_25_EXAMPLE_UTILS
