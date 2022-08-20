/*
 * We want to implement a mini-language to control a simple turtle-graphics system.
 * The language consists of single-letter commands, some followed by a single number.
 * For example, the following input would draw a rectangle:
 *
 *	P 2  # select pen 2
 *	D    # pen down
 *	W 2  # draw west 2cm
 *	N 1  # then north 1
 *	E 2  # then east 2
 *	S 1  # then back south
 *	U    # pen up
 *
 * Implement the code that parses this language.
 * It should be designed so that it is simple to add new commands.
 */

#ifndef THE_PRAGMATIC_PROGRAMMER_CH02_EX4_HPP
#define THE_PRAGMATIC_PROGRAMMER_CH02_EX4_HPP

#include <array>
#include <functional>
#include <map>
#include <string>
#include <variant>

namespace turtle
{
    extern void pen_down(std::vector<int> const & = {});
    extern void pen_up(std::vector<int> const & = {});
    extern void select_pen(std::vector<int> const & = {});
    extern void draw_south(std::vector<int> const & = {});
    extern void draw_east(std::vector<int> const & = {});
    extern void draw_north(std::vector<int> const & = {});
    extern void draw_west(std::vector<int> const & = {});
}

namespace tpp::ch2::ex4
{
    using command_executor_t = std::tuple<std::function<void(std::vector<int> const &)>, std::vector<int>>;
    using command_executors_t = std::vector<command_executor_t>;

    struct turtle_command_t
    {
        char name{};
        uint8_t number_of_parameters{};
    };

    static constexpr std::array turtle_commands {
        turtle_command_t{ 'D', 0u }
      , turtle_command_t{ 'U', 0u }
      , turtle_command_t{ 'P', 1u }
      , turtle_command_t{ 'S', 1u }
      , turtle_command_t{ 'E', 1u }
      , turtle_command_t{ 'N', 1u }
      , turtle_command_t{ 'W', 1u }
    };

    static const std::map<char, std::function<void(std::vector<int> const &)>> turtle_command_executors {
        { 'D', turtle::pen_down }
      , { 'U', turtle::pen_up }
      , { 'P', turtle::select_pen }
      , { 'S', turtle::draw_south }
      , { 'E', turtle::draw_east }
      , { 'N', turtle::draw_north }
      , { 'W', turtle::draw_west }
    };

    command_executors_t turtle_parse(std::string const &code);
    void turtle_run(command_executors_t const &commands);

    int run();
} // tpp

#endif //THE_PRAGMATIC_PROGRAMMER_CH02_EX4_HPP
