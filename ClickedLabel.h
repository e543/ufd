#pragma once
#include <QLabel>
#include <QWidget>

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickedLabel(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickedLabel();

signals:
    void clicked();
protected:
    void mouseReleaseEvent(QMouseEvent* ev);
};