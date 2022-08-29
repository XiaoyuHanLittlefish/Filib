#include "parser.h"

namespace fjson
{
    JsonObject Parser::parse()
    {
        char token = get_next_token();
        if (token == 'n')
        {
            return parse_null();
        }
        if (token == 't' || token == 'f')
        {
            return parse_bool();
        }
        if (token == '-' || std::isdigit(token))
        {
            return parse_number();
        }
        if (token == '\"')
        {
            return parse_string();
        }
        if (token == '[')
        {
            return parse_list();
        }
        if (token == '{')
        {
            return parse_dict();
        }

        throw std::logic_error("unexpected character in parse json");
    }

    char Parser::get_next_token()
    {
        while (std::isspace(m_str[m_idx]))
        {
            m_idx++;
        }

        if (m_idx >= m_str.size())
        {
            throw std::logic_error("unexpected character in parse json");
        }

        skip_comment();
        return m_str[m_idx];
    }

    JsonObject Parser::parse_null()
    {
        if (m_str.compare(m_idx, 4, "null") == 0)
        {
            return {};
        }
        throw std::logic_error("parse null error");
    }

    JsonObject Parser::parse_bool()
    {
        if (m_str.compare(m_idx, 4, "true") == 0)
        {
            return "true";
        }
        if (m_str.compare(m_idx, 5, "false") == 0)
        {
            return "false";
        }
        throw std::logic_error("parse bool error");
    }

    JsonObject Parser::parse_number()
    {
        auto pos = m_idx;

        // integer
        if (m_str[m_idx] == '-')
        {
            m_idx++;
        }
        if (isdigit(m_str[m_idx]))
        {
            while (isdigit(m_str[m_idx]))
            {
                m_idx++;
            }
        }
        else
        {
            throw std::logic_error("invalid character in number");
        }

        if (m_str[m_idx] != '.')
        {
            return (int)strtol(m_str.c_str() + pos, nullptr, 10);
        }

        // decimal
        if (m_str[m_idx])
        {
            m_idx++;
            if (!std ::isdigit(m_str[m_idx]))
            {
                throw std::logic_error("at least one digit required in parse float part!");
            }
            while (std::isdigit(m_str[m_idx]))
            {
                m_idx++;
            }
        }
        return strtof64(m_str.c_str() + pos, nullptr);
    }

    JsonObject Parser::parse_string()
    {
        m_idx++;
        auto pos = m_idx;
        while (m_str[m_idx] != '\"')
        {
            m_idx++;
        }
        if (m_idx >= m_str.size())
        {
            throw std::logic_error("a double quotation mark is required at the end of the string");
        }

        return m_str.substr(pos, m_idx - pos);
    }

    JsonObject Parser::parse_list()
    {
        JsonObject arr((list_t()));
        m_idx++;
        char ch = get_next_token();
        if (ch == ']')
        {
            m_idx++;
            return arr;
        }

        while (true)
        {
            arr.push_back(parse());
            ch = get_next_token();

            if (ch == ']')
            {
                m_idx++;
                break;
            }

            if (ch != ',')
            {
                throw std::logic_error("expected ',' in parse list");
            }

            // skip ','
            m_idx++;
        }
        return arr;
    }

    JsonObject Parser::parse_dict()
    {
        JsonObject dict((dict_t()));
        m_idx++;
        char ch = get_next_token();
        if (ch == '}')
        {
            m_idx++;
            return dict;
        }

        while (true)
        {
            // value
            string_t key = std::move(parse().Value<string>());
            ch = get_next_token();
            if (ch != ':')
            {
                throw std::logic_error("expected ':' in parse dict");
            }
            m_idx++;

            // key
            dict[key] = parse();
            ch = get_next_token();
            if (ch == '}')
            {
                m_idx++;
                break;
            }

            if (ch != ',')
            {
                throw std::logic_error("expected ',' in parse dict");
            }

            // skip ','
            m_idx++;
        }
        return dict;
    }
} // namespace fjson
