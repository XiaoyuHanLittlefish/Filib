#include "pystr.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>

namespace pylike::details
{
    void reverse_strings(std::vector<std::string> &result)
    {
        for (std::vector<std::string>::size_type i = 0; i < result.size() / 2; i++)
        {
            std::swap(result[i], result[result.size() - 1 - i]);
        }
    }

    void split_whitespace(const std::string &str, std::vector<std::string> &result, int maxsplit = MAX_32BIT_INT)
    {
        std::string::size_type i, j, len = str.size();
        for (i = j = 0; i < len;)
        {
            while (i < len && ::isspace(str[i]))
            {
                i++;
            }
            j = i;

            while (i < len && !::isspace(str[i]))
            {
                i++;
            }

            if (j < i)
            {
                if (maxsplit-- <= 0)
                {
                    break;
                }

                result.push_back(str.substr(j, i - j));

                while (i < len && ::isspace(str[i]))
                {
                    i++;
                }
                j = i;
            }
        }

        if (j < len)
        {
            result.push_back(str.substr(j, len - j));
        }
    }

    void rsplit_whitespace(const std::string &str, std::vector<std::string> &result, int maxsplit = MAX_32BIT_INT)
    {
        std::string::size_type len = str.size();
        std::string::size_type i = 0, j = 0;

        for (i = j = len; i > 0;)
        {
            while (i > 0 && ::isspace(str[i - 1]))
            {
                i--;
            }
            j = i;

            while (i > 0 && !::isspace(str[i - 1]))
            {
                i--;
            }

            if (j > i)
            {
                if (maxsplit-- <= 0)
                {
                    break;
                }

                result.push_back(str.substr(i, j - i));

                while (i > 0 && ::isspace(str[i - 1]))
                {
                    i--;
                }
                j = i;
            }
        }

        if (j > 0)
        {
            result.push_back(str.substr(0, j));
        }
        reverse_strings(result);
    }

    int string_tailmatch(const std::string &self, const std::string &substr,
                         Py_ssize_t start, Py_ssize_t end,
                         int direction)
    {
        Py_ssize_t len = (Py_ssize_t)self.size();
        Py_ssize_t slen = (Py_ssize_t)substr.size();

        const char *sub = substr.c_str();
        const char *str = self.c_str();

        adjust_indices(start, end, len);

        if (direction < 0)
        {
            if (start + slen > len)
            {
                return 0;
            }
        }
        else
        {
            if (end - start < slen || start > len)
            {
                return 0;
            }

            if (end - slen > start)
            {
                start = end - slen;
            }
        }

        if (end - start >= slen)
        {
            return !std::memcmp(str + start, sub, slen);
        }

        return 0;
    }
} // namespace pylike::details

namespace pylike
{
#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS) || defined(_MSC_VER)
#ifndef WINDOWS
#define WINDOWS
#endif
#endif

    typedef int Py_ssize_t;
    const std::string forward_slash = "/";
    const std::string double_forward_slash = "//";
    const std::string triple_forward_slash = "///";
    const std::string double_back_slash = "\\";
    const std::string empty_string = "";
    const std::string dot = ".";
    const std::string double_dot = "..";
    const std::string colon = ":";

    inline void adjust_indices(int &start, int &end, const int len)
    {
        if (end > len)
        {
            end = len;
        }
        else if (end < 0)
        {
            end += len;
            if (end < 0)
            {
                end = 0;
            }
        }

        if (start < 0)
        {
            start += len;
            if (start < 0)
            {
                start = 0;
            }
        }
    }

    void split(const std::string &str, std::vector<std::string> &result, const std::string &sep, int maxsplit)
    {
        result.clear();

        if (maxsplit < 0)
        {
            maxsplit = MAX_32BIT_INT;
        }

        if (sep.size() == 0)
        {
            details::split_whitespace(str, result, maxsplit);
            return;
        }

        std::string::size_type i, j, len = str.size(), n = sep.size();

        i = j = 0;

        while (i + n <= len)
        {
            if (str[i] == sep[0] && str.substr(i, n) == sep)
            {
                if (maxsplit-- <= 0)
                {
                    break;
                }

                result.push_back(str.substr(j, i - j));
                i = j = i + n;
            }
            else
            {
                i++;
            }
        }

        result.push_back(str.substr(j, len - j));
    }

    void rsplit(const std::string &str, std::vector<std::string> &result, const std::string &sep, int maxsplit)
    {
        if (maxsplit < 0)
        {
            split(str, result, sep, maxsplit);
            return;
        }

        result.clear();

        if (sep.size() == 0)
        {
            details::rsplit_whitespace(str, result, maxsplit);
            return;
        }

        Py_ssize_t len = (Py_ssize_t)str.size();
        Py_ssize_t n = (Py_ssize_t)sep.size();
        Py_ssize_t i, j;
        i = j = len;

        while (i >= n)
        {
            if (str[i - 1] == sep[n - 1] && str.substr(i - n, n) == sep)
            {
                if (maxsplit-- <= 0)
                {
                    break;
                }

                result.push_back(str.substr(i, j - i));
                i = j = i - n;
            }
            else
            {
                i--;
            }
        }

        result.push_back(str.substr(0, j));
        details::reverse_strings(result);
    }

#define LEFTSTRIP 0
#define RIGHTSTRIP 1
#define BOTHSTRIP 2

