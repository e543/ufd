#include "SecondaryWidget.h"


SecondaryWidget::SecondaryWidget(QWidget* parent, QChart* chart) : ChannelWidget(parent, chart)
{
    QValueAxis* axisX = qobject_cast<QValueAxis*>(chart->axisX());
    axisX->setRange(0, 255);
    axisX->setTickCount(7);
    QValueAxis* axisYleft = qobject_cast<QValueAxis*>(chart->axisY());
    axisYleft->show();
    axisYleft->setRange(0, 255);
    axisYleft->setTickCount(7);

    QValueAxis* axisYright = new QValueAxis;
    axisYright->setRange(0, 255);
    axisYright->setTickCount(7);
    chart->addAxis(axisYright, Qt::AlignRight);

    axisYleft->setLabelFormat("%d");
    axisYright->setLabelFormat("%d");
}
SecondaryWidget::~SecondaryWidget()
{
}
