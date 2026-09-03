#pragma once

#include "entities/Category.h"
#include <QString>

class QCategory
{
public:
    QCategory(const std::unique_ptr<Category> &category);

    const qint64 getId() const;

    const QString getName() const;

private:
    const std::unique_ptr<Category> &category;
};