    std::string do_strip(const std::string &str, int striptype, const std::string &chars)
    {
        Py_ssize_t len = (Py_ssize_t)str.size(), charslen = (Py_ssize_t)chars.size();
        Py_ssize_t i, j;

        if (charslen == 0)
        {
            i = 0;
            if (striptype != RIGHTSTRIP)
            {
                while (i < len && ::isspace(str[i]))
                {
                    i++;
                }
            }

            j = len;
            if (striptype != LEFTSTRIP)
            {
                do
                {
                    j--;
                } while (j >= i && ::isspace(str[j]));

                j++;
            }
        }
        else
        {
            const char *sep = chars.c_str();

            i = 0;
            if (striptype != RIGHTSTRIP)
            {
                while (i < len && memchr(sep, str[i], charslen))
                {
                    i++;
                }
            }

            j = len;
            if (striptype != LEFTSTRIP)
            {
                do
                {
                    j--;
                } while (j >= i && memchr(sep, str[j], charslen));
                j++;
            }
        }

        if (i == 0 && j == len)
        {
            return str;
        }
        else
        {
            return str.substr(i, j - i);
        }
    }

    void partition(const std::string &str, const std::string &sep, std::vector<std::string> &result)
    {
        result.resize(3);
        int index = find(str, sep);

        if (index < 0)
        {
            result[0] = str;
            result[1] = empty_string;
            result[2] = empty_string;
        }
        else
        {
            result[0] = str.substr(0, index);
            result[1] = sep;
            result[2] = str.substr(index + sep.size(), str.size());
        }
    }

    void rpartition(const std::string &str, const std::string &sep, std::vector<std::string> &result)
    {
        result.resize(3);
        int index = rfind(str, sep);
        if (index < 0)
        {
            result[0] = empty_string;
            result[1] = empty_string;
            result[2] = str;
        }
        else
        {
            result[0] = str.substr(0, index);
            result[1] = sep;
            result[2] = str.substr(index + sep.size(), str.size());
        }
    }

    std::string strip(const std::string &str, const std::string &chars)
    {
        return do_strip(str, BOTHSTRIP, chars);
    }

    std::string lstrip(const std::string &str, const std::string &chars)
    {
        return do_strip(str, LEFTSTRIP, chars);
    }

    std::string rstrip(const std::string &str, const std::string &chars)
    {
        return do_strip(str, RIGHTSTRIP, chars);
    }

    std::string join(const std::string &str, const std::vector<std::string> &seq)
    {
        std::vector<std::string>::size_type seqlen = seq.size(), i;

        if (seqlen == 0)
        {
            return empty_string;
        }

        if (seqlen == 1)
        {
            return seq[0];
        }

        std::string result(seq[0]);

        for (int i = 1; i < seqlen; ++i)
        {
            result += (str + seq[i]);
        }

        return result;
    }

    bool endswith(const std::string &str, const std::string &suffix, int start, int end)
    {
        int result = details::string_tailmatch(str, suffix, (Py_ssize_t)start, (Py_ssize_t)end, +1);

        // if (result == -1) // TODO: Error condition

        return static_cast<bool>(result);
    }

    bool startswith(const std::string &str, const std::string &suffix, int start, int end)
    {
        int result = details::string_tailmatch(str, suffix, (Py_ssize_t)start, (Py_ssize_t)end, -1);

        // if (result == -1) // TODO: Error condition

        return static_cast<bool>(result);
    }

    bool isalnum(const std::string &str)
    {
        std::string::size_type len = str.size();
        if (len == 0)
        {
            return false;
        }

        if (len == 1)
        {
            return ::isalnum(str[0]);
        }

        for (std::string::size_type i = 0; i < len; ++i)
        {
            if (!::isalnum(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool isalpha(const std::string &str)
    {
        std::string::size_type len = str.size();
        if (len == 0)
        {
            return false;
        }

        if (len == 1)
        {
            return ::isalpha((int)str[0]);
        }

        for (std::string::size_type i = 0; i < len; i++)
        {
            if (!::isalpha((int)str[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool isdigit(const std::string &str)
    {
        std::string::size_type len = str.size();
        if (len == 0)
        {
            return false;
        }

        if (len == 1)
        {
            return ::isdigit(str[0]);
        }

        for (std::string::size_type i = 0; i < len; ++i)
        {
            if (!::isdigit(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool islower(const std::string &str)
    {
        std::string::size_type len = str.size();
        if (len == 0)
        {
            return false;
        }
        if (len == 1)
        {
            return ::islower(str[0]);
        }

        for (std::string::size_type i = 0; i < len; ++i)
        {
            if (!::islower(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool isspace(const std::string &str)
    {
        std::string::size_type len = str.size();
        if (len == 0)
        {
            return false;
        }
        if (len == 1)
        {
            return ::isspace(str[0]);
        }

        for (std::string::size_type i = 0; i < len; ++i)
        {
            if (!::isspace(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool istitle(const std::string &str)
    {
        std::string::size_type len = str.size();

        if (len == 0)
        {
            return false;
        }
        if (len == 1)
        {
            return ::isupper(str[0]);
        }

        bool cased = false;
        bool previous_is_cased = false;

        for (std::string::size_type i = 0; i < len; ++i)
        {
            if (::isupper(str[i]))
            {
                if (previous_is_cased)
                {
                    return false;
                }

                previous_is_cased = true;
                cased = true;
            }
            else if (::islower(str[i]))
            {
                if (!previous_is_cased)
                {
                    return false;
                }

                previous_is_cased = true;
                cased = true;
            }
            else
            {
                previous_is_cased = false;
            }
        }

        return cased;
    }

    bool isupper(const std::string &str)
    {
        std::string::size_type len = str.size();
        if (len == 0)
        {
            return false;
        }
        if (len == 1)
        {
            return ::isupper(str[0]);
        }

        for (std::string::size_type i = 0; i < len; ++i)
        {
            if (!::isupper(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    

} // namespace pylike
