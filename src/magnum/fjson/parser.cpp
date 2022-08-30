#include "parser.h"

#include <cctype>
#include <algorithm>
#include <iostream>

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
            m_idx += 4;
            return {};
        }
        throw std::logic_error("parse null error");
    }

    bool Parser::parse_bool()
    {
        if (m_str.compare(m_idx, 4, "true") == 0)
        {
            m_idx += 4;
            return true;
        }
        if (m_str.compare(m_idx, 5, "false") == 0)
        {
            m_idx += 5;
            return false;
        }
        throw std::logic_error("parse bool error");
    }

    JsonObject Parser::parse_number()
    {
        auto pos = m_idx;
        // integer part
        if (m_str[m_idx] == '-')
        {
            m_idx++;
        }
        if (isdigit(m_str[m_idx]))
            while (isdigit(m_str[m_idx]))
                m_idx++;
        else
        {
            throw std::logic_error("invalid character in number");
        }

        if (m_str[m_idx] != '.')
        {
            return (int)strtol(m_str.c_str() + pos, nullptr, 10);
        }

        // decimal part
        if (m_str[m_idx] == '.')
        {
            m_idx++;
            if (!std::isdigit(m_str[m_idx]))
            {
                throw std::logic_error("at least one digit required in parse float part!");
            }
            while (std::isdigit(m_str[m_idx]))
                m_idx++;
        }
        return strtod(m_str.c_str() + pos, nullptr);
    }

    JsonObject Parser::parse_string()
    {
        auto pre_pos = ++m_idx;
        auto pos = m_str.find('"', m_idx);
        if (pos != std::string::npos)
        {
            //解析还没有结束，需要判断是否是转义的结束符号，如果是转义，则需要继续探查
            while (true)
            {
                if (m_str[pos - 1] != '\\') //如果不是转义则解析结束
                {
                    break;
                }
                //如果是转义字符，则判断是否已经被抵消，已经被消耗完则跳出，否则继续寻找下个字符串结束符
                if (is_esc_consume(pos - 1))
                {
                    break;
                }
                pos = m_str.find('"', pos + 1);
                if (pos == std::string::npos)
                {
                    throw std::logic_error(R"(expected left '"' in parse string)");
                }
            }
            m_idx = pos + 1;
            return m_str.substr(pre_pos, pos - pre_pos);
        }
        throw std::logic_error("parse string error");
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

            if (ch != ',') //如果不是逗号
            {
                throw std::logic_error("expected ',' in parse list");
            }
            //跳过逗号
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
            //解析key
            string_t key = std::move(parse().get_value<string_t>());
            ch = get_next_token();
            if (ch != ':')
            {
                throw std::logic_error("expected ':' in parse dict");
            }
            m_idx++;

            //解析value
            dict[key] = parse();
            ch = get_next_token();
            if (ch == '}')
            {
                m_idx++;
                break; //解析完毕
            }
            if (ch != ',') //没有结束，此时必须为逗号
            {
                throw std::logic_error("expected ',' in parse dict");
            }
            //跳过逗号
            m_idx++;
        }
        return dict;
    }

    void Parser::init(std::string_view src)
    {
        m_str = src;
        m_idx = 0;
        trim_right();
    }

    void Parser::trim_right()
    {
        m_str.erase(std::find_if(m_str.rbegin(), m_str.rend(), [](char ch)
                                 { return !std::isspace(ch); })
                        .base(),
                    m_str.end());
    }

    void Parser::skip_comment()
    {
        if (m_str.compare(m_idx, 2, R"(//)") == 0)
        {
            while (true)
            {
                auto next_pos = m_str.find('\n', m_idx);
                if (next_pos == std::string::npos)
                {
                    throw std::logic_error("invalid comment area!");
                }
                //查看下一行是否还是注释
                m_idx = next_pos + 1;
                while (isspace(m_str[m_idx]))
                {
                    m_idx++;
                }
                if (m_str.compare(m_idx, 2, R"(//)") != 0)
                { //结束注释
                    return;
                }
            }
        }
    }

    JsonObject Parser::from_string(std::string_view content)
    {
        static Parser instance;
        instance.init(content);
        return instance.parse();
    }

    bool Parser::is_esc_consume(size_t pos)
    {
        size_t end_pos = pos;
        while (m_str[pos] == '\\')
            pos--;
        auto cnt = end_pos - pos;
        //如果 \ 的个数为偶数，则成功抵消，如果为奇数，则未抵消
        return cnt % 2 == 0;
    }
} // namespace fjson
