
#include <iostream>
#include "DynamicArray.h"
#include "Match.h"

int main() {
    DynamicArray<int> numbers;

    numbers.push_back(10);
    numbers.push_back(20);
    numbers.insertAt(1, 15);

    std::cout << "Size: " << numbers.getSize() << '\n';
    std::cout << "Capacity: "
              << numbers.getCapacity() << '\n';

    for (int i = 0; i < numbers.getSize(); i++) {
        int* value = numbers.getAt(i);

        if (value != nullptr) {
            std::cout << *value << ' ';
        }
    }

    std::cout << "\n";

    numbers.deleteAt(1);

    std::cout << "After deletion:\n";
    for (int i = 0; i < numbers.getSize(); i++) {
        std::cout << *numbers.getAt(i) << ' ';
    }

    std::cout << "\n";

    DynamicArray<Match> results;
    results.push_back(Match(3, 5));

    Match* match = results.getAt(0);

    if (match != nullptr) {
        std::cout << "Match line: " << match->line
                  << ", column: " << match->col << '\n';
    }

    return 0;
}
