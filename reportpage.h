#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class ReportPage;
}

class ReportPage : public QWidget
{
    Q_OBJECT

signals:
    void back();
public slots:
    void setTomatoDuration(int duration);
    void updateJSON();

public:
    explicit ReportPage(QWidget *parent = nullptr);
    ~ReportPage();
    void loadDataFromJson(const QString& path);
    void saveSession();
    void updateTotalStatsTable();
    void updateTodayStatsTable();
    void callUpdataCloud();
private slots:
    void on_testButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::ReportPage *ui;
    int tomatoDuration = 0;
    bool effective;
    //QString dataPath="../dataTransport/data.json";
    QString dataPath="data.json";
    QString cloudPath="../dataTransport/cloud1.py";
    QProcess *pythonProcess;
};

#endif // REPORTPAGE_H
