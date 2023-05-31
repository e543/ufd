#include "ClickedLabel.h"

ClickedLabel::ClickedLabel(QWidget* parent, Qt::WindowFlags f) :QLabel(parent)
{
}
ClickedLabel::~ClickedLabel()
{
}
bool ClickedLabel::isClicked()
{
    return click;
}
void ClickedLabel::setClicked()
{
    click = true;
}
void ClickedLabel::setNotClicked()
{
    click = false;
}
void ClickedLabel::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit clicked();
}
void ClickedLabel::setIndex(quint8 i)
{
    index = i;
}
quint8 ClickedLabel::getIndex()
{
    return index;
}