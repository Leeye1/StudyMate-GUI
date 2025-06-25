#include "jsonhandler.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>



QList<FocusSession> JsonHandler::readJson(const QString& filePath)//参数是文件路径，返回一个 FocusSession 的列表。
{
    QList<FocusSession> sessions;
    QFile file(filePath);//创建文件对象。准备读取。

    if (!file.open(QIODevice::ReadOnly)) {//如果打开失败，报错并返回空列表。
        qWarning() << "Cannot open file for reading:" << filePath;
        return sessions;
    }

    QByteArray data = file.readAll();
    file.close();//读取整个 JSON 文件的内容并关闭文件。

    QJsonDocument doc = QJsonDocument::fromJson(data);//把读取的字符串转为 JSON 对象（QJsonDocument）。必须是一个 JSON 对象（不是数组或错误内容）。
    if (!doc.isObject()) return sessions;

    QJsonArray history = doc.object()["history"].toArray();//提取 history 键下的数组。每个元素是一次 FocusSession。

    for (const QJsonValue& val : history) {
        QJsonObject obj = val.toObject();
        FocusSession session;
        session.datetime = obj["datetime"].toString();
        session.duration_minutes = obj["duration_minutes"].toInt();
        session.focus_minutes = obj["focus_minutes"].toInt();

        QJsonObject dis = obj["distractions"].toObject();
        session.distractions.action0 = dis["action0"].toInt();
        session.distractions.action1 = dis["action1"].toInt();
        session.distractions.action2 = dis["action2"].toInt();
        session.distractions.action3 = dis["action3"].toInt();
        session.distractions.action4 = dis["action4"].toInt();
        session.distractions.total_time = dis["total_time"].toInt();

        sessions.append(session);
    }

    return sessions;
}

void JsonHandler::writeJson(const QString& filePath, const QList<FocusSession>& sessions)
{
    QJsonArray history;//用于保存每条 session。
    //把 distractions 信息打包进一个 JSON 对象。
    for (const FocusSession& session : sessions) {
        QJsonObject dis;
        dis["action0"] = session.distractions.action0;
        dis["action1"] = session.distractions.action1;
        dis["action2"] = session.distractions.action2;
        dis["action3"] = session.distractions.action3;
        dis["action4"] = session.distractions.action4;
        dis["total_time"] = session.distractions.total_time;
    //把每条记录转成 JSON 对象后放进数组中。
        QJsonObject obj;
        obj["datetime"] = session.datetime;
        obj["duration_minutes"] = session.duration_minutes;
        obj["focus_minutes"] = session.focus_minutes;
        obj["distractions"] = dis;

        history.append(obj);
    }

    QJsonObject root;
    root["history"] = history;//构建最终的 JSON 根对象。
    //保存为 JSON 文件。
    QJsonDocument doc(root);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return;
    }

    file.write(doc.toJson());
    file.close();
}

DistractionData JsonHandler::readDistractionFromFile(const QString& path) {
    QFile file(path);
    DistractionData data;

    if (!file.open(QIODevice::ReadOnly)) return data;

    QByteArray bytes = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    QJsonObject obj = doc.object();

    data.action0 = obj["action0"].toInt();
    data.action1 = obj["action1"].toInt();
    data.action2 = obj["action2"].toInt();
    data.action3 = obj["action3"].toInt();
    data.action4 = obj["action4"].toInt();
    data.total_time = obj["total_time"].toInt();

    return data;
}
void JsonHandler::appendSession(const QString& filePath, const FocusSession& session) {
    QList<FocusSession> sessions = readJson(filePath);
    sessions.append(session);
    writeJson(filePath, sessions);
}
