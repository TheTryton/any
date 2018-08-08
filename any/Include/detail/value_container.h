#pragma once

#include "any_helpers.h"

NMS_BEGIN

class value_container
{
protected:
    value_container() = default;
public:
    value_container(const value_container& other) = delete;
    value_container(value_container&& other) = delete;
    virtual ~value_container() = default;
public:
    value_container& operator= (const value_container& other) = delete;
    value_container& operator= (value_container&& other) = delete;
public:
    virtual bool operator==(const value_container& other) = 0;
    virtual bool operator!=(const value_container& other) = 0;
    virtual bool operator<(const value_container& other) = 0;
    virtual bool operator<=(const value_container& other) = 0;
    virtual bool operator>(const value_container& other) = 0;
    virtual bool operator>=(const value_container& other) = 0;
public:
    virtual void* value_ptr() = 0;
    virtual value_container* copy(size_t* size) const = 0;
    virtual const type_info& type_id() const = 0;
};

template<class T>
class value_container_impl final : public value_container
{
public:
    value_container_impl() = delete;
    value_container_impl(const value_container_impl<T>&) = delete;
    value_container_impl(value_container_impl<T>&&) = delete;

    value_container_impl(const T& value):
        m_value(value)
    {
    }
    virtual ~value_container_impl() override = default;
public:
    value_container_impl& operator= (const value_container_impl<T>& other) = delete;
    value_container_impl& operator= (value_container_impl<T>&& other) = delete;
public:
    virtual bool operator==(const value_container& other) override
    {
        if (other.type_id() != type_id())
        {
            return false;
        }

        return equal(m_value, reinterpret_cast<const value_container_impl<T>&>(other).m_value);
    }
    virtual bool operator!=(const value_container& other) override
    {
        if (other.type_id() != type_id())
        {
            return true;
        }

        return inequal(m_value, reinterpret_cast<const value_container_impl<T>&>(other).m_value);
    }
    virtual bool operator<(const value_container& other) override
    {
        if (other.type_id() != type_id())
        {
            return false;
        }

        return less(m_value, reinterpret_cast<const value_container_impl<T>&>(other).m_value);
    }
    virtual bool operator<=(const value_container& other) override
    {
        if (other.type_id() != type_id())
        {
            return false;
        }

        return less_equal(m_value, reinterpret_cast<const value_container_impl<T>&>(other).m_value);
    }
    virtual bool operator>(const value_container& other) override
    {
        if (other.type_id() != type_id())
        {
            return false;
        }

        return greater(m_value, reinterpret_cast<const value_container_impl<T>&>(other).m_value);
    }
    virtual bool operator>=(const value_container& other) override
    {
        if (other.type_id() != type_id())
        {
            return false;
        }

        return greater_equal(m_value, reinterpret_cast<const value_container_impl<T>&>(other).m_value);
    }
public: 
    virtual void* value_ptr() override
    {
        return reinterpret_cast<void*>(&m_value);
    }
    virtual value_container* copy(size_t* size) const override
    {
        return new (nullptr, size) value_container_impl<T>(m_value);
    }
    virtual const type_info& type_id() const override
    {
        return typeid(T);
    }
public:
    static void* operator new(std::size_t sz) = delete;
    static void operator delete(void* ptr) = delete;
    static void* operator new(std::size_t sz, value_container* ptr, size_t* size) {
        if (!*size)
        {
            *size = sz;
            return ::operator new(sz);
        }
        return ptr;
    }
    static void operator delete(void* ptr, value_container* obj_ptr, size_t* size)
    {
        NMS_UNUSED(obj_ptr);
        *size = 0;
        ::operator delete(ptr);
    }
private:
    T m_value;

    static comparators::compare_equal<T, T> equal;
    static comparators::compare_inequal<T, T> inequal;
    static comparators::compare_less<T, T> less;
    static comparators::compare_less_equal<T, T> less_equal;
    static comparators::compare_greater<T, T> greater;
    static comparators::compare_greater_equal<T, T> greater_equal;
};

template<class T>
comparators::compare_equal<T, T> value_container_impl<T>::equal;
template<class T>
comparators::compare_inequal<T, T> value_container_impl<T>::inequal;
template<class T>
comparators::compare_less<T, T> value_container_impl<T>::less;
template<class T>
comparators::compare_less_equal<T, T> value_container_impl<T>::less_equal;
template<class T>
comparators::compare_greater<T, T> value_container_impl<T>::greater;
template<class T>
comparators::compare_greater_equal<T, T> value_container_impl<T>::greater_equal;

NMS_END