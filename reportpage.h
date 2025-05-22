#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>

namespace Ui {
class ReportPage;
}

class ReportPage : public QWidget
{
    Q_OBJECT

public slots:
    void setTomatoDuration(int duration);

public:
    explicit ReportPage(QWidget *parent = nullptr);
    ~ReportPage();
    void loadDataFromJson(const QString& path);
    void saveSession();

private slots:
    void on_testButton_clicked();

private:
    Ui::ReportPage *ui;
    int tomatoDuration = 0;
};

#endif // REPORTPAGE_H
