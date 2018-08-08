#pragma once

#include <exception>

#define NMS_NM nms

#define NMS_BEGIN namespace NMS_NM{
#define NMS_END }

#define NMS_UNUSED(v) ((void)v)

NMS_BEGIN

#define DEFINE_CHECK_OPERATOR_T1_R(operator, className)\
template <typename T1>\
class className\
{\
    template <typename C1>\
    static std::true_type test(decltype(std::declval<C1>()operator));\
    template <typename C1>\
    static std::false_type test(...);\
public:\
    enum { value = decltype(test<T1>(0))::value };\
};

#define DEFINE_CHECK_OPERATOR_T1_L(operator, className)\
template <typename T1>\
class className\
{\
    template <typename C1>\
    static std::true_type test(decltype(operator std::declval<C1>()));\
    template <typename C1>\
    static std::false_type test(...);\
public:\
    enum { value = decltype(test<T1>(0))::value };\
};

#define DEFINE_CHECK_OPERATOR_T2(operator, className)\
template <typename T1, typename T2>\
class className\
{\
    template <typename C1, typename C2>\
    static std::true_type test(decltype(std::declval<C1>() operator std::declval<C2>()));\
    template <typename C1, typename C2>\
    static std::false_type test(...);\
public:\
    enum { value = decltype(test<T1, T2>(0))::value };\
};

DEFINE_CHECK_OPERATOR_T1_R(++, is_post_incrementable)
DEFINE_CHECK_OPERATOR_T1_R(--, is_post_decrementable)
DEFINE_CHECK_OPERATOR_T1_L(++, is_pre_incrementable)
DEFINE_CHECK_OPERATOR_T1_L(--, is_pre_decrementable)
DEFINE_CHECK_OPERATOR_T1_L(!, is_negatable)
DEFINE_CHECK_OPERATOR_T1_L(~, is_bitwise_negatable)
DEFINE_CHECK_OPERATOR_T1_L((bool), is_bool_convertible)
DEFINE_CHECK_OPERATOR_T1_R((), is_callable)
DEFINE_CHECK_OPERATOR_T1_R([0], can_use_subscript)

DEFINE_CHECK_OPERATOR_T2(| , can_bitwise_or)
DEFINE_CHECK_OPERATOR_T2(^, can_bitwise_xor)
DEFINE_CHECK_OPERATOR_T2(&, can_bitwise_and)
DEFINE_CHECK_OPERATOR_T2(|| , can_or)
DEFINE_CHECK_OPERATOR_T2(&&, can_and)
DEFINE_CHECK_OPERATOR_T2(+=, are_sum_assignable)
DEFINE_CHECK_OPERATOR_T2(-=, are_difference_assignable)
DEFINE_CHECK_OPERATOR_T2(*=, are_product_assignable)
DEFINE_CHECK_OPERATOR_T2(/=, are_quotient_assignable)
DEFINE_CHECK_OPERATOR_T2(%=, are_remainder_assignable)
DEFINE_CHECK_OPERATOR_T2(<<=, are_left_bitwise_shift_assignable)
DEFINE_CHECK_OPERATOR_T2(>>=, are_right_bitwise_shift_assignable)
DEFINE_CHECK_OPERATOR_T2(&=, are_bitwise_and_assignable)
DEFINE_CHECK_OPERATOR_T2(^=, are_bitwise_xor_assignable)
DEFINE_CHECK_OPERATOR_T2(|=, are_bitwise_or_assignable)

DEFINE_CHECK_OPERATOR_T2(*, are_multiplicable)
DEFINE_CHECK_OPERATOR_T2(/ , are_divisible)
DEFINE_CHECK_OPERATOR_T2(%, are_modulable)
DEFINE_CHECK_OPERATOR_T2(+, are_addable)
DEFINE_CHECK_OPERATOR_T2(-, are_subtractable)

DEFINE_CHECK_OPERATOR_T2(<< , are_left_bitwise_shiftable)
DEFINE_CHECK_OPERATOR_T2(>> , are_right_bitwise_shiftable)

DEFINE_CHECK_OPERATOR_T2(== , are_equality_comparable)
DEFINE_CHECK_OPERATOR_T2(!= , are_inequality_comparable)

DEFINE_CHECK_OPERATOR_T2(< , are_less_comparable)
    DEFINE_CHECK_OPERATOR_T2(<= , are_less_equal_comparable)

    DEFINE_CHECK_OPERATOR_T2(> , are_greater_comparable)
    DEFINE_CHECK_OPERATOR_T2(>= , are_greater_equal_comparable)

    namespace comparators
{
#define CREATE_COMPARATOR(op, comparator_name, comparability_test, defaultValue)\
    template<class T1, class T2, bool S>\
    struct comparator_name##_impl\
    {\
        bool operator()(const T1& v1, const T2& v2)\
        {\
            NMS_UNUSED(v1);\
            NMS_UNUSED(v2);\
            return defaultValue;\
        }\
    };\
    template<class T1, class T2>\
    struct comparator_name##_impl<T1, T2, true>\
    {\
        bool operator()(const T1& v1, const T2& v2)\
        {\
            return v1 op v2;\
        }\
    };\
    template<class T1, class T2>\
    class comparator_name\
    {\
    public:\
        bool operator()(const T1& v1, const T2& v2)\
        {\
            return comparator(v1, v2);\
        }\
    private:\
        static comparator_name##_impl<T1, T2, comparability_test<T1, T2>::value> comparator;\
    };\
    template<class T1, class T2>\
    comparator_name##_impl<T1, T2, comparability_test<T1, T2>::value> comparator_name<T1, T2>::comparator;

    CREATE_COMPARATOR(==, compare_equal, are_equality_comparable, false)
    CREATE_COMPARATOR(!= , compare_inequal, are_inequality_comparable, true)
    CREATE_COMPARATOR(< , compare_less, are_less_comparable, false)
    CREATE_COMPARATOR(<= , compare_less_equal, are_less_equal_comparable, false)
    CREATE_COMPARATOR(> , compare_greater, are_greater_comparable, false)
    CREATE_COMPARATOR(>= , compare_greater_equal, are_greater_equal_comparable, false)
}

NMS_END