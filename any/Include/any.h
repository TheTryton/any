#pragma once

#include <type_traits>
#include <typeinfo>
#include <cstring>

#define NMS_NM nms
#define NMS_BEGIN namespace NMS_NM{
#define NMS_END }

NMS_BEGIN

#define SMALL_DATA_BUFFER_SIZE sizeof(void*)*4
#define TRIVIAL_DATA_BUFFER_SIZE sizeof(void*)*4

#undef small
#undef big

class any final
{
private:
    constexpr size_t roundUp(size_t numToRound, size_t multiple)
    {
        if (multiple == 0)
            return numToRound;

        size_t remainder = numToRound % multiple;
        if (remainder == 0)
            return numToRound;

        return numToRound + multiple - remainder;
    }

    enum class _storage_type
    {
        null,
        trivial,
        small,
        big,
    };

    struct alignas(alignof(void*) * 2) trivial_storage
    {
        char                                                    m_trivial_buffer[TRIVIAL_DATA_BUFFER_SIZE];
    };

    struct alignas(alignof(void*) * 2) small_storage
    {
        char                                                    m_small_buffer[SMALL_DATA_BUFFER_SIZE];
    };

    struct alignas(alignof(void*) * 2) big_storage
    {
        void*                                                   m_ptr           = nullptr;
        size_t                                                  m_capacity      = 0;
    };

    struct irtti
    {
        virtual const type_info& type_id() const
        {
            return typeid(nullptr_t);
        }

        virtual void destruct(void* ptr) const {}

        virtual void copy(void* other, void* ptr) const {}

        virtual void move(void* other, void* ptr) const {}
    };

    template<class T>
    struct rtti : public irtti
    {
        virtual const type_info& type_id() const override
        {
            return typeid(T);
        }

        virtual void destruct(void* ptr) const override
        {
            reinterpret_cast<T*>(ptr)->~T();
        }

        virtual void copy(void* other, void* ptr) const override
        {
            if (std::is_trivially_copyable_v<T>)
            {
                std::memcpy(ptr, other, sizeof(T));
            }
            else
            {
                new (ptr) T(*reinterpret_cast<T*>(other));
            }
        }

        virtual void move(void* other, void* ptr) const override
        {
            if (std::is_trivially_copyable_v<T>)
            {
                std::memcpy(ptr, other, sizeof(T));
            }
            else
            {
                new (ptr) T(std::move(*reinterpret_cast<T*>(other)));
            }
        }
    };

    union
    {
        trivial_storage                                         m_trivial_data;
        small_storage                                           m_small_data;
        big_storage                                             m_big_data;
    };

    alignas(void*) _storage_type                                m_storage_type;
    irtti                                                       m_rtti;

