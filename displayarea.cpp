#include "displayarea.h"

#define MIN(x,y) x > y ? y : x


DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    pen = QPen(Qt::black, 5, Qt::PenStyle::SolidLine,
               Qt::PenCapStyle::RoundCap,
               Qt::PenJoinStyle::MiterJoin);
    brush = QBrush(Qt::green, Qt::BrushStyle::SolidPattern);
    this->resize(400, 300);
    widgetWidth = width();
    widgetHeight = height();
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void DisplayArea::paintEvent(QPaintEvent *event)
{
    for(int i = 0; i<dataList->size(); i++)
    {
        qreal height = MIN(dataList->at(i)*5, widgetHeight);
        qreal width = widgetWidth/static_cast<qreal>(dataList->size());
        qreal x = i*width;
        qreal y = (widgetHeight-height);
        QRectF rect(x, y, width, height);
        QPainter painter(this);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(rect);
    }
}
void DisplayArea::setList(QList<int>* values)
{
    dataList = values;
}

void DisplayArea::updatedList()
{
    repaint();
}

QSize DisplayArea::sizeHint() const
{
    return QSize(400, 300);
}
QSize DisplayArea::minimumSizeHint() const
{
    return QSize(100, 100);
}
