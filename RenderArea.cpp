#include "RenderArea.h"


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

    Player* player = new Player(Player::Team::ally, 15, 15, 0);
    DrawPlayer(*player);

    Player* player_1 = new Player(Player::Team::ally, 70, 40, 60);
    DrawPlayer(*player_1);

    label->setPixmap(QPixmap::fromImage(*image));
}

void RenderArea::DrawCircle() {
    QPainter painter(image);
    painter.setWindow(QRect(r_PITCH_MARGIN, r_PITCH_MARGIN, r_TOTAL_IMAGE_WIDTH, r_TOTAL_IMAGE_HEIGHT));
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawChord(QRectF(30, 30, 10, 10), 0, 180*16);

    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawChord(QRectF(30, 30, 10, 10), 180*16, 180*16);

    //painter.drawLine(30, 30, 100, 140);

    painter.drawEllipse(10, 10, 5, 5);
}

void RenderArea::DrawPlayer(Player player) {
    QPainter painter(image);
    painter.setWindow(QRect(r_PITCH_MARGIN, r_PITCH_MARGIN, r_TOTAL_IMAGE_WIDTH, r_TOTAL_IMAGE_HEIGHT));
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF kickableAreaRect = player.getKickableAreaRect();

    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));

    painter.drawChord(kickableAreaRect, player.getAngle()*16, 180*16);

    painter.setPen(QPen(Qt::red));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

    painter.drawChord(kickableAreaRect, (180+player.getAngle())*16, 180*16);

    //painter.drawLine(10, 10, 50, 50);

    //painter.drawEllipse(50, 50, radius, radius);

}

void RenderArea::DrawField() {
    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    //pa

}

