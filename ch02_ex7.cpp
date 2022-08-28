#include <cstdlib>
#include <fstream>
#include <iostream>

#include "ch02_ex7.hpp"
#include "parse_utils.hpp"

namespace
{
    auto peek(std::string::const_iterator cursor, std::string::const_iterator end)
    {
        return cursor == end ? '\0' : *cursor;
    }

    auto is_begin_comment(std::string::const_iterator cursor, std::string::const_iterator end)
    {
        return *cursor == '(' && peek(cursor + 1, end) == '*';
    }

    auto is_end_comment(std::string::const_iterator cursor, std::string::const_iterator end)
    {
        return *cursor == '*' && peek(cursor + 1, end) == ')';
    }

    std::string::const_iterator skip_comment(std::string::const_iterator cursor, std::string::const_iterator end) // NOLINT(misc-no-recursion)
    {
        auto begin = cursor;
        while (!is_end_comment(cursor, end))
        {
            if (cursor != begin && is_begin_comment(cursor, end))
                cursor = skip_comment(cursor, end);
            else
                ++cursor;
        }

        return cursor == end ? end : std::next(cursor, 2);
    }

    auto trim(std::string const &s)
    {
        auto begin = s.cbegin();
        auto last = std::prev(s.cend());

        while (begin <= last && std::isspace(*begin))
            ++begin;
        while (begin <= last && std::isspace(*last))
            --last;

        return std::string(begin, std::next(last));
    }

    auto parse_terminal_character(tpp::cursor_t &cursor)
    {
        auto c = *(++cursor);
        std::advance(cursor, 2);

        return c;
    }

    auto create_symbols_tree(std::string const &definition)
    {
        using tpp::ch2::ex7::node_t;
        node_t *root = nullptr;
        node_t *current_node = nullptr;

        for (auto cursor = definition.cbegin(), end = definition.cend(); cursor != end;)
        {
            node_t *node = nullptr;
            cursor = tpp::skip_whitespaces(cursor, end);
            switch (*cursor)
            {
            case '\"': node = tpp::ch2::ex7::make_terminal_node(parse_terminal_character(cursor)); break;
            case ',': [[fallthrough]];
            default: ++cursor; break;
            }

            if (!node)
                continue;

            if (root == nullptr)
                root = node;
            else
                root->add_child(node);
        }

        return root;
    }
}

tpp::ch2::ex7::could_not_open_file::could_not_open_file(std::string const &file_name)
{
    what_ += file_name;
}

char const *tpp::ch2::ex7::could_not_open_file::what() const noexcept
{
    return what_.c_str();
}

tpp::ch2::ex7::node_t::~node_t()
{
    type_ = type_t::empty;
    root_ = nullptr;
    delete child_;
    delete next_;
}

tpp::ch2::ex7::lexer_t::lexer_t(std::string const &bnf_file)
{
    std::ifstream is(bnf_file.data());

    if (!is.is_open())
        throw could_not_open_file(bnf_file);

    is.seekg(0l, std::ios_base::end);
    auto const file_size = is.tellg();
    std::string data(file_size + 1l, '\0');
    is.seekg(0l, std::ios_base::beg);
    is.read(data.data(), file_size);

    create_symbols_map(data);

    for (auto const &[symbol, definition] : symbols_)
        std::clog << '[' << symbol << ", " << definition << ']' << std::endl;

    is.close();
}

void tpp::ch2::ex7::lexer_t::create_symbols_map(std::string const &grammar)
{
    for (auto end = grammar.cend(), cursor = grammar.cbegin(); cursor != end;)
    {
        cursor = tpp::skip_whitespaces(cursor, end);

        if (is_begin_comment(cursor, end))
            cursor = skip_comment(cursor, end);
        else
        {
            auto symbol_start = cursor;

            auto equal_pos = grammar.find('=', std::distance(grammar.cbegin(), symbol_start));
            if (equal_pos == std::string::npos)
                break;
            auto equal = std::next(grammar.cbegin(), static_cast<long>(equal_pos));

            auto semicolon_pos = grammar.find(';', equal_pos);
            auto statement_end = std::next(grammar.cbegin(), static_cast<long>(semicolon_pos));

            symbols_.emplace(trim(std::string(symbol_start, equal)), trim(std::string(equal + 1, statement_end)));
            auto definition_as_tree = create_symbols_tree(trim(std::string(equal + 1, statement_end)));
            delete definition_as_tree;

            cursor = ++statement_end;
        }
    }
}

int tpp::ch2::ex7::parse(std::string const &command, lexer_t &lexer)
{
    return 0;
}

int tpp::ch2::ex7::run()
{
    using namespace std::string_literals;

    lexer_t lexer("../ch02_ex6.bnf"s);

    std::cout << "Input time to compute how many minutes have passed since midnight.\n"
              << "Type 'exit' to quit." << std::endl;

    for (std::string command{};;)
    {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;

        try
        {
            auto const minutes_elapsed = parse(command, lexer);
            std::cout << minutes_elapsed << " minutes have passed since midnight." << std::endl;
        }
        catch (std::exception const &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << "Bye!" << std::endl;

    return EXIT_SUCCESS;
}
