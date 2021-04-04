#ifndef SHAPES_H
#define SHAPES_H

#include<QtWidgets>
#include<QPen>
#include<QColor>

enum ToolType {
    tt_Line = 1,
    tt_Graffiti,
    tt_Rectangle,
    tt_Oval,
    tt_Triangle
};

class Shapes: public QGraphicsItem
{
   // Q_OBJECT
public:
    Shapes(int type, QGraphicsItem *parent = nullptr);
//    Shapes(int type):m_type(type)
//      ,m_strokeColor(Qt::black)
//      ,m_strokeWidth(1.0f)
//      ,m_fillColor(Qt::transparent)
//      ,m_creatorId(-1)
//      ,m_globalId(-1)
//      ,m_localId(generateLocalId())

//    {}
    static int m_idBase;

    virtual ~Shapes(){}
    virtual void setStartPoit(const QPointF &pos)  =0;
    virtual void setEndPoint(const QPointF &pos) =0;
    virtual void setStrokeWidth(float w);
    virtual void setStrokeColor(const QColor &color);
    virtual void setFillColor(const QColor &color);
//    virtual void setStrokeWidth(float w) {m_strokeWidth = w;}
//    virtual void setStrokeColor(const QColor &color) {m_strokeColor = color;}
//    virtual void setFillColor(const QColor &color) {m_fillColor = color;}
    virtual bool isValid(){return true;}

    virtual void serialize(QJsonObject &obj)  = 0;
    static int generateLocalId();

    void setCreator(int id){m_creatorId = id;}
    int creator(){return m_creatorId;}

    void setGlocalId(int id){m_globalId = id;}
    int globalId(){return m_globalId;}

    void setLocalId(int id){m_localId = id;}
    int localId(){return m_localId;}

 protected:
    int m_type;
    float m_strokeWidth;//线宽
    QColor m_strokeColor; //线颜色

    int  m_globalId;
    int m_localId;
    int m_creatorId;
    QColor m_fillColor2;//填充颜色
};

class SLine:public Shapes{
public:
    SLine();
    QRectF boundingRect() const ;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option ,QWidget *widget);


    void setStartPoit(const QPointF &pos);
    void setEndPoint(const QPointF &pos);
    void setStrokeWidth(float w);
    void setStrokeColor(const QColor &color);

    bool isValid() ;
    void serialize(QJsonObject &obj);

public:
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QLineF m_line;
    QRectF m_rcBounding;
    QPen m_pen;
};


class SRectangle:public Shapes
{
public:
    SRectangle(int type = tt_Rectangle);
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option ,QWidget *widget);


    void setStartPoit(const QPointF &pos);
    void setEndPoint(const QPointF &pos);
    void setStrokeWidth(float w);
    void setStrokeColor(const QColor &color);
    void setFillColor(const QColor &color);
    void serialize(QJsonObject &obj);
    bool isValid();

public:
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QPen m_pen;

};

class SOval :public SRectangle
{
public:
    SOval();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option ,QWidget *widget);

};

class STriangle:public SRectangle
{
public:
    STriangle();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option ,QWidget *widget);
};


class SGraffiti:public Shapes
{
public:
    SGraffiti();

    QRectF boundingRect() const ;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option ,QWidget *widget);


    void setStartPoit(const QPointF &pos);
    void setEndPoint(const QPointF &pos);
    void setStrokeWidth(float w);
    void setStrokeColor(const QColor &color);

    void serialize(QJsonObject &obj);
    bool isValid();
    void setPath(QPainterPath &path);

public:
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QPen m_pen;
    QPainterPath m_path;
    QPointF m_topLeftInScene;













};














#endif // SHAPES_H
