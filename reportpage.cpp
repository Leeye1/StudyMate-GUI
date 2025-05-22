#include "reportpage.h"
#include "ui_reportpage.h"
#include "jsonhandler.h"
#include <QDebug>
#include <QDateTime>

ReportPage::ReportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReportPage)
{
    ui->setupUi(this);

    loadDataFromJson("data.json");

}

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::loadDataFromJson(const QString& path)
{
    QList<FocusSession> sessions = JsonHandler::readJson(path);
    ui->tableReport->setRowCount(sessions.size());

    for (int i = 0; i < sessions.size(); ++i) {
        const FocusSession& s = sessions[i];

        // 有效性判断逻辑：专注时长 > 950% 则为“有效”
        bool effective = (s.focus_minutes >= 0.8 * s.duration_minutes);

        ui->tableReport->setItem(i, 0, new QTableWidgetItem(s.datetime));
        ui->tableReport->setItem(i, 1, new QTableWidgetItem(QString::number(s.duration_minutes)));
        ui->tableReport->setItem(i, 2, new QTableWidgetItem(QString::number(s.distractions.action1)));
        ui->tableReport->setItem(i, 3, new QTableWidgetItem(QString::number(s.distractions.action2)));
        ui->tableReport->setItem(i, 4, new QTableWidgetItem(QString::number(s.distractions.action3)));
        ui->tableReport->setItem(i, 5, new QTableWidgetItem(effective ? "✔" : "✘"));
    }

    ui->tableReport->horizontalHeader()->setStretchLastSection(true);
    ui->tableReport->resizeColumnsToContents();
}

void ReportPage::saveSession() {
    FocusSession session;
    session.datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm ddd");
    session.duration_minutes = tomatoDuration;

    DistractionData distraction = JsonHandler::readDistractionFromFile("temp_distraction.json");
    session.focus_minutes = tomatoDuration - distraction.total_time;
    session.distractions.action1 = distraction.action1;
    session.distractions.action2 = distraction.action2;
    session.distractions.action3 = distraction.action3;
    session.distractions.total_time = distraction.total_time;

    JsonHandler::appendSession("data.json", session);
}

void ReportPage::setTomatoDuration(int duration)
{
    tomatoDuration = duration;
    // 如果需要，更新界面或做其他处理
}

void ReportPage::on_testButton_clicked()
{
    tomatoDuration = 25;  // 模拟25分钟的番茄时长，或者用你自己的变量赋值
    saveSession();

    // 调用加载函数刷新表格
    loadDataFromJson("data.json");
}

