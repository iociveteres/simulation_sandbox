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

    if (json.contains("npcs") && json["npcs"].isArray()) {
        QJsonArray teamAllyArray = json["npcs"].toArray();
        teamAlly.clear();
        teamAlly.reserve(teamAllyArray.size());
        for (int playerIndex = 0; playerIndex < teamAllyArray.size(); ++playerIndex) {
            QJsonObject playerObject = teamAllyArray[playerIndex].toObject();
            Player player;
            player.readJSON(playerObject);
            teamAlly.append(player);
        }
    }
}

void World::writeJSON(QJsonObject &json) const
{
    //json["name"] = mName;
    QJsonArray teamAllyArray;
    for (const Player &player : teamAlly) {
        QJsonObject npcObject;
        player.writeJSON(npcObject);
        teamAllyArray.append(npcObject);
    }
    json["npcs"] = teamAllyArray;
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

    return true;
}

World World::getWorld()
{
    return *this;
}

void World::populate() {
    teamAlly.append(Player(Player::Team::ally, 30, 20, 90));
    teamAlly.append(Player(Player::Team::ally, 30, 30, 90));
    teamAlly.append(Player(Player::Team::ally, 30, 40, 90));

    teamEnemy.append(Player(Player::Team::enemy, 50, 20, -90));
    teamEnemy.append(Player(Player::Team::enemy, 50, 30, -90));
    teamEnemy.append(Player(Player::Team::enemy, 50, 40, -90));

    ball = new Ball(40, 30);
}
