#ifndef FJSON_JSONOBJECT_H__
#define FJSON_JSONOBJECT_H__

#include <stdexcept>
#include <utility>
#include <variant>
#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>

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

    class JsonObject;

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
        using value_t = std::variant<bool_t, int_t, double_t, string_t, list_t, dict_t>;

        JsonObject() //默认为null类型
        {
            m_type = T_NULL;
            m_value = "null";
        }

        JsonObject(int_t value)
        {
            Int(value);
        }

        JsonObject(bool_t value)
        {
            Bool(value);
        }

        JsonObject(double_t value)
        {
            Double(value);
        }

        JsonObject(string_t const &value)
        {
            Str(value);
        }

        JsonObject(list_t value)
        {
            List(std::move(value));
        }

        JsonObject(dict_t value)
        {
            Dict(std::move(value));
        }

        void Null()
        {
            m_value = "null";
            m_type = T_NULL;
        }

        void Int(int_t value)
        {
            m_value = value;
            m_type = T_INT;
        }

        void Bool(bool_t value)
        {
            m_value = value;
            m_type = T_BOOL;
        }

        void Double(double_t value)
        {
            m_type = T_DOUBLE;
            m_value = value;
        }

        void Str(std::string_view value)
        {
            m_value = std::string(value);
            m_type = T_STRING;
        }

        void List(list_t value)
        {
            m_value = std::move(value);
            m_type = T_LIST;
        }

        void Dict(dict_t value)
        {
            m_value = std::move(value);
            m_type = T_DICT;
        }

    public:
        template <class V>
        V get_value();

        JsonObject &operator[](const std::string &key);

    private:
        //根据类型获取值的地址，直接硬转为void*类型，然后外界调用Value函数进行类型的强转
        void *value();

    private:
        TYPE m_type;
        value_t m_value;
    };

} // namespace fjson

#endif //! FJSON_JSONOBJECT_H__