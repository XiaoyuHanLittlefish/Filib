#ifndef FJSON_JSONOBJECT_H__
#define FJSON_JSONOBJECT_H__

#include <string>
#include <vector>
#include <map>
#include <variant>

namespace fjson
{
    enum TYPE
    {
        T_NULL,
        T_BOOL,
        T_INT,
        T_DOUBLE,
        T_STRING,
        T_LIST,
        T_DICT
    };

    using null_t = std::string;
    using int_t = int32_t;
    using double_t = double;
    using bool_t = bool;
    using string_t = std::string;
    using list_t = std::vector<JsonObject>;
    using dict_t = std::map<std::string, JsonObject>;

    class JsonObject
    {
    public:
        using value_t = std::variant<bool, int_t, double_t, string_t, list_t, dict_t>;

    public:
        JsonObject() : m_type(T_NULL), m_value(true) {}
        void turn_null();
        void turn_bool();
        void turn_int();
        void turn_double();
        void turn_string();
        void turn_list();
        void turn_dict();

        void Null()
        {
            m_type = T_NULL;
            m_value = "null";
        }

    private:
        TYPE m_type;
        value_t m_value;
    };

} // namespace fjson

#endif //! FJSON_JSONOBJECT_H__