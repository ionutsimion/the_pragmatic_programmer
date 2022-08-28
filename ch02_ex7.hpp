/*
 * Implement a parser for the BNF grammar in the previous exercise using a PEG parser generator in the language of your choice.
 * The output should be an integer containing the number of minutes past midnight.
 * NOTE: see ch02_ex6.bnf for the defined BNF.
 */

#ifndef THE_PRAGMATIC_PROGRAMMER_CH02_EX7_HPP
#define THE_PRAGMATIC_PROGRAMMER_CH02_EX7_HPP

#include <map>
#include <string>

namespace tpp::ch2::ex7
{
    class could_not_open_file : public std::exception
    {
    public:
        explicit could_not_open_file(std::string const &file_name);

        [[nodiscard]] char const *what() const noexcept override;

    private:
        std::string what_{ "Failed to open file: " };
    };

    class node_t
    {
    public:
        enum class type_t
        {
            empty
          , terminal
          , concatenate
          , optional
          , select
        };
        virtual ~node_t();

        auto add_child(node_t *const node)
        {
            if (!child_)
                child_ = node;
            else
            {
                auto *sibling = child_;
                for (; sibling->next_ != nullptr; sibling = sibling->next_);
                sibling->next_ = node;
            }
        }

    protected:
        type_t type_{ type_t::empty };
        node_t *root_{ nullptr };
        node_t *child_{ nullptr };
        node_t *next_{ nullptr };
    };

    class terminal_node_t : public node_t
    {
    public:
        terminal_node_t()
        {
            type_ = type_t::terminal;
        }

        friend terminal_node_t *make_terminal_node(char c);

    private:
        char data_{ '\0' };
    };

    inline terminal_node_t *make_terminal_node(char const c)
    {
        auto *node = new terminal_node_t{};
        node->data_ = c;
        return node;
    }

    class lexer_t
    {
    public:
        explicit lexer_t(std::string const &bnf_file);

    private:
        void create_symbols_map(std::string const &grammar);

        std::map<std::string, std::string> symbols_{};
    };

    int parse(std::string const &command, lexer_t &lexer);

    int run();
}

#endif //THE_PRAGMATIC_PROGRAMMER_CH02_EX7_HPP
