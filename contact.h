#pragma once
#include "global.h"
#include <QString>

enum class contact_type { other, phone, email };
struct contact_t
{
    user_id_t user_id;
    contact_type type;
    QString value;
};

