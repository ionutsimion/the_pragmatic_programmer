//
// Created by ionut on 20.08.2022.
//

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "ch02_ex4.hpp"
#include "parse_utils.hpp"

namespace
{
    struct invalid_parameter_exception : std::exception
    {
        [[nodiscard]] char const *what() const noexcept override
        {
            return "Error: Numerical parameter expected.";
        }
    };

    struct unknown_command_exception : std::exception
    {
        explicit unknown_command_exception(char const command)
        {
            using namespace std::string_literals;
            text_ += "'"s + command + "'."s;
        }

        [[nodiscard]] char const *what() const noexcept override
        {
            return text_.c_str();
        }

    private:
        std::string text_{ "Error: Unknown command found: " };
    };

    struct invalid_number_of_parameters_exception : std::exception
    {
        invalid_number_of_parameters_exception(int const actual_number_of_parameters, int const expected_number_of_parameters)
        {
            using namespace std::string_literals;
            text_ += std::to_string(actual_number_of_parameters) + " found, "s
                   + std::to_string(expected_number_of_parameters) + " expected."s;
        }

    private:
        std::string text_{ "Error: Invalid number of parameters: " };
    };

    auto skip_comment(tpp::cursor_t cursor, tpp::cursor_t end)
    {
        while (cursor != end && *cursor != '\n')
            ++cursor;
        return cursor;
    }

    auto to_digit(char const c)
    {
        return c - '0';
    }

    auto scan_for_parameters(tpp::cursor_t cursor, tpp::cursor_t end, uint8_t number_of_parameters)
    {
        std::vector<int> parameters{};

        while (cursor != end && parameters.size() < number_of_parameters)
        {
            cursor = tpp::skip_whitespaces(cursor, end);

            if (!std::isdigit(*cursor))
                throw invalid_parameter_exception();

            int parameter = 0;
            while (std::isdigit(*cursor))
                parameter = parameter * 10 + to_digit(*cursor++);
            parameters.push_back(parameter);
        }

        return std::make_tuple(cursor, parameters);
    }
} // anonymous

namespace turtle
{
    void pen_down(std::vector<int> const &)
    {
        std::clog << "pen down" << std::endl;
    }

    void pen_up(std::vector<int> const &)
    {
        std::clog << "pen up" << std::endl;
    }

    void select_pen(std::vector<int> const &parameters)
    {
        std::clog << "select pen: " << parameters.front() << std::endl;
    }

    void draw_south(std::vector<int> const &parameters)
    {
        std::clog << "draw South: " << parameters.front() << "cm" << std::endl;
    }

    void draw_east(std::vector<int> const &parameters)
    {
        std::clog << "draw East: " << parameters.front() << "cm" << std::endl;
    }

    void draw_north(std::vector<int> const &parameters)
    {
        std::clog << "draw North: " << parameters.front() << "cm" << std::endl;
    }

    void draw_west(std::vector<int> const &parameters)
    {
        std::clog << "draw West: " << parameters.front() << "cm" << std::endl;
    }
}

auto tpp::ch2::ex4::turtle_parse(std::string const &code) -> command_executors_t
{
    command_executors_t commands{};

    auto cursor = code.cbegin();
    while (cursor != code.cend())
    {
        cursor = skip_whitespaces(cursor, code.cend());

        if (  auto turtle_command = std::find_if(
              turtle_commands.cbegin()
            , turtle_commands.cend()
            , [c = *cursor](turtle_command_t const command){ return command.name == c; })
            ; turtle_command != turtle_commands.cend())
        {
            std::vector<int> parameters{};
            std::tie(cursor, parameters) = scan_for_parameters(++cursor, code.cend(), turtle_command->number_of_parameters);
            commands.emplace_back(turtle_command_executors.at(turtle_command->name), parameters);
        }
        else if (*cursor == '#')
            cursor = skip_comment(cursor, code.end());
        else if (cursor != code.cend())
            throw unknown_command_exception(*cursor);
    }

    return commands;
}

void tpp::ch2::ex4::turtle_run(command_executors_t const &commands)
{
    for (auto &[f, parameters] : commands)
        f(parameters);
}

int tpp::ch2::ex4::run()
{
    using namespace std::string_literals;

    std::cout << "Welcome to Turtle Draw.\n"
              << "Enter commands one by one.\n"
              << "Enter 'exit' to quit.\n" << std::endl;

    std::string command{};
    do
    {
        std::cout << "> "s;
        std::getline(std::cin, command);
        if (command != "exit"s)
        {
            try
            {
                turtle_run(turtle_parse(command));
            }
            catch (std::exception const &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
    while (command != "exit"s);

    std::cout << "Turtle Draw out." << std::endl;

    return EXIT_SUCCESS;
}
