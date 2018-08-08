#pragma once

#include <type_traits>
#include <cassert>
#include "detail/value_container.h"

NMS_BEGIN

class any final
{
public:
    any();
    template<class T>
    any(const T& other)
    {
        m_size = sizeof(value_container_impl<T>);
        m_value = new value_container_impl<T>(other);
    }
    any(const any& other);
    any(any&& other);
    ~any();
public:
    template<class T>
    any& operator= (const T& value)
    {
        if (!m_value)
        {
            m_value = new (nullptr, &m_size) value_container_impl<T>(value);
        }
        else
        {
            if (m_value->type_id() == typeid(T))
            {
                *reinterpret_cast<T*>(m_value->value_ptr()) = value;
            }
            else
            {
                if (m_size >= sizeof(value_container_impl<T>))
                {
                    m_value->~value_container();
                    m_value = new (m_value, &m_size) value_container_impl<T>(value);
                }
                else
                {
                    m_size = 0;

                    if (m_value)
                    {
                        delete m_value;
                    }

                    m_value = new (m_value, &m_size) value_container_impl<T>(value);
                }
            }
        }

        return *this;
    }
    any& operator= (const any& other);
    any& operator= (any&& other);
public:
    template<class T>
    bool is_type()
    {
        if (!m_value)
        {
            return false;
        }

        return m_value->type_id() == typeid(T);
    }

    template<class T>
    T value()
    {
        assert(m_value);
        return *reinterpret_cast<T*>(m_value->value_ptr());
    }
    template<class T>
    void set_value(const T& value)
    {
        operator=(value);
    }

    const type_info& type_id() const;
    bool is_null() const;
public:
    //comparators
    bool operator==(const any& other);
    bool operator!=(const any& other);

    bool operator<(const any& other);
    bool operator<=(const any& other);

    bool operator>(const any& other);
    bool operator>=(const any& other);
private:
    value_container* m_value = nullptr;
    size_t m_size = 0;
};

NMS_END