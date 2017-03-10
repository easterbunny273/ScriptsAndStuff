#include "QPainter.h"
#include "QMouseEvent"

#include "renderarea.h"

#include <iostream>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    //setMouseTracking(true);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);


    for (auto& box : m_boxes)
        paintNodeWidget(painter, box);


    return;
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    auto x = event->x();
    auto y = event->y();

    if (m_boxes.empty() == false)
    {
        auto& lastItem = m_boxes[m_boxes.size()-1];
        lastItem.first = x - 40;
        lastItem.second = y;
    }

    repaint();
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    m_boxes.emplace_back(std::make_pair<int, int>(event->x() - 40, event->y()));
    repaint();
}

void RenderArea::paintNodeWidget(QPainter &painter, std::pair<int, int> box)
{
    const int WIDTH = 80;
    const int HEIGHT = 120;
    const int HEIGHT_TITLE = 20;

    QPainterPath path;
    path.addRoundedRect(0.5f, 0.5f, WIDTH, HEIGHT, 10, 10);

    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setBackground(QBrush(Qt::white));


    painter.translate(box.first, box.second);
    painter.fillPath(path, QBrush(Qt::red));
    painter.drawPath(path);
    painter.drawLine(0.5f, 0.5f+HEIGHT_TITLE, 0.5f+WIDTH, 0.5f+HEIGHT_TITLE);
    painter.drawText(0.5f, 0.5f, WIDTH, HEIGHT_TITLE, Qt::AlignHCenter | Qt::AlignVCenter, "Test");

    painter.translate(-box.first, -box.second);
}
