#include "ch02_ex4.hpp"
#include "ch02_ex5.hpp"

int tpp::ch2::ex5::run()
{
    turtle::select_pen({ 2 });
    turtle::pen_down();
    turtle::draw_west({ 2 });
    turtle::draw_north({ 1 });
    turtle::draw_east({ 2 });
    turtle::draw_south({ 1 });
    turtle::pen_up();

    return EXIT_SUCCESS;
}
