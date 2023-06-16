#include "Ruler.h"
#include <QDebug>

Ruler::Ruler(QWidget* parent) : QWidget(parent)
{

	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(400, 50));
	setMaximumSize(QSize(3000, 60));
	setStyleSheet("border:  1px dashed blue;");
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Ruler::run);
	reset();
}

Ruler::~Ruler()
{

}

void Ruler::reset()
{
	degStart = 0;
	coordStart = 0;
	count = 0;

	for (int i = 0; i < 12; ++i) {
		mm[i] = i * 5;
	}
}

void Ruler::start()
{
	timer->start(5);
}

void Ruler::stop()
{
	timer->stop();
}

void Ruler::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	p.setPen(QPen(Qt::black, 2, Qt::SolidLine));
	p.setFont(QFont("Arial", 12));
	for (int x = degStart, i = 0; x < myWidth + degStart; ++i) {
		int cX = x % myWidth;
		x += xstep;

		if (i % 5 == 0) {
			p.drawLine(width() - cX, 0, width() - cX, height() * 0.3);
			p.drawText(QPointF{ width() - qreal(cX - 5) , height() * 0.35 + 2 }, QString::number((360 - i * 6) % 360));
		}
		else {
			p.drawLine(width() - cX, 0, width() - cX, height() * 0.1);
		}
	}

	for (int x = coordStart, i = 0; x < myWidth + coordStart; ++i) {
		/*if (x % myWidth == 0) {
			coord += 5;
		}*/

		int cX = x % myWidth;
		x += xstep;

		if (i % 5 == 0) {
			p.drawLine(width() - cX, height() * 0.7, width() - cX, height());
			int num = ((60 - i) / 5) % 60 - 1;
			quint32 k = mm[num];
			p.drawText(QPointF{ width() - qreal(cX - 5) , height() * 0.75 }, QString::number(k + 5));
		}
		else {
			p.drawLine(width() - cX, height() * 0.9, width() - cX, height());
		}
	}
}

void Ruler::resizeEvent(QResizeEvent* event)
{
	auto w = width();
	myWidth = (width() / 60) * 60;
	xstep = width() / 60.;
}


void Ruler::run()
{
	if (width())
	{
		degStart += 1;
		degStart %= myWidth;
		if (degStart % 12 == 0) {
			coordStart += 1;
			coordStart %= myWidth;
		}
		if (degStart % 720 == 0) {
			count %= 12;
			mm[count] += 60;
			count += 1;
		}
		update();
	}
}