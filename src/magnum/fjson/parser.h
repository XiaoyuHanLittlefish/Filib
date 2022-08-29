#ifndef FJSON_PARSER_H__
#define FJSON_PARSER_H__

#include "json_object.h"

namespace fjson
{
    class Parser
    {
    public:
        JsonObject parse();
        char get_next_token();
        JsonObject parse_null();
        JsonObject parse_bool();
        JsonObject parse_number();
        JsonObject parse_string();
        JsonObject parse_list();
        JsonObject parse_dict();

    private:
        std::string m_str;
        int m_idx;
    };
} // namespace fjson

#endif //! FJSON_PARSER_H__