    inline void destruct_existing()
    {
        switch (m_storage_type)
        {
        case _storage_type::trivial:
            m_rtti.destruct(&m_trivial_data.m_trivial_buffer);
            break;
        case _storage_type::small:
            m_rtti.destruct(&m_small_data.m_small_buffer);
            break;
        case _storage_type::big:
            m_rtti.destruct(m_big_data.m_ptr);
            break;
        }
    }
public:
    any():
        m_storage_type(_storage_type::null)
    {
    }
    template<class T>
    any(const T& value)
    {
        set_value(value);
    }
    any(const any& other)
    {
        std::memcpy(&m_rtti, &other.m_rtti, sizeof(irtti));
        m_storage_type = other.m_storage_type;

        switch (m_storage_type)
        {
        case _storage_type::trivial:
            other.m_rtti.copy(const_cast<any&>(other).m_trivial_data.m_trivial_buffer, m_trivial_data.m_trivial_buffer);
            break;
        case _storage_type::small:
            other.m_rtti.copy(const_cast<any&>(other).m_small_data.m_small_buffer, m_small_data.m_small_buffer);
            break;
        case _storage_type::big:
            m_big_data.m_capacity = other.m_big_data.m_capacity;
            m_big_data.m_ptr = std::malloc(m_big_data.m_capacity);
            other.m_rtti.copy(const_cast<any&>(other).m_big_data.m_ptr, m_big_data.m_ptr);
            break;
        }
    }
    any(any&& other) noexcept
    {
        std::memcpy(&m_rtti, &other.m_rtti, sizeof(irtti));
        m_storage_type = other.m_storage_type;

        switch (m_storage_type)
        {
        case _storage_type::trivial:
            other.m_rtti.move(const_cast<any&>(other).m_trivial_data.m_trivial_buffer, m_trivial_data.m_trivial_buffer);
            break;
        case _storage_type::small:
            other.m_rtti.move(const_cast<any&>(other).m_small_data.m_small_buffer, m_small_data.m_small_buffer);
            break;
        case _storage_type::big:
            m_big_data.m_capacity = other.m_big_data.m_capacity;
            m_big_data.m_ptr = other.m_big_data.m_ptr;
            break;
        }

        new (&other.m_rtti) irtti();
        other.m_storage_type = _storage_type::null;
    }
    ~any() noexcept
    {
        destruct_existing();
        if (m_storage_type == _storage_type::big)
        {
            delete m_big_data.m_ptr;
        }
    }
public:
    template<class T>
    any& operator= (const T& value)
    {
        set_value(value);

        return *this;
    }
    any& operator= (const any& other)
    {
        destruct_existing();

        std::memcpy(&m_rtti, &other.m_rtti, sizeof(irtti));

        switch (other.m_storage_type)
        {
        case _storage_type::trivial:
            other.m_rtti.copy(const_cast<any&>(other).m_trivial_data.m_trivial_buffer, m_trivial_data.m_trivial_buffer);
            break;
        case _storage_type::small:
            other.m_rtti.copy(const_cast<any&>(other).m_small_data.m_small_buffer, m_small_data.m_small_buffer);
            break;
        case _storage_type::big:
            if (m_storage_type == _storage_type::big)
            {
                if (m_big_data.m_capacity >= other.m_big_data.m_capacity)
                {
                    other.m_rtti.copy(const_cast<any&>(other).m_big_data.m_ptr, m_big_data.m_ptr);
                    break;
                }

                delete m_big_data.m_ptr;
            }

            m_big_data.m_capacity = other.m_big_data.m_capacity;
            m_big_data.m_ptr = std::malloc(m_big_data.m_capacity);
            other.m_rtti.copy(const_cast<any&>(other).m_big_data.m_ptr, m_big_data.m_ptr);
            
            break;
        }

        m_storage_type = other.m_storage_type;

        return *this;
    }
    any& operator= (any&& other) noexcept
    {
        destruct_existing();

        std::memcpy(&m_rtti, &other.m_rtti, sizeof(irtti));

        switch (other.m_storage_type)
        {
        case _storage_type::trivial:
            other.m_rtti.move(const_cast<any&>(other).m_trivial_data.m_trivial_buffer, m_trivial_data.m_trivial_buffer);
            break;
        case _storage_type::small:
            other.m_rtti.move(const_cast<any&>(other).m_small_data.m_small_buffer, m_small_data.m_small_buffer);
            break;
        case _storage_type::big:
            if (m_storage_type == _storage_type::big)
            {
                delete m_big_data.m_ptr;
            }
            m_big_data.m_capacity = other.m_big_data.m_capacity;
            m_big_data.m_ptr = other.m_big_data.m_ptr;
            break;
        }

        m_storage_type = other.m_storage_type;
        other.m_storage_type = _storage_type::null;
        new (&other.m_rtti) irtti();

        return *this;
    }
public:
    template<class T>
    bool is_type()
    {
        if (m_storage_type == _storage_type::null)
        {
            return false;
        }

        return m_rtti.type_id() == &typeid(T);
    }

    template<class T>
    inline const T& value()
    {
        switch (m_storage_type)
        {
        case _storage_type::trivial:
            return *reinterpret_cast<T*>(m_trivial_data.m_trivial_buffer);
        case _storage_type::small:
            return *reinterpret_cast<T*>(m_small_data.m_small_buffer);
        case _storage_type::big:
            return *reinterpret_cast<T*>(m_big_data.m_ptr);
        }

        return *(new T());
    }

    template<class T>
    inline void set_value(const T& value)
    {
        if (m_storage_type == _storage_type::big)
        {
            if (sizeof(T) <= m_big_data.m_capacity)
            {
                m_rtti.destruct(m_big_data.m_ptr);
                new (m_big_data.m_ptr) std::remove_reference<T>::type(value);
            }
            else
            {
                m_rtti.destruct(m_big_data.m_ptr);
                delete m_big_data.m_ptr;
                m_big_data.m_capacity = roundUp(sizeof(T), sizeof(void*));
                m_big_data.m_ptr = std::malloc(m_big_data.m_capacity);
                new (m_big_data.m_ptr) std::remove_reference<T>::type(value);
            }
        }
        else
        {
            if (sizeof(T) <= TRIVIAL_DATA_BUFFER_SIZE && std::is_trivially_copyable<T>::value)
            {
                destruct_existing();
                m_storage_type = _storage_type::trivial;
                new (m_trivial_data.m_trivial_buffer) std::remove_reference<T>::type(value);
            }
            else if (sizeof(T) <= SMALL_DATA_BUFFER_SIZE)
            {
                destruct_existing();
                m_storage_type = _storage_type::small;
                new (m_small_data.m_small_buffer) std::remove_reference<T>::type(value);
            }
            else
            {
                destruct_existing();
                m_storage_type = _storage_type::big;
                m_big_data.m_capacity = roundUp(sizeof(T), sizeof(void*));
                m_big_data.m_ptr = std::malloc(m_big_data.m_capacity);
                new (m_big_data.m_ptr) std::remove_reference<T>::type(value);
            }
        }

        new (&m_rtti) rtti<std::remove_reference<T>::type>();
    }

    const type_info& type_id() const
    {
        return m_rtti.type_id();
    }

    bool is_null() const
    {
        return m_storage_type == _storage_type::null;
    }
};

NMS_END