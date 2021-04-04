#include "shapes.h"

int Shapes::m_idBase = 0;
int Shapes::generateLocalId()
{
    return ++m_idBase;
}

Shapes::Shapes(int type, QGraphicsItem *parent):
   QGraphicsItem(parent)
  ,m_type(type)
  ,m_strokeWidth(1.0f)
  ,m_strokeColor(Qt::black)

  ,m_globalId(-1)
  ,m_localId(generateLocalId())
  ,m_creatorId(-1)
  ,m_fillColor2(Qt::transparent)
{
   // m_fillColor = Qt::transparent;
}

void Shapes::setStrokeWidth(float w)
{
    m_strokeWidth = w;
}

void Shapes::setStrokeColor(const QColor &color)
{
    m_strokeColor = color;
}

void Shapes::setFillColor(const QColor &color)
{
    m_fillColor2 = color;
}

//SLine
SLine::SLine():Shapes(tt_Line), m_rcBounding(0,0,0,0)
{

}

void SLine::setStartPoit(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void SLine::setEndPoint(const QPointF &pos)
{
    m_endPosScene = pos;
    qreal endx = m_endPosScene.x();
    qreal endy = m_endPosScene.y();
    qreal startx = m_startPosScene.x();
    qreal starty = m_startPosScene.y();
    m_rcBounding.setWidth(qAbs(endx-startx));
    m_rcBounding.setHeight(qAbs(endy-starty));

    QPointF startPoint;
    QPointF endPoint;

    if(endx < startx)
    {
        startx = endx;
        startPoint.setX(m_rcBounding.width());
        endPoint.setX(0);
    }
    else
    {
        startPoint.setX(0);
        endPoint.setX(m_rcBounding.width());
    }

    if(endy < starty)
    {
        starty = endy;
        startPoint.setY(m_rcBounding.height());
        endPoint.setY(0);
    }
    else
    {
        startPoint.setY(0);
        endPoint.setY(m_rcBounding.height());
    }

    m_line.setPoints(startPoint,endPoint);
    setPos(startx,starty);
}


void SLine::setStrokeWidth(float w)
{
    m_strokeWidth = w;
    m_pen.setWidthF(w);
}


void SLine::setStrokeColor(const QColor &color)
{
    m_strokeColor = color;
    m_pen.setColor(color);
}

QRectF SLine::boundingRect() const
{
    return m_rcBounding;
}

void SLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(m_pen);
    painter->drawLine(m_line);
    painter->restore();
}

bool SLine::isValid()
{
    return !m_line.isNull();
}

void SLine::serialize(QJsonObject &obj)
{
    obj.insert("type", QJsonValue("line"));
    QJsonObject data;
    data.insert("color", QJsonValue((qint64)(m_strokeColor.rgba())));
    data.insert("line_width", QJsonValue(m_strokeWidth));
    data.insert("fill_color", QJsonValue((qint64)(m_fillColor2.rgba())));

    QJsonArray points;
    points.append(QJsonValue(m_startPosScene.x()));
    points.append(QJsonValue(m_startPosScene.y()));
    points.append(QJsonValue(m_endPosScene.x()));
    points.append(QJsonValue(m_endPosScene.y()));
    data.insert("points", QJsonValue(points));
    obj.insert("data", QJsonValue(data));
}

SRectangle::SRectangle(int type)
      :Shapes(type),m_rcBounding(0,0,0,0)
{

}

void SRectangle::setStartPoit(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void SRectangle::setEndPoint(const QPointF &pos)
{
    m_endPosScene = pos;
    qreal endx = m_endPosScene.x();
    qreal endy = m_endPosScene.y();
    qreal startx = m_startPosScene.x();
    qreal starty = m_startPosScene.y();

    m_rcBounding.setWidth(qAbs(endx-startx));
    m_rcBounding.setHeight(qAbs(endy-starty));

    if(endx <startx)
    {
        startx = endx;
    }
    if (endy < starty)
    {
        starty = endy;
    }
    setPos(startx,starty);

}

void SRectangle::setStrokeWidth(float w)
{
    m_strokeWidth = w;
    m_pen.setWidthF(w);
}


void SRectangle::setStrokeColor(const QColor &color)
{
    m_strokeColor = color;
    m_pen.setColor(color);
}

void SRectangle::setFillColor(const QColor &color)
{
    m_fillColor2 = color;
}

QRectF SRectangle::boundingRect() const
{
    return m_rcBounding;
}

void SRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if(m_strokeWidth > 0.1)
        painter->setPen(m_pen);
    else
        painter->setPen(Qt::NoPen);

    if(m_fillColor2 == Qt::transparent)
        painter->setBrush(Qt::NoBrush);
    else
        painter->setBrush(m_fillColor2);

    painter->drawRect(m_rcBounding);
    painter->restore();

}

