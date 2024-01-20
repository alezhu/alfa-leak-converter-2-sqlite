#pragma once

#include "contact.h"
#include "global.h"
#include <QString>

date_t toDate(const QString &value);

QString toDateEx(const date_t value);

contact_type determineContactType(const QString& value);
