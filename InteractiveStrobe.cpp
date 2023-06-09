#include "InteractiveStrobe.h"
#include <QDebug>

InteractiveStrobe::InteractiveStrobe(QChart* chart,const QPointF& pos) : QGraphicsItem(chart), chart(chart)
{
    auto rect = chart->plotArea();
    initPos = pos;
    setPos(initPos);

    initWidth = 100;
    m_rect = QRectF(0, 0, initWidth, 5);
    chartPos = chart->mapToValue(initPos);

    color = Qt::red;


    setZValue(11);
    setFlags(QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
}

InteractiveStrobe::InteractiveStrobe(QChart* chart, QColor color, const QPointF& initPos)
{
    InteractiveStrobe(chart, initPos);
    setColor(color);
}

InteractiveStrobe::~InteractiveStrobe()
{
}

QRectF InteractiveStrobe::boundingRect() const
{

    return m_rect.adjusted(-5, -5, 5, 5);
}

void InteractiveStrobe::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path;
    path.addRoundedRect(m_rect, 2, 2);


    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 1);
    painter->setPen(pen);
    painter->fillPath(path, color);
    painter->drawPath(path);
}

void InteractiveStrobe::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        setCursor(QCursor(Qt::ClosedHandCursor));
        dragStartWidth = m_rect.width();
        *strobeIsChanged = true;
        event->setAccepted(true);
    }
    else {
        event->setAccepted(false);
    }
}

void InteractiveStrobe::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

    if (event->buttons() & Qt::LeftButton) {
        drag(event->buttonDownPos(Qt::LeftButton),event->pos());

        //qDebug() << getLPoint() << getRPoint();
        prepareGeometryChange();
        event->setAccepted(true);
    }
    else {
        event->setAccepted(false);
    }
}

void InteractiveStrobe::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (Qt::LeftButton) {
        setCursor(QCursor(Qt::OpenHandCursor));
        *strobeIsChanged = false;
        event->setAccepted(true);
    }
    else {
        event->setAccepted(false);
    }
}

void InteractiveStrobe::drag(QPointF start, QPointF current)
{
    auto delta = current - start;
    qreal width;
    auto rect = itemRect();
    auto plotArea = chart->plotArea();
    auto mousePos = mapToScene(delta);

    switch (dragMode)
    {
    default:
    case DragMode::None:
        break;
    case DragMode::StretchLeft:
        if (mousePos.x() < plotArea.x()) {
            break;
        }

        width = m_rect.width() - delta.x();
        if (width > minWidth) {
            setX(mousePos.x());
            chartPos = chart->mapToValue(mousePos);
            m_rect.setWidth(width);
        }
        break;
    case DragMode::StretchRight:
        if (mapToScene(current).x() + shift >= plotArea.right() && rect.right() >= plotArea.right()) {
            break;
        }

        width = dragStartWidth + delta.x();
        if (width > minWidth) {
            m_rect.setWidth(width);
        }
        break;
    case DragMode::Move:
        chartPos = chart->mapToValue(mousePos);
        setPos(mousePos);
        break;
    }
}

QPair<QPointF, qreal> InteractiveStrobe::getPosWidth()
{
    return QPair<QPointF, qreal>{ chart->mapToValue(pos()), m_rect.width() };
}

QPointF InteractiveStrobe::getLPoint()
{
    return chart->mapToValue(pos());
}

QPointF InteractiveStrobe::getRPoint()
{
    return chart->mapToValue(itemRect().topRight());
}

void InteractiveStrobe::setColor(QColor color)
{
    this->color = color;
}

void InteractiveStrobe::setStrobeChanged(bool* strobeIsChanged)
{
    this->strobeIsChanged = strobeIsChanged;
}

void InteractiveStrobe::setPosWidth(QPointF pos, qreal width)
{
    chartPos = pos;
    setPos(chart->mapToPosition(pos));
    m_rect.setWidth(width);
}

void InteractiveStrobe::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    InteractiveStrobe::hoverMoveEvent(event);
}

void InteractiveStrobe::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    auto checkLength = mapToScene(event->pos() - pos()).x();
    auto gap = qMin(m_rect.width() * percent, qreal(maxGap));
    auto left = checkLength;
    auto right = m_rect.width() - checkLength;

    if (left < gap) {
        dragMode = DragMode::StretchLeft;
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else
    if (right < gap) {
        dragMode = DragMode::StretchRight;
        setCursor(QCursor(Qt::SizeHorCursor));
        shift = right;
    }
    else {
        dragMode = DragMode::Move;
        setCursor(QCursor(Qt::OpenHandCursor));
    }
}

void InteractiveStrobe::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    dragMode = DragMode::None;
    setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event)
}

QVariant InteractiveStrobe::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && scene()) {
        auto newPos = value.toPointF();
        QRectF newRect = QRectF{ newPos.x(), newPos.y() , m_rect.width(), m_rect.height() };
        QRectF rect = chart->plotArea();
        rect.setWidth(rect.width() - m_rect.width());
        rect.setHeight(rect.height() - m_rect.height());

        if (!rect.contains(newRect))
        {
            newRect.setX(qMin(rect.right(), qMax(newRect.x(), rect.left())));
            newRect.setY(qMin(rect.bottom(), qMax(newRect.y(), rect.top())));
        }

        return newRect.topLeft();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF InteractiveStrobe::itemRect()
{
    QRectF rect = m_rect;
    rect.translate(pos());
    return rect;
}

void InteractiveStrobe::updateGeometry()
{
    prepareGeometryChange();
    m_rect.setWidth(initWidth);
    setPos(chart->mapToPosition(chartPos));
}