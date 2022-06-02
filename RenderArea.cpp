#include "RenderArea.h"

const Qt::GlobalColor neutralColor = Qt::black;
const Qt::GlobalColor allyColor = Qt::yellow;
const Qt::GlobalColor enemyColor = Qt::red;
const Qt::GlobalColor ballColor = Qt::black;
const Qt::GlobalColor ballFillColor = Qt::white;

const QRect playArea(PITCH_MARGIN, PITCH_MARGIN, PITCH_LENGTH, PITCH_WIDTH);
const QRect wholeFieldArea(0, 0, TOTAL_FIELD_LENGTH, TOTAL_FIELD_WIDTH);
const QPointF MarginPoint(PITCH_MARGIN, PITCH_MARGIN);

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

    drawWorld();

    label->setPixmap(QPixmap::fromImage(*image));
}

void RenderArea::update() {
    drawWorld();
    label->setPixmap(QPixmap::fromImage(*image));
}

void RenderArea::DrawCircle() {
    QPainter painter(image);
    painter.setWindow(QRect(PITCH_MARGIN, PITCH_MARGIN, TOTAL_FIELD_LENGTH, TOTAL_FIELD_WIDTH));
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

    Action a = playerAI.getIntention();
    QPointF intendedPoint = playerAI.getIntention().getPrefferedPoint() - MarginPoint;
    QPointF currentPoint = playerAI.getCoordinatesPoint();
    QRectF intendedKickableAreaRect = playerAI.getKickableAreaRect(intendedPoint);
    //intendedKickableAreaRect.moveTopLeft(intendedKickableAreaRect.topLeft() + MarginPoint);

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
    painter.drawLine(intendedPoint+MarginPoint, currentPoint+MarginPoint);
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
    painter.drawEllipse(getRectFCircleAtCenter(TOTAL_FIELD_LENGTH/2, TOTAL_FIELD_WIDTH/2, 2*CENTER_CIRCLE_R));
    //center line
    painter.drawLine(QLineF(TOTAL_FIELD_LENGTH/2, PITCH_MARGIN, TOTAL_FIELD_LENGTH/2, TOTAL_FIELD_WIDTH-PITCH_MARGIN));

    //left goal
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(QRectF(PITCH_MARGIN - GOAL_DEPTH, TOTAL_FIELD_WIDTH/2 - GOAL_WIDTH/2, GOAL_DEPTH, GOAL_WIDTH));
    painter.setBrush(Qt::NoBrush);
    //goal area
    painter.drawRect(QRectF(PITCH_MARGIN, TOTAL_FIELD_WIDTH/2 - GOAL_AREA_WIDTH/2, GOAL_AREA_LENGTH, GOAL_AREA_WIDTH));
    //penalty area
    painter.drawRect(QRectF(PITCH_MARGIN, TOTAL_FIELD_WIDTH/2 - PENALTY_AREA_WIDTH/2, PENALTY_AREA_LENGTH, PENALTY_AREA_WIDTH));
    //penalty arc
    painter.drawArc(QRectF(PITCH_MARGIN+PENALTY_SPOT_DIST-PENALTY_ARC_R, TOTAL_FIELD_WIDTH/2-PENALTY_ARC_R, 2*PENALTY_ARC_R, 2*PENALTY_ARC_R), -53*16, 106*16);

    //right goal
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(QRectF(TOTAL_FIELD_LENGTH - PITCH_MARGIN, TOTAL_FIELD_WIDTH/2 - GOAL_WIDTH/2, GOAL_DEPTH, GOAL_WIDTH));
    painter.setBrush(Qt::NoBrush);
    //goal area
    painter.drawRect(QRectF(TOTAL_FIELD_LENGTH - PITCH_MARGIN - GOAL_AREA_LENGTH, TOTAL_FIELD_WIDTH/2 - GOAL_AREA_WIDTH/2, GOAL_AREA_LENGTH, GOAL_AREA_WIDTH));
    //penalty area
    painter.drawRect(QRectF(TOTAL_FIELD_LENGTH - PITCH_MARGIN - PENALTY_AREA_LENGTH, TOTAL_FIELD_WIDTH/2 - PENALTY_AREA_WIDTH/2, PENALTY_AREA_LENGTH, PENALTY_AREA_WIDTH));
    //penalty arc
    painter.drawArc(QRectF(TOTAL_FIELD_LENGTH - PITCH_MARGIN - PENALTY_SPOT_DIST - PENALTY_ARC_R, TOTAL_FIELD_WIDTH/2-PENALTY_ARC_R, 2*PENALTY_ARC_R, 2*PENALTY_ARC_R), 127*16, 106*16);

    //corner arcs, clockwise from top-left
    painter.drawArc(QRectF(PITCH_MARGIN-CORNER_ARC_R/2, PITCH_MARGIN-CORNER_ARC_R/2, CORNER_ARC_R, CORNER_ARC_R), -90*16, 90*16);
    painter.drawArc(QRectF(TOTAL_FIELD_LENGTH - PITCH_MARGIN-CORNER_ARC_R/2, PITCH_MARGIN-CORNER_ARC_R/2, CORNER_ARC_R, CORNER_ARC_R), 180*16, 90*16);
    painter.drawArc(QRectF(TOTAL_FIELD_LENGTH - PITCH_MARGIN-CORNER_ARC_R/2, TOTAL_FIELD_WIDTH - PITCH_MARGIN-CORNER_ARC_R/2, CORNER_ARC_R, CORNER_ARC_R), 90*16, 90*16);
    painter.drawArc(QRectF(PITCH_MARGIN-CORNER_ARC_R/2, TOTAL_FIELD_WIDTH - PITCH_MARGIN-CORNER_ARC_R/2, CORNER_ARC_R, CORNER_ARC_R), 0*16, 90*16);

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
    if (!roles.empty()) {
        QPainter painter(image);
        painter.setWindow(wholeFieldArea);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.setPen(QPen(Qt::yellow, 0.08));

        for (PlayerRole r: roles) {
            //QRectF a = r.getRoleRect(); debug, later delete
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(r.getRoleRect());
            painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
            painter.drawEllipse(r.getRolePoint(), 0.1, 0.1);
        }
    }
}

