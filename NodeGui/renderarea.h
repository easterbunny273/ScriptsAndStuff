#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QMainWindow>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    //enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
    //             Chord, Pie, Path, Text, Pixmap };

    RenderArea(QWidget *parent = 0);

    //QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    //QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void paintNodeWidget(QPainter &painter, std::pair<int, int> box);

private:

    std::vector<std::pair<int, int>> m_boxes;
};
#endif // RENDERAREA_H
