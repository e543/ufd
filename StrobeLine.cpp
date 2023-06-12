#include "StrobeLine.h"

StrobeLine::StrobeLine(QColor color)
{
    this->color = color;
}

StrobeLine::~StrobeLine()
{
}

void StrobeLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path;
    //path.line;


    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 1);
    painter->setPen(pen);
    painter->fillPath(path, color);
    painter->drawPath(path);
}