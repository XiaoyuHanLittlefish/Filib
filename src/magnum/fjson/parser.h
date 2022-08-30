#ifndef FJSON_PARSER_H__
#define FJSON_PARSER_H__

#include "json_object.h"

namespace fjson
{
    class Parser
    {
#define FUNC_TO_NAME _to_json
#define FUNC_FROM_NAME _from_json

#define START_TO_JSON                               \
    void FUNC_TO_NAME(fjson::JsonObject &obj) const \
    {
#define to(key) obj[key]
        // push一个自定义类型的成员
#define to_struct(key, struct_member)         \
    fjson::JsonObject tmp((fjson::dict_t())); \
    struct_member.FUNC_TO_NAME(tmp);          \
    obj[key] = tmp
#define END_TO_JSON }

#define START_FROM_JSON                         \
    void FUNC_FROM_NAME(fjson::JsonObject &obj) \
    {
#define from(key, type) obj[key].get_value<type>()
#define from_struct(key, struct_member) struct_member.FUNC_FROM_NAME(obj[key])
#define END_FROM_JSON }

    public:
        Parser() = default;

        void init(std::string_view src);

        JsonObject parse();
        JsonObject parse_null();
        bool parse_bool();
        JsonObject parse_number();
        JsonObject parse_string();
        JsonObject parse_list();
        JsonObject parse_dict();

        char get_next_token();
        void trim_right();
        void skip_comment();
        static JsonObject from_string(std::string_view content);
        bool is_esc_consume(size_t pos);

        template <class T>
        static std::string ToJSON(const T &src)
        {
            if constexpr (IS_TYPE(T, int_t))
            {
                JsonObject object(src);
                return object.to_string();
            }
            else if constexpr (IS_TYPE(T, bool_t))
            {
                JsonObject object(src);
                return object.to_string();
            }
            else if constexpr (IS_TYPE(T, double_t))
            {
                JsonObject object(src);
                return object.to_string();
            }
            else if constexpr (IS_TYPE(T, string_t))
            {
                JsonObject object(src);
                return object.to_string();
            }

            fjson::JsonObject obj((fjson::dict_t()));
            src.FUNC_TO_NAME(obj);
            return obj.to_string();
        }

        template <class T>
        static T FromJSON(std::string_view src)
        {
            JsonObject object = from_string(src);
            //如果是基本类型
            if constexpr (is_basic_type<T>())
            {
                return object.template get_value<T>();
            }

            //调用T类型对应的成岩函数
            if (object.get_type() != T_DICT)
                throw std::logic_error("not dict type fromjson");
            T ret;
            ret.FUNC_FROM_NAME(object);
            return ret;
        }

    private:
        std::string m_str;
        int m_idx;
    };
} // namespace fjson

#endif //! FJSON_PARSER_H__