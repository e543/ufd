#pragma once
#include <QtWidgets>

#define DEG_STEP 6;

class Ruler : public QWidget
{
	QTimer* timer;
	static constexpr quint32 degStep = DEG_STEP;
	quint32 x;
	qreal xstep;
	QVector<QGraphicsItem*> lines;
	quint32 mm[12];
	int myWidth;
	int degStart;
	int coordStart;
	int count;
public:
	Ruler(QWidget* parent);
	~Ruler();
	void reset();
	void start();
	void stop();
private:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
private slots:
	void run();
};



