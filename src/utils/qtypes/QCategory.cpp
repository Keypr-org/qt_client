#include "QCategory.h"

QCategory::QCategory(const std::unique_ptr<Category> &category) : category(category)
{
}

const qint64 QCategory::getId() const
{
    return category->getId();
}

const QString QCategory::getName() const
{
    return QString::fromStdString(category->getName());
}