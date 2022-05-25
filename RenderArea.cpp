#include "RenderArea.h"

const Qt::GlobalColor neutralColor = Qt::black;
const Qt::GlobalColor allyColor = Qt::yellow;
const Qt::GlobalColor enemyColor = Qt::red;
const Qt::GlobalColor ballColor = Qt::black;
const Qt::GlobalColor ballFillColor = Qt::white;

const QRect playArea(r_PITCH_MARGIN, r_PITCH_MARGIN, r_PITCH_LENGTH, r_PITCH_WIDTH);
const QRect wholeFieldArea(0, 0, r_TOTAL_FIELD_LENGTH, r_TOTAL_FIELD_WIDTH);


RenderArea::RenderArea(QWidget* parent, World* _world):QWidget(parent)
{
    world = _world;
    image = new QImage(d_TOTAL_IMAGE_WIDTH, d_TOTAL_IMAGE_HEIGHT, QImage::Format_RGB32);
    image->fill(Qt::white);
    label = new QLabel();

    setFixedSize(d_TOTAL_IMAGE_WIDTH, d_TOTAL_IMAGE_HEIGHT);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(label);
    setLayout(layout);

    drawWorld(false);

    label->setPixmap(QPixmap::fromImage(*image));
}

void RenderArea::update() {
    drawWorld(false);
    label->setPixmap(QPixmap::fromImage(*image));
}

void RenderArea::DrawCircle() {
    QPainter painter(image);
    painter.setWindow(QRect(r_PITCH_MARGIN, r_PITCH_MARGIN, r_TOTAL_FIELD_LENGTH, r_TOTAL_FIELD_WIDTH));
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

void RenderArea::drawPlayer(Player player) {
    QPainter painter(image);
    painter.setWindow(wholeFieldArea);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF kickableAreaRect = player.getKickableAreaRect();
    QPen p{neutralColor};
    p.setJoinStyle(Qt::MiterJoin);
    p.setWidthF(0.1);
    painter.setPen(p);
    painter.setBrush(QBrush(neutralColor, Qt::SolidPattern));
    p.setColor(Qt::red);
    painter.setPen(p);
    // painter.drawEllipse(kickableAreaRect);
    // painter.drawRect(QRect(5, 5, 5, 5));
    // painter.drawChord(QRect(10, 10, 5, 5), 0, 180 * 16);
    p.setColor(Qt::black);
    painter.setPen(p);
    painter.drawChord(kickableAreaRect, player.getAngle() * 16, 180*16);
    // painter.drawChord(kickableAreaRect, 90 * 16, 180*16);

    Qt::GlobalColor playerColor;
    switch (player.getTeam()) {
    case Player::enemy:
        playerColor = enemyColor;
        break;
    case Player::ally:
        playerColor = allyColor;
        break;
    }
    p.setColor(playerColor);
    painter.setPen(p);
    painter.setBrush(QBrush(playerColor, Qt::SolidPattern));

    painter.drawChord(kickableAreaRect, (player.getAngle() + 180) * 16, 180*16);
}

void RenderArea::drawIntentions(PlayerAI playerAI) {
    QPainter painter(image);
    painter.setWindow(wholeFieldArea);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPointF intendedPoint = playerAI.getIntention().getPrefferedPoint();
    QPointF currentPoint = playerAI.getCoordinatesPoint();
    QRectF intendedKickableAreaRect = playerAI.getKickableAreaRect();

    painter.setPen(QPen(neutralColor, 0.2));
    painter.setBrush(Qt::NoBrush);

    painter.drawChord(intendedKickableAreaRect, playerAI.getAngle()*16, 180*16);

    Qt::GlobalColor playerColor;
    switch (playerAI.getTeam()) {
    case Player::enemy:
        playerColor = enemyColor;
        break;
    case Player::ally:
        playerColor = allyColor;
        break;
    }
    painter.setPen(QPen(playerColor, 0.2));
    painter.setBrush(Qt::NoBrush);

    painter.drawChord(intendedKickableAreaRect, (180+playerAI.getAngle())*16, 180*16);

    painter.setPen(QPen(playerColor, 0.2, Qt::DashLine));
    painter.drawLine(intendedPoint, currentPoint);
}

void RenderArea::drawField() {
    QPainter painter(image);
    painter.setWindow(wholeFieldArea);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(126, 200, 80), Qt::SolidPattern));

    painter.drawRect(wholeFieldArea);
    painter.setPen(QPen(Qt::white, 0.15));
    painter.setBrush(Qt::NoBrush);
    //pitch rim
    painter.drawRect(playArea);

    //center circle
    painter.drawEllipse(getRectFCircleAtCenter(r_TOTAL_FIELD_LENGTH/2, r_TOTAL_FIELD_WIDTH/2, 2*r_CENTER_CIRCLE_R));
    //center line
    painter.drawLine(QLineF(r_TOTAL_FIELD_LENGTH/2, r_PITCH_MARGIN, r_TOTAL_FIELD_LENGTH/2, r_TOTAL_FIELD_WIDTH-r_PITCH_MARGIN));

    //left goal
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(QRectF(r_PITCH_MARGIN - r_GOAL_DEPTH, r_TOTAL_FIELD_WIDTH/2 - r_GOAL_WIDTH/2, r_GOAL_DEPTH, r_GOAL_WIDTH));
    painter.setBrush(Qt::NoBrush);
    //goal area
    painter.drawRect(QRectF(r_PITCH_MARGIN, r_TOTAL_FIELD_WIDTH/2 - r_GOAL_AREA_WIDTH/2, r_GOAL_AREA_LENGTH, r_GOAL_AREA_WIDTH));
    //penalty area
    painter.drawRect(QRectF(r_PITCH_MARGIN, r_TOTAL_FIELD_WIDTH/2 - r_PENALTY_AREA_WIDTH/2, r_PENALTY_AREA_LENGTH, r_PENALTY_AREA_WIDTH));
    //penalty arc
    painter.drawArc(QRectF(r_PITCH_MARGIN+r_PENALTY_SPOT_DIST-r_PENALTY_ARC_R, r_TOTAL_FIELD_WIDTH/2-r_PENALTY_ARC_R, 2*r_PENALTY_ARC_R, 2*r_PENALTY_ARC_R), -53*16, 106*16);

    //right goal
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(QRectF(r_TOTAL_FIELD_LENGTH - r_PITCH_MARGIN, r_TOTAL_FIELD_WIDTH/2 - r_GOAL_WIDTH/2, r_GOAL_DEPTH, r_GOAL_WIDTH));
    painter.setBrush(Qt::NoBrush);
    //goal area
    painter.drawRect(QRectF(r_TOTAL_FIELD_LENGTH - r_PITCH_MARGIN - r_GOAL_AREA_LENGTH, r_TOTAL_FIELD_WIDTH/2 - r_GOAL_AREA_WIDTH/2, r_GOAL_AREA_LENGTH, r_GOAL_AREA_WIDTH));
    //penalty area
    painter.drawRect(QRectF(r_TOTAL_FIELD_LENGTH - r_PITCH_MARGIN - r_PENALTY_AREA_LENGTH, r_TOTAL_FIELD_WIDTH/2 - r_PENALTY_AREA_WIDTH/2, r_PENALTY_AREA_LENGTH, r_PENALTY_AREA_WIDTH));
    //penalty arc
    painter.drawArc(QRectF(r_TOTAL_FIELD_LENGTH - r_PITCH_MARGIN - r_PENALTY_SPOT_DIST - r_PENALTY_ARC_R, r_TOTAL_FIELD_WIDTH/2-r_PENALTY_ARC_R, 2*r_PENALTY_ARC_R, 2*r_PENALTY_ARC_R), 127*16, 106*16);

    //corner arcs, clockwise from top-left
    painter.drawArc(QRectF(r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_CORNER_ARC_R, r_CORNER_ARC_R), -90*16, 90*16);
    painter.drawArc(QRectF(r_TOTAL_FIELD_LENGTH - r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_CORNER_ARC_R, r_CORNER_ARC_R), 180*16, 90*16);
    painter.drawArc(QRectF(r_TOTAL_FIELD_LENGTH - r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_TOTAL_FIELD_WIDTH - r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_CORNER_ARC_R, r_CORNER_ARC_R), 90*16, 90*16);
    painter.drawArc(QRectF(r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_TOTAL_FIELD_WIDTH - r_PITCH_MARGIN-r_CORNER_ARC_R/2, r_CORNER_ARC_R, r_CORNER_ARC_R), 0*16, 90*16);

}

