#include <iostream>

#include "ch02_ex4.hpp"
#include "ch02_ex5.hpp"

namespace tpp
{
    int run_exercise_in_chapter(int const chapter, int const exercise)
    {
        if (chapter == 2)
        {
            switch (exercise)
            {
            case 4: return ch2::ex4::run();
            case 5: return ch2::ex5::run();
            default: return EXIT_FAILURE;
            }
        }

        return EXIT_FAILURE;
    }
}

int main()
{
    std::cout << "Enter your command:"
                 "\n\t<chapter> <exercise>, where chapter and exercise are integers;"
                 "\n\tenter 0 for chapter or exercise to quit.\n" << std::endl;

    for (int chapter{}, exercise{};;)
    {
        std::cout << "> ";
        try
        {
            std::cin >> chapter >> exercise;
            if (chapter == 0 || exercise == 0)
                break;

            if (tpp::run_exercise_in_chapter(chapter, exercise) == EXIT_FAILURE)
                std::cerr << "Failed to run exercise " << exercise << " in chapter " << chapter << "." << std::endl;
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << "\nTry again!" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
