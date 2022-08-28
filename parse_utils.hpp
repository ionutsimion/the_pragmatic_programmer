#ifndef THE_PRAGMATIC_PROGRAMMER_PARSE_UTILS_HPP
#define THE_PRAGMATIC_PROGRAMMER_PARSE_UTILS_HPP

namespace tpp
{
    using cursor_t = std::string::const_iterator;

    inline auto skip_whitespaces(cursor_t cursor, cursor_t end)
    {
        while (cursor != end && (std::isspace(*cursor)))
            ++cursor;
        return cursor;
    }
}

#endif //THE_PRAGMATIC_PROGRAMMER_PARSE_UTILS_HPP
