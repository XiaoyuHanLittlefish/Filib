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

#define IS_TYPE(typea, typeb) std::is_same<typea, typeb>::value

    template <class T>
    constexpr bool is_basic_type()
    {
        if constexpr (IS_TYPE(T, string_t) ||
                      IS_TYPE(T, bool_t) ||
                      IS_TYPE(T, double_t) ||
                      IS_TYPE(T, int_t))
            return true;

        return false;
    }

    class JsonObject
    {
    public:
        using value_t = std::variant<bool_t, int_t, double_t, string_t, list_t, dict_t>;

        JsonObject();
        JsonObject(int_t value);
        JsonObject(bool_t value);
        JsonObject(double_t value);
        JsonObject(const string_t &value);
        JsonObject(list_t value);
        JsonObject(dict_t value);

        void Null();
        void Int(int_t value);
        void Bool(bool_t value);
        void Double(double_t value);
        void Str(std::string_view value);
        void List(list_t value);
        void Dict(dict_t value);

    public:
#define THROW_GET_ERROR(erron) throw std::logic_error("type error in get " #erron " value!")

        template <class V>
        V &get_value()
        {
            //添加安全检查
            if constexpr (IS_TYPE(V, string_t))
            {
                if (m_type != T_STRING)
                    THROW_GET_ERROR(string);
            }
            else if constexpr (IS_TYPE(V, bool_t))
            {
                if (m_type != T_BOOL)
                    THROW_GET_ERROR(BOOL);
            }
            else if constexpr (IS_TYPE(V, int_t))
            {
                if (m_type != T_INT)
                    THROW_GET_ERROR(INT);
            }
            else if constexpr (IS_TYPE(V, double_t))
            {
                if (m_type != T_DOUBLE)
                    THROW_GET_ERROR(DOUBLE);
            }
            else if constexpr (IS_TYPE(V, list_t))
            {
                if (m_type != T_LIST)
                    THROW_GET_ERROR(LIST);
            }
            else if constexpr (IS_TYPE(V, dict_t))
            {
                if (m_type != T_DICT)
                    THROW_GET_ERROR(DICT);
            }

            void *v = value();
            if (v == nullptr)
                throw std::logic_error("unknown type in JsonObject::get_value()");
            return *((V *)v); //强转即可
        }

        TYPE get_type()
        {
            return m_type;
        }

        JsonObject &operator[](const std::string &key);
        void push_back(JsonObject item);
        std::string to_string();

    private:
        //根据类型获取值的地址，直接硬转为void*类型，然后外界调用Value函数进行类型的强转
        void *value();

    private:
        TYPE m_type;
        value_t m_value;
    };

} // namespace fjson

#endif //! FJSON_JSONOBJECT_H__