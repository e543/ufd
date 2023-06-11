#pragma once
#include <QtCharts>

class InteractiveStrobe : public QGraphicsItem
{
	QChart* chart;
	QRectF m_rect;

	const qreal minWidth = 30.0f;
	const qreal maxGap = 10.0f;
	const qreal percent = 0.1f;
	qreal shift;
	QPointF chartPos;
	QPointF initPos;
	qreal initWidth;
	qreal dragStartWidth;
	QColor color = Qt::red;
	bool* strobeIsChanged;

	enum DragMode {
		None,
		Move,
		StretchLeft,
		StretchRight
	};
	DragMode dragMode = DragMode::None;
public:
	InteractiveStrobe(QChart* chart,const QPointF& initPos);
	InteractiveStrobe(QChart* chart, QColor color, const QPointF& initPos);
	~InteractiveStrobe();
	void updateGeometry();
	QRectF itemRect();
	void drag(QPointF start, QPointF current);
	QPointF getLPoint();
	QPointF getRPoint();
	void setColor(QColor color);
	void setStrobeChanged(bool* strobeIsChanged);
protected:
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override; 
	void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
};

