#include "ClickedLabel.h"

ClickedLabel::ClickedLabel(QWidget* parent, Qt::WindowFlags f) :QLabel(parent)
{
}
ClickedLabel::~ClickedLabel()
{
}
void ClickedLabel::mouseReleaseEvent(QMouseEvent* ev)
{
    emit clicked();
}