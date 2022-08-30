#include "json_object.h"

namespace fjson
{
    void *JsonObject::value()
    {
        switch (this->m_type)
        {
        case T_NULL:
            return std::get_if<string_t>(&m_value);
        case T_BOOL:
            return std::get_if<bool_t>(&m_value);
        case T_INT:
            return std::get_if<int_t>(&m_value);
        case T_DOUBLE:
            return std::get_if<double_t>(&m_value);
        case T_LIST:
            return std::get_if<list_t>(&m_value);
        case T_DICT:
            return std::get_if<dict_t>(&m_value);
        default:
            return nullptr;
        }
    }

#define THROW_GET_ERROR(erron) throw std::logic_error("type error in get " #erron " value!")

    template <class V>
    V &JsonObject::get_value()
    {
        //添加安全检查
        if constexpr (IS_TYPE(V, str_t))
        {
            if (m_type != T_STR)
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
            throw std::logic_error("unknown type in JObject::Value()");
        return *((V *)v); //强转即可
    }

    JsonObject &JsonObject::operator[](const std::string &key)
    {
        if (this->m_type == T_DICT)
        {
            auto &dict = get_value<dict_t>();
            return dict[key];
        }
        throw std::logic_error("not dict type! JObject::opertor[]()");
    }
} // namespace fjson
