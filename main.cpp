#include <iostream>
#include "Earley_algorithm.h"

int main() {
    Grammar gram;
    int n;
    std::cin >> n;
    std::string str;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 0; i < n; ++i) {
        std::cin >> gram;
    }
    Algo a(gram);
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> str;
        a.PredictWrapper(str);
    }
    return 0;
}
