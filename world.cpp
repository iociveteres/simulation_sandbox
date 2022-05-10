#include "world.h"



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
