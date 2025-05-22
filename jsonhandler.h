#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QString>
#include <QList>
#include "sessiondata.h"

class JsonHandler
{
public:
    static QList<FocusSession> readJson(const QString& filePath);
    static void writeJson(const QString& filePath, const QList<FocusSession>& sessions);
    static DistractionData readDistractionFromFile(const QString& path);//读取python写的中间json数据
    static void appendSession(const QString& filePath, const FocusSession& session);

};




#endif // JSONHANDLER_H
