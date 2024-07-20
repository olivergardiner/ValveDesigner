#include "marker.h"

#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>

Marker::Marker(QChart *chart):
    QGraphicsItem(chart),
    m_chart(chart)
{
}

QRectF Marker::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect;
    rect.setLeft(qMin(m_rect.left(), anchor.x()));
    rect.setRight(qMax(m_rect.right(), anchor.x()));
    rect.setTop(qMin(m_rect.top(), anchor.y()));
    rect.setBottom(qMax(m_rect.bottom(), anchor.y()));
    return rect;
}

void Marker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterPath path;
    path.addRoundedRect(m_rect, 5, 5);

    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));

    path.addEllipse(anchor, 3, 3);
    path.addEllipse(anchor, 2, 2);
    path.addEllipse(anchor, 1, 1);

    if (withLegend) {
        painter->setBrush(QColor(255, 255, 255));
        painter->drawPath(path);
        painter->drawText(m_textRect, m_text);
    }
}

void Marker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->setAccepted(true);
}

void Marker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        setPos(mapToParent(event->pos() - event->buttonDownPos(Qt::LeftButton)));
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

bool Marker::getWithLegend() const
{
    return withLegend;
}

void Marker::setWithLegend(bool newWithLegend)
{
    withLegend = newWithLegend;
}

void Marker::setText(const QString &text)
{
    m_text = text;
    QFontMetrics metrics(m_font);
    m_textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
    int shift = m_textRect.width();
    m_textRect.translate(5 - shift, 5);
    prepareGeometryChange();
    m_rect = m_textRect.adjusted(-5, -5, 5, 5);
}

void Marker::setAnchor(QPointF point)
{
    m_anchor = point;
}

void Marker::updateGeometry()
{
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_anchor) + QPoint(-20, -20));
}

