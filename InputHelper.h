#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <iostream>
#include <string>
#include <limits>

class InputHelper
{
public:
    static int readInt(const std::string& prompt)
    {
        int value;

        while (true)
        {
            std::cout << prompt;

            if (std::cin >> value)
            {
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );

                return value;
            }

            std::cout << "Invalid input. Please enter a number.\n";

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );
        }
    }

    static std::string readLine(const std::string& prompt)
    {
        std::string input;

        std::cout << prompt;
        std::getline(std::cin, input);

        return input;
    }

    static bool readYesNo(const std::string& prompt)
    {
        char choice;

        while (true)
        {
            std::cout << prompt << " (y/n): ";

            if (std::cin >> choice)
            {
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );

                if (choice == 'y' || choice == 'Y')
                    return true;

                if (choice == 'n' || choice == 'N')
                    return false;
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );
            }

            std::cout << "Please enter y or n.\n";
        }
    }
};

#endif