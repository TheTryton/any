#include "Include/any.h"

NMS_BEGIN

any::any()
{
}

any::any(const any& other)
{
    if (other.m_value)
    {
        m_value = other.m_value->copy(&m_size);
    }
}

any::any(any&& other)
{
    m_value = other.m_value;
    m_size = other.m_size;
    other.m_value = nullptr;
    other.m_size = 0;
}

any::~any()
{
    if(m_value)
    {
        delete m_value;
    }
}

const type_info& any::type_id() const
{
    return m_value->type_id();
}

any& any::operator=(const any& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (m_value)
    {
        delete m_value;
        m_value = nullptr;
    }

    if (other.m_value)
    {
        m_value = other.m_value->copy(&m_size);
    }

    return *this;
}

any& any::operator=(any&& other)
{
    if (this == &other)
    {
        return *this;
    }

    if(m_value)
    {
        delete m_value;
    }

    m_value = other.m_value;
    m_size = other.m_size;
    other.m_value = nullptr;
    other.m_size = 0;

    return *this;
}

bool any::is_null() const
{
    return !m_value;
}

bool any::operator==(const any & other)
{
    if (m_value && other.m_value)
    {
        return *m_value == *other.m_value;
    }
    return false;
}

bool any::operator!=(const any & other)
{
    if (m_value && other.m_value)
    {
        return *m_value != *other.m_value;
    }

    if (!m_value && !other.m_value)
    {
        return true;
    }

    return false;
}

bool any::operator<(const any & other)
{
    if (m_value && other.m_value)
    {
        return *m_value < *other.m_value;
    }
    return false;
}

bool any::operator<=(const any & other)
{
    if (m_value && other.m_value)
    {
        return *m_value <= *other.m_value;
    }
    return false;
}

bool any::operator>(const any & other)
{
    if (m_value && other.m_value)
    {
        return *m_value > *other.m_value;
    }
    return false;
}

bool any::operator>=(const any & other)
{
    if (m_value && other.m_value)
    {
        return *m_value >= *other.m_value;
    }
    return false;
}

NMS_END
