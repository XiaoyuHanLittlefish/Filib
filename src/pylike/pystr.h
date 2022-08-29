#ifndef PYLIKE_PYSTR_H__
#define PYLIKE_PYSTR_H__

/**
 * @file pystr.h
 * @author littlefishovo (littlefishovo@outlook.com)
 * @brief 包含了python风格的字符串函数
 * @version 0.1
 * @date 2022-08-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <string>
#include <vector>

namespace pylike
{

#define MAX_32BIT_INT 2147483647

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string with only its first character capitalized.
    ///
    std::string capitalize(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return centered in a string of length width. Padding is done using spaces.
    ///
    std::string center(const std::string &str, int width);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the number of occurrences of substring sub in string S[start:end]. Optional
    /// arguments start and end are interpreted as in slice notation.
    ///
    int count(const std::string &str, const std::string &substr, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return True if the string ends with the specified suffix, otherwise return False. With
    /// optional start, test beginning at that position. With optional end, stop comparing at that position.
    ///
    bool endswith(const std::string &str, const std::string &suffix, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string where all tab characters are expanded using spaces. If tabsize
    /// is not given, a tab size of 8 characters is assumed.
    ///
    std::string expandtabs(const std::string &str, int tabsize = 8);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the lowest index in the string where substring sub is found, such that sub is
    /// contained in the range [start, end). Optional arguments start and end are interpreted as
    /// in slice notation. Return -1 if sub is not found.
    ///
    int find(const std::string &str, const std::string &sub, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Synonym of find right now. Python version throws exceptions. This one currently doesn't
    ///
    int index(const std::string &str, const std::string &sub, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if all characters in the string are alphanumeric and there is at least one
    /// character, false otherwise.
    ///
    bool isalnum(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if all characters in the string are alphabetic and there is at least one
    /// character, false otherwise
    ///
    bool isalpha(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if all characters in the string are digits and there is at least one
    /// character, false otherwise.
    ///
    bool isdigit(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if all cased characters in the string are lowercase and there is at least one
    /// cased character, false otherwise.
    ///
    bool islower(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if there are only whitespace characters in the string and there is at least
    /// one character, false otherwise.
    ///
    bool isspace(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if the string is a titlecased string and there is at least one character,
    /// i.e. uppercase characters may only follow uncased characters and lowercase characters only
    /// cased ones. Return false otherwise.
    ///
    bool istitle(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return true if all cased characters in the string are uppercase and there is at least one
    /// cased character, false otherwise.
    ///
    bool isupper(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a string which is the concatenation of the strings in the sequence seq.
    /// The separator between elements is the str argument
    ///
    std::string join(const std::string &str, const std::vector<std::string> &seq);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the string left justified in a string of length width. Padding is done using
    /// spaces. The original string is returned if width is less than str.size().
    ///
    std::string ljust(const std::string &str, int width);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string converted to lowercase.
    ///
    std::string lower(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string with leading characters removed. If chars is omitted or None,
    /// whitespace characters are removed. If given and not "", chars must be a string; the
    /// characters in the string will be stripped from the beginning of the string this method
    /// is called on (argument "str" ).
    ///
    std::string lstrip(const std::string &str, const std::string &chars = "");

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string, concatenated N times, together.
    /// Corresponds to the __mul__ operator.
    ///
    std::string mul(const std::string &str, int n);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Split the string around first occurance of sep.
    /// Three strings will always placed into result. If sep is found, the strings will
    /// be the text before sep, sep itself, and the remaining text. If sep is
    /// not found, the original string will be returned with two empty strings.
    ///
    void partition(const std::string &str, const std::string &sep, std::vector<std::string> &result);
    inline std::vector<std::string> partition(const std::string &str, const std::string &sep)
    {
        std::vector<std::string> result;
        partition(str, sep, result);
        return result;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string with all occurrences of substring old replaced by new. If
    /// the optional argument count is given, only the first count occurrences are replaced.
    ///
    std::string replace(const std::string &str, const std::string &oldstr, const std::string &newstr, int count = -1);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the highest index in the string where substring sub is found, such that sub is
    /// contained within s[start,end]. Optional arguments start and end are interpreted as in
    /// slice notation. Return -1 on failure.
    ///
    int rfind(const std::string &str, const std::string &sub, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Currently a synonym of rfind. The python version raises exceptions. This one currently
    /// does not
    ///
    int rindex(const std::string &str, const std::string &sub, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the string right justified in a string of length width. Padding is done using
    /// spaces. The original string is returned if width is less than str.size().
    ///
    std::string rjust(const std::string &str, int width);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Split the string around last occurance of sep.
    /// Three strings will always placed into result. If sep is found, the strings will
    /// be the text before sep, sep itself, and the remaining text. If sep is
    /// not found, the original string will be returned with two empty strings.
    ///
    void rpartition(const std::string &str, const std::string &sep, std::vector<std::string> &result);
    inline std::vector<std::string> rpartition(const std::string &str, const std::string &sep)
    {
        std::vector<std::string> result;
        rpartition(str, sep, result);
        return result;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string with trailing characters removed. If chars is "", whitespace
    /// characters are removed. If not "", the characters in the string will be stripped from the
    /// end of the string this method is called on.
    ///
    std::string rstrip(const std::string &str, const std::string &chars = "");

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Fills the "result" list with the words in the string, using sep as the delimiter string.
    /// If maxsplit is > -1, at most maxsplit splits are done. If sep is "",
    /// any whitespace string is a separator.
    ///
    void split(const std::string &str, std::vector<std::string> &result, const std::string &sep = "", int maxsplit = -1);
    inline std::vector<std::string> split(const std::string &str, const std::string &sep = "", int maxsplit = -1)
    {
        std::vector<std::string> result;
        split(str, result, sep, maxsplit);
        return result;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Fills the "result" list with the words in the string, using sep as the delimiter string.
    /// Does a number of splits starting at the end of the string, the result still has the
    /// split strings in their original order.
    /// If maxsplit is > -1, at most maxsplit splits are done. If sep is "",
    /// any whitespace string is a separator.
    ///
    void rsplit(const std::string &str, std::vector<std::string> &result, const std::string &sep = "", int maxsplit = -1);
    inline std::vector<std::string> rsplit(const std::string &str, const std::string &sep = "", int maxsplit = -1)
    {
        std::vector<std::string> result;
        rsplit(str, result, sep, maxsplit);
        return result;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a list of the lines in the string, breaking at line boundaries. Line breaks
    /// are not included in the resulting list unless keepends is given and true.
    ///
    void splitlines(const std::string &str, std::vector<std::string> &result, bool keepends = false);
    inline std::vector<std::string> splitlines(const std::string &str, bool keepends = false)
    {
        std::vector<std::string> result;
        splitlines(str, result, keepends);
        return result;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return True if string starts with the prefix, otherwise return False. With optional start,
    /// test string beginning at that position. With optional end, stop comparing string at that
    /// position
    ///
    bool startswith(const std::string &str, const std::string &prefix, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string with leading and trailing characters removed. If chars is "",
    /// whitespace characters are removed. If given not "",  the characters in the string will be
    /// stripped from the both ends of the string this method is called on.
    ///
    std::string strip(const std::string &str, const std::string &chars = "");

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string with uppercase characters converted to lowercase and vice versa.
    ///
    std::string swapcase(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a titlecased version of the string: words start with uppercase characters,
    /// all remaining cased characters are lowercase.
    ///
    std::string title(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string where all characters occurring in the optional argument
    /// deletechars are removed, and the remaining characters have been mapped through the given
    /// translation table, which must be a string of length 256.
    ///
    std::string translate(const std::string &str, const std::string &table, const std::string &deletechars = "");

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string converted to uppercase.
    ///
    std::string upper(const std::string &str);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the numeric string left filled with zeros in a string of length width. The original
    /// string is returned if width is less than str.size().
    ///
    std::string zfill(const std::string &str, int width);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief function matching python's slice functionality.
    ///
    std::string slice(const std::string &str, int start = 0, int end = MAX_32BIT_INT);

} // namespace pylike

#endif // !PYLIKE_PYSTR_H__