bool SRectangle::isValid()
{
    return m_rcBounding.width() >= 1 && m_rcBounding.height() >= 1;
}

void SRectangle::serialize(QJsonObject &obj)
{
    switch (m_type)
    {
    case tt_Rectangle:
        obj.insert("type",QJsonValue("rect"));
        break;
    case tt_Oval:
        obj.insert("type",QJsonValue("oval"));
        break;
    case tt_Triangle:
        obj.insert("type",QJsonValue("triangle"));
        break;
    default:
        return;
    }

    QJsonObject data;
    data.insert("color",QJsonValue((qint64)m_strokeColor.rgba()));
    data.insert("line_width",QJsonValue(m_strokeWidth));
    data.insert("fill_color",QJsonValue((qint64)m_fillColor2.rgba()));

    QJsonArray points;
    points.append(QJsonValue(m_startPosScene.x()));
    points.append(QJsonValue(m_startPosScene.y()));
    points.append(QJsonValue(m_endPosScene.x()));
    points.append(QJsonValue(m_endPosScene.y()));
    data.insert("points",QJsonValue(points));
    obj.insert("data",QJsonValue(data));
}


SOval::SOval() :SRectangle(tt_Oval)
{

}

void SOval::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if(m_strokeWidth > 0.1)
        painter->setPen(m_pen);
    else
        painter->setPen(Qt::NoPen);

    if(m_fillColor2 == Qt::transparent)
        painter->setBrush(Qt::NoBrush);
    else
        painter->setBrush(m_fillColor2);

    painter->drawEllipse(m_rcBounding);
    painter->restore();
}

STriangle::STriangle():SRectangle(tt_Triangle)
{

}

void STriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if(m_strokeWidth > 0.1)
        painter->setPen(m_pen);
    else
        painter->setPen(Qt::NoPen);

    if(m_fillColor2 == Qt::transparent)
        painter->setBrush(Qt::NoBrush);
    else
        painter->setBrush(m_fillColor2);

    QPointF points[3] = {
        m_rcBounding.bottomLeft(),
        m_rcBounding.bottomRight(),
        QPointF(m_rcBounding.width() / 2,0)
    };

    painter->drawPolygon(points,3);
    painter->restore();
}


SGraffiti::SGraffiti()
    :Shapes(tt_Graffiti),m_rcBounding(0,0,0,0)
{

}

void SGraffiti::setStartPoit(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
    m_path.moveTo(pos);
}


void SGraffiti::setEndPoint(const QPointF &pos)
{
    m_path.lineTo(pos);
    m_rcBounding = m_path.boundingRect();
    m_topLeftInScene = m_rcBounding.topLeft();
    setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0,0);
}


void SGraffiti::setStrokeWidth(float w)
{
    m_strokeWidth = w;
    m_pen.setWidth(w);
}

void SGraffiti::setStrokeColor(const QColor &color)
{
    m_strokeColor = color;
    m_pen.setColor(color);
}

QRectF SGraffiti::boundingRect() const
{
    return m_rcBounding;
}

void SGraffiti::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();
    painter->setPen(m_pen);
    QPainterPath path = m_path.translated(-m_topLeftInScene);
    painter->drawPath(path);
    painter->restore();
}

void SGraffiti::setPath(QPainterPath &path)
{
    m_path = path;
    m_rcBounding = m_path.boundingRect();
    m_topLeftInScene = m_rcBounding.topLeft();
    setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0,0);

}

bool SGraffiti::isValid()
{
    return !m_path.isEmpty();
}


void SGraffiti::serialize(QJsonObject &obj)
{
    obj.insert("type",QJsonValue("graffiti"));

    QJsonObject data;
    data.insert("color", QJsonValue((qint64)m_strokeColor.rgba()));
    data.insert("line_width", QJsonValue(m_strokeWidth));

    QJsonArray points;
    int ptCount = m_path.elementCount();

    QPainterPath::Element e;
    for(int i= 0;i < ptCount; i++)
    {
        e = m_path.elementAt(i);
        points.append(QJsonValue(e.x));
        points.append(QJsonValue(e.y));
    }

    data.insert("points",QJsonValue(points));
    obj.insert("data",QJsonValue(data));
}


