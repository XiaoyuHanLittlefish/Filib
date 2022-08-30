#include "json_object.h"

namespace fjson
{
    JsonObject::JsonObject() //默认为null类型
    {
        m_type = T_NULL;
        m_value = "null";
    }

    JsonObject::JsonObject(int_t value)
    {
        JsonObject::Int(value);
    }

    JsonObject::JsonObject(bool_t value)
    {
        Bool(value);
    }

    JsonObject::JsonObject(double_t value)
    {
        Double(value);
    }

    JsonObject::JsonObject(string_t const &value)
    {
        Str(value);
    }

    JsonObject::JsonObject(list_t value)
    {
        List(std::move(value));
    }

    JsonObject::JsonObject(dict_t value)
    {
        Dict(std::move(value));
    }

    void JsonObject::Null()
    {
        m_value = "null";
        m_type = T_NULL;
    }

    void JsonObject::Int(int_t value)
    {
        m_value = value;
        m_type = T_INT;
    }

    void JsonObject::Bool(bool_t value)
    {
        m_value = value;
        m_type = T_BOOL;
    }

    void JsonObject::Double(double_t value)
    {
        m_type = T_DOUBLE;
        m_value = value;
    }

    void JsonObject::Str(std::string_view value)
    {
        m_value = std::string(value);
        m_type = T_STRING;
    }

    void JsonObject::List(list_t value)
    {
        m_value = std::move(value);
        m_type = T_LIST;
    }

    void JsonObject::Dict(dict_t value)
    {
        m_value = std::move(value);
        m_type = T_DICT;
    }

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
        case T_STRING:
            return std::get_if<string_t>(&m_value);
        default:
            return nullptr;
        }
    }

    JsonObject &JsonObject::operator[](const std::string &key)
    {
        if (this->m_type == T_DICT)
        {
            auto &dict = get_value<dict_t>();
            return dict[key];
        }
        throw std::logic_error("not dict type! JsonObject::opertor[]()");
    }

    void JsonObject::push_back(JsonObject item)
    {
        if (this->m_type == T_LIST)
        {
            auto &list = get_value<list_t>();
            list.push_back(std::move(item));
            return;
        }
        throw std::logic_error("not a list type! JsonObject::push_back()");
    }

#define GET_VALUE(type) *((type *)value)

    std::string JsonObject::to_string()
    {
        void *value = this->value();
        std::ostringstream os;
        switch (this->m_type)
        {
        case T_NULL:
            os << "null";
            break;
        case T_BOOL:
            if (GET_VALUE(bool_t))
            {
                os << "true";
            }
            else
            {
                os << "false";
            }
            break;
        case T_INT:
            os << GET_VALUE(int_t);
            break;
        case T_DOUBLE:
            os << GET_VALUE(double_t);
            break;
        case T_STRING:
            os << '\"' << GET_VALUE(string_t) << '\"';
            break;
        case T_LIST:
        {
            list_t list = GET_VALUE(list_t);
            os << '[';
            for (auto i = 0; i < list.size(); i++)
            {
                if (i != list.size() - 1)
                {
                    os << ((list[i]).to_string());
                    os << ',';
                }
                else
                {
                    os << ((list[i]).to_string());
                }
            }
            os << ']';
            break;
        }
        case T_DICT:
        {
            dict_t &dict = GET_VALUE(dict_t);
            os << '{';
            for (auto it = dict.begin(); it != dict.end(); ++it)
            {
                if (it != dict.begin()) //为了保证最后的json格式正确
                    os << ',';
                os << '\"' << it->first << "\":" << it->second.to_string();
            }
            os << '}';
            break;
        }
        default:
            return "";
        }
        return os.str();
    }
} // namespace fjson
