#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>

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
private slots:
    void on_testButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::ReportPage *ui;
    int tomatoDuration = 0;
    bool effective;
};

#endif // REPORTPAGE_H
