#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include <QObject>
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPainter>

class DisplayArea : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayArea(QWidget *parent = nullptr);
    void setList(QList<int> *values);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void updatedList();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QList<int> *dataList;
    int* List;
    double widgetWidth, widgetHeight;
};

#endif // DISPLAYAREA_H