void RenderArea::drawBall(Ball *ball)
{
    if (ball == nullptr)
            return;
    QPainter painter(image);

    painter.setWindow(wholeFieldArea);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF ballRect = ball->getBallRect();
    QPen q{ballColor};
    q.setWidthF(0.1);
    painter.setPen(q);
    painter.setBrush(QBrush(ballFillColor, Qt::SolidPattern));
    painter.drawEllipse(ballRect);
}

void RenderArea::drawRoleRects(QVector<PlayerRole> roles)
{
    QPainter painter(image);
    painter.setWindow(wholeFieldArea);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::yellow, 0.05));
    painter.setBrush(Qt::NoBrush);
    for (PlayerRole r: roles) {
        //QRectF a = r.getRoleRect(); debug, later delete
        painter.drawRect(r.getRoleRect());
        painter.drawEllipse(r.getRolePoint(), 0.1, 0.1);
    }
}

void RenderArea::drawWorld(bool doDrawRoleRects = false) {
    drawField();

    if (doDrawRoleRects) {
        drawRoleRects(Player().getRoles());
    }

    for (Player& ally: world->getTeamAlly()) {
        drawPlayer(ally);
    }

    for (Player& enemy: world->getTeamEnemy()) {
        drawPlayer(enemy);
    }

    drawBall(world->getBall());


    //if (world->getCalculated()) {}
}

QRectF RenderArea::getRectFCircleAtCenter(double x, double y, double radius) {
    return QRectF(x - radius/2, y - radius/2, radius, radius);
}

QRectF RenderArea::getRectFAtCenter(double x, double y, double awidth, double aheight) {
    return QRectF(x - awidth/2, y - aheight/2, awidth, aheight);
}
