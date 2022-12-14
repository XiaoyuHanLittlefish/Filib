#ifndef STL_ALGOBASE_H__
#define STL_ALGOBASE_H__

/**
 * @brief
 * 包含了stl的基本算法
 */

#include <cstring>

#include "iterator.h"
#include "utils.h"

namespace stl
{

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

    // max 取二者中较大值，语义相等时返回第一个参数
    template <class T>
    const T &max(const T &lhs, const T &rhs)
    {
        return lhs < rhs ? rhs : lhs;
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class T, class Compare>
    const T &max(const T &lhs, const T &rhs, Compare comp)
    {
        return comp(lhs, rhs) ? rhs : lhs;
    }

    // min 取二者中的较小值，语义相等时返回第一个参数
    template <class T>
    const T &min(const T &lhs, const T &rhs)
    {
        return rhs < lhs ? rhs : lhs;
    }

    // 重载版本使用函数对象 comp 代替比较操作
    template <class T, class Compare>
    const T &min(const T &lhs, const T &rhs, Compare comp)
    {
        return comp(rhs, lhs) ? rhs : lhs;
    }

    // iter_swap 将两个迭代器所指对象对调
    template <class FIter1, class FIter2>
    void iter_swap(FIter1 lhs, FIter2 rhs)
    {
        stl::swap(*lhs, *rhs);
    }

    // copy 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
    // input_iterator_tag 版本
    template <class InputIter, class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
                                  stl::input_iterator_tag)
    {
        for (; first != last; ++first, ++result)
        {
            *result = *first;
        }
        return result;
    }

    // ramdom_access_iterator_tag 版本
    template <class RandomIter, class OutputIter>
    OutputIter unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
                                  stl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, ++first, ++result)
        {
            *result = *first;
        }
        return result;
    }

    template <class InputIter, class OutputIter>
    OutputIter
    unchecked_copy(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_copy_cat(first, last, result, stl::iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
            std::is_trivially_copy_assignable<Up>::value,
        Up *>::type
    unchecked_copy(Tp *first, Tp *last, Up *result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            std::memmove(result, first, n * sizeof(Up));
        }
        return result + 0;
    }

    template <class InputIter, class OutputIter>
    OutputIter copy(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_copy(first, last, result);
    }

    // copy_backward 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内

} // namespace stl

#endif // !STL_ALGOBASE_H__