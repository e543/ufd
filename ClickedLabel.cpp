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
void ClickedLabel::mouseReleaseEvent(QMouseEvent* ev)
{
    click = !click;
    emit clicked();
}