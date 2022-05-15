#include "World.h"



QVector<Player> World::getTeamAlly() const
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

World::World()
{
    
}

void World::readJSON(const QJsonObject &json)
{
//    if (json.contains("name") && json["name"].isString())
//        mName = json["name"].toString();

    if (json.contains("teamAlly") && json["teamAlly"].isArray()) {
        QJsonArray teamAllyArray = json["teamAlly"].toArray();
        teamAlly.clear();
        teamAlly.reserve(teamAllyArray.size());
        for (int playerIndex = 0; playerIndex < teamAllyArray.size(); ++playerIndex) {
            QJsonObject playerObject = teamAllyArray[playerIndex].toObject();
            Player player;
            player.readJSON(playerObject);
            teamAlly.append(player);
        }
    }
    if (json.contains("teamEnemy") && json["teamEnemy"].isArray()) {
        QJsonArray teamEnemyArray = json["teamEnemy"].toArray();
        teamEnemy.clear();
        teamEnemy.reserve(teamEnemyArray.size());
        for (int playerIndex = 0; playerIndex < teamEnemyArray.size(); ++playerIndex) {
            QJsonObject playerObject = teamEnemyArray[playerIndex].toObject();
            Player player;
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

bool World::loadWorld(World::SaveFormat saveFormat)
{
    QFile loadFile(saveFormat == Json
        ? QStringLiteral("save.json")
        : QStringLiteral("save.dat"));

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

World World::getWorld()
{
    return *this;
}

void World::populate() {
    teamAlly.append(Player(Player::Team::ally, r_PITCH_LENGTH/4, 0, 90));
    teamAlly.append(Player(Player::Team::ally, r_PITCH_LENGTH/4, r_PITCH_WIDTH/2, 90));
    teamAlly.append(Player(Player::Team::ally, r_PITCH_LENGTH/4, r_PITCH_WIDTH, 90));

    teamEnemy.append(Player(Player::Team::enemy, r_PITCH_LENGTH/4*3,0, -90));
    teamEnemy.append(Player(Player::Team::enemy, r_PITCH_LENGTH/4*3, r_PITCH_WIDTH/2, -90));
    teamEnemy.append(Player(Player::Team::enemy, r_PITCH_LENGTH/4*3, r_PITCH_WIDTH, -90));

    ball = new Ball(r_PITCH_LENGTH/2, r_PITCH_WIDTH/2);
}
