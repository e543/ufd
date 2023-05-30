#pragma once
#include <QLabel>
#include <QWidget>

class ClickedLabel : public QLabel
{
    Q_OBJECT
    bool click = false;
public:
    explicit ClickedLabel(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickedLabel();
    bool isClicked();
signals:
    void clicked();
protected:
    void mouseReleaseEvent(QMouseEvent* ev);
};