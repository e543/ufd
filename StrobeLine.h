#pragma once
#include <QtWidgets>
class StrobeLine : QGraphicsLineItem
{
    QColor color;
public:
    StrobeLine(QColor color);
    ~StrobeLine();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

