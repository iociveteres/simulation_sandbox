#include "renderarea.h"
#include "ModelConstants.h"

RenderArea::RenderArea(QWidget* parent):QWidget(parent)
{
    image = new QImage(d_TOTAL_IMAGE_WIDTH, d_TOTAL_IMAGE_HEIGHT, QImage::Format_RGB32);
    image->fill(Qt::white);
    label = new QLabel();

    setFixedSize(d_TOTAL_IMAGE_WIDTH, d_TOTAL_IMAGE_HEIGHT);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(label);
    setLayout(layout);

    DrawCircle();

    label->setPixmap(QPixmap::fromImage(*image));
}

void RenderArea::DrawCircle() {
    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int radius = 50;
    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawChord(QRect(200, 200, 400, 400), 0, 180*16);

    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawChord(QRect(200, 200, 400, 400), 180*16, 180*16);

    painter.drawLine(30, 30, 100, 140);

    painter.drawEllipse(50, 50, radius, radius);
}

void RenderArea::DrawPlayer() {
    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect playerRect = QRect(200, 200, 400, 400);
    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawChord(, 0, 180*16);

    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawChord(QRect(200, 200, 400, 400), 180*16, 180*16);

    painter.drawLine(30, 30, 100, 140);

    painter.drawEllipse(50, 50, radius, radius);

}