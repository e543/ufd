#pragma once
#include <QLabel>
#include <QWidget>

class ClickedLabel : public QLabel
{
    Q_OBJECT
    bool click = false;
    quint8 index;
public:
    explicit ClickedLabel(QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickedLabel();
    bool isClicked();
    void setClicked();
    void setNotClicked(); 
    void setIndex(quint8 i);
    quint8 getIndex();
signals:
    void clicked();
protected:
    void mouseReleaseEvent(QMouseEvent* event);
};