void RenderArea::drawWorld() {
    qDebug() << "Draw world\n";
    drawField();

    for (Player ally: world->getTeamAlly()) {
        drawPlayer(ally);
    }

    for (Player enemy: world->getTeamEnemy()) {
        drawPlayer(enemy);
    }

    if (bDrawIntentions) {
        for (PlayerAI ally: world->getTeamAlly()) {
            drawIntentions(ally);
        }
    }
    if (bDrawRoleRects) {
        drawRoleRects(playerToDrawRects.getWorldModel()->getFormation());
    }
    if (bDrawGoalDefendPositions) {

    }

    drawBall(world->getBall());
}

QRectF RenderArea::getRectFCircleAtCenter(double x, double y, double radius) {
    return QRectF(x - radius/2, y - radius/2, radius, radius);
}

QRectF RenderArea::getRectFAtCenter(double x, double y, double awidth, double aheight) {
    return QRectF(x - awidth/2, y - aheight/2, awidth, aheight);
}

void RenderArea::setPlayerToDrawRects(int i) {
    qDebug() << "Combobox chosen player" << i << "\n" ;
    playerToDrawRects = world->getTeamAlly()[i];
}

void RenderArea::setDrawRectsState(int i)
{
    if (i == 0)
        bDrawRoleRects = false;
    if (i == 2)
        bDrawRoleRects = true;
    emit world->updateRequired();
}

void RenderArea::setDrawIntentionsState(int i)
{
    if (i == 0)
        bDrawIntentions = false;
    if (i == 2)
        bDrawIntentions = true;
    emit world->updateRequired();
}


void RenderArea::setRandom(int i)
{
    qDebug() << "set random\n";
    if (i == 0)
        bDefaultRandomSeed = false;
    if (i == 2)
        bDefaultRandomSeed = true;
}

void RenderArea::setIntroduceNoises(int i)
{
    qDebug() << "introduce noises\n";
    if (i == 0)
        bIntroduceNoises = false;
    if (i == 2)
        bIntroduceNoises = true;
}

void RenderArea::setLimitVision(int i)
{
    qDebug() << "set limit vision\n";
    if (i == 0)
        bLimitVision = false;
    if (i == 2)
        bLimitVision = true;
}

