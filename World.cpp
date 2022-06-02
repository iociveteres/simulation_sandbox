#include "World.h"

World::World()
{

}

QVector<PlayerAI> World::getTeamAlly() const
{
    return teamAlly;
}

QVector<Player> World::getTeamEnemy() const
{
    return teamEnemy;
}

Ball *World::getBall() const
{
    return ball;
}

QVector<Player> World::makePlayerTeamAllyForWorldView(int forPlayer) const
{
    QVector<Player> a;

    for (PlayerAI p: teamAlly) {
        if (forPlayer == p.getId())
            continue;
        a.append(Player(p.getTeam(), p.getX(), p.getY(), p.getAngle(), p.getId()));
    }

    return a;
}

PlayerAI World::getAllyById(int id) {
    for (PlayerAI a: teamAlly) {
        if (a.getId() == id)
            return a;
    }
    return PlayerAI();
}

void World::readJSON(const QJsonObject &json)
{
//    if (json.contains("name") && json["name"].isString())
//        mName = json["name"].toString();
    Player p;
    p.player_roleMap.clear();
    if (json.contains("teamAlly") && json["teamAlly"].isArray()) {
        QJsonArray teamAllyArray = json["teamAlly"].toArray();
        teamAlly.clear();
        teamAlly.reserve(teamAllyArray.size());
        for (int playerIndex = 0; playerIndex < teamAllyArray.size(); ++playerIndex) {
            QJsonObject playerObject = teamAllyArray[playerIndex].toObject();
            PlayerAI player = PlayerAI(Player::Team::ally, this);
            player.readJSON(playerObject);
            player.choosePlayerRole();
            teamAlly.append(player);
            p.player_roleMap[player.getId()] = player.getAssignedRole();
        }

    }
    if (json.contains("teamEnemy") && json["teamEnemy"].isArray()) {
        QJsonArray teamEnemyArray = json["teamEnemy"].toArray();
        teamEnemy.clear();
        teamEnemy.reserve(teamEnemyArray.size());
        for (int playerIndex = 0; playerIndex < teamEnemyArray.size(); ++playerIndex) {
            QJsonObject playerObject = teamEnemyArray[playerIndex].toObject();
            Player player(nullptr);
            player.readJSON(playerObject);

            teamEnemy.append(player);
        }
    }

   ball = new Ball();
    if (json.contains("ball") && json["ball"].isObject())
        ball->readJSON(json["ball"].toObject());  
}

void World::writeJSON(QJsonObject &json) const
{
    //json["name"] = mName;

    QJsonArray teamAllyArray;
    for (const Player &player : teamAlly) {
        QJsonObject playerObject;
        player.writeJSON(playerObject);
        teamAllyArray.append(playerObject);
    }
    json["teamAlly"] = teamAllyArray;

    QJsonArray teamEnemyArray;
    for (const Player &player : teamEnemy) {
        QJsonObject playerObject;
        player.writeJSON(playerObject);
        teamEnemyArray.append(playerObject);
    }
    json["teamEnemy"] = teamEnemyArray;

    QJsonObject ballObject;
    ball->writeJSON(ballObject);
    json["ball"] = ballObject;
}

bool World::loadWorld(World::SaveFormat saveFormat, QString fileName)
{
    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
        ? QJsonDocument::fromJson(saveData)
        : QJsonDocument(QCborValue::fromCbor(saveData).toMap().toJsonObject()));

    readJSON(loadDoc.object());

    QTextStream(stdout) << "Loaded save for "
                        << loadDoc["player"]["name"].toString()
                        << " using "
                        << (saveFormat != Json ? "CBOR" : "JSON") << "...\n";

    qDebug() << "Succesfull load\n";
    return true;
}

bool World::saveWorld(World::SaveFormat saveFormat) const
{
    QFile saveFile(saveFormat == Json
        ? QStringLiteral("save.json")
        : QStringLiteral("save.dat"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject gameObject;
    writeJSON(gameObject);
    saveFile.write(saveFormat == Json
        ? QJsonDocument(gameObject).toJson()
        : QCborValue::fromJsonValue(gameObject).toCbor());

    qDebug() << "Succesfull save\n";
    return true;
}

//World World::getWorld()
//{
//    return *this;
//}

void World::handlePlayButton()
{
    QList<Action> intendedActions;
    for (PlayerAI& a: teamAlly) {
        a.cycle();
        intendedActions.append(a.getIntention());
    }
    int duplicates = 0;
    for (Action a: intendedActions) {
        for (Action b: intendedActions) {
            if (a == b)
                duplicates++;
        }
        QString action_name;
        switch (a.getActionType()) {
            case a.DefendGoal:
                qDebug() << "Player " << a.getExecutorId() << " defends goal" << "\n";
                break;
            case a.Intercept:
                qDebug() << "Player " << a.getExecutorId() << " intercepts ball" << "\n";
                break;
            case a.Mark:
                qDebug() << "Player " << a.getExecutorId() << "marks enemy " << a.getAgainstId() << "\n";
                break;
            case a.Wait:
                qDebug() << "Player " << a.getExecutorId() << "waits on his position" << "\n";
            break;
        }
    }
    duplicates -= intendedActions.length();
    qDebug() << "There is " << duplicates << "duplicated action out of " << intendedActions.length() << "\n";
    emit updateRequired();
}


void World::populate() {
    teamAlly.append(PlayerAI(Player::Team::ally, PITCH_LENGTH/4, 0, 90, nullptr));
    teamAlly.append(PlayerAI(Player::Team::ally, PITCH_LENGTH/4, PITCH_WIDTH/2, 90, nullptr));
    teamAlly.append(PlayerAI(Player::Team::ally, PITCH_LENGTH/4, PITCH_WIDTH, 90, nullptr));

    teamEnemy.append(Player(Player::Team::enemy, PITCH_LENGTH/4*3,0, -90));
    teamEnemy.append(Player(Player::Team::enemy, PITCH_LENGTH/4*3, PITCH_WIDTH/2, -90));
    teamEnemy.append(Player(Player::Team::enemy, PITCH_LENGTH/4*3, PITCH_WIDTH, -90));

    ball = new Ball(PITCH_LENGTH/2, PITCH_WIDTH/2);
}
