#include "json_object.h"

namespace fjson
{
    void JsonObject::turn_null()
    {
        m_type = T_NULL;
        value_t va(true);
        va = 100;
        this->m_value = true;
    }
} // namespace fjson
