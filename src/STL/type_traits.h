#ifndef STL_TYPE_TRAITS_H__
#define STL_TYPE_TRAITS_H__

/**
 * @brief
 * 放置了用于提取类型信息的内容
 */

#include <type_traits>

namespace stl
{
    template <class T, T v>
    struct m_integral_constant
    {
        static constexpr T value = v;
    };

    template <bool b>
    using m_bool_constant = m_integral_constant<bool, b>;

    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;
    /**
     * @todo 验证是否与上述代码相同
     */
    // using m_true_type = m_bool_constant<true>;
    // using m_false_type = m_bool_constant<false>;

    template <class T1, class T2>
    struct pair;

    template <class T>
    struct is_pair : stl::m_false_type
    {
    };

    template <class T1, class T2>
    struct is_pair<stl::pair<T1, T2>> : stl::m_true_type
    {
    };

} // namespace stl

#endif // !STL_TYPE_TRAITS_H__
