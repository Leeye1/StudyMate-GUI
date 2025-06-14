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
    updateTotalStatsTable();
    updateTodayStatsTable();

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

        // 有效性判断逻辑：专注时长 > 90% 则为“有效” 后面还需要增加检测逻辑
        bool effective = (s.focus_minutes >= 0.9 * s.duration_minutes);

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
    /*
struct DistractionData {
    int action1 = 0;//行为1的分神次数
    int action2 = 0;
    int action3 = 0;
    int total_time = 0;//所有分神动作的累积分神时间
};

struct FocusSession {
    QString datetime;//日期+时间
    int duration_minutes = 0;//本次番茄钟时长
    int focus_minutes = 0;//本次专注时间等于durationtime-totaltime
    DistractionData distractions;//distractions: 嵌套了上面定义的 DistractionData。
};
     */
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
    updateTotalStatsTable();
    updateTodayStatsTable();
    // 调用加载函数刷新表格
    loadDataFromJson("data.json");
}

void ReportPage::updateJSON(){
    saveSession();//->append session
    updateTotalStatsTable();
    updateTodayStatsTable();
    loadDataFromJson("data.json");
}

void ReportPage::on_returnButton_clicked()
{
    emit this->back();
}

void ReportPage::updateTotalStatsTable(){
    QList<FocusSession> sessions = JsonHandler::readJson("data.json");

    int totalDuration = 0;
    int totalFocus = 0;
    int sessionCount = sessions.size();

    int totalAction1 = 0;
    int totalAction2 = 0;
    int totalAction3 = 0;

    for (const FocusSession& s : sessions) {
        totalDuration += s.duration_minutes;
        totalFocus += s.focus_minutes;

        totalAction1 += s.distractions.action1;
        totalAction2 += s.distractions.action2;
        totalAction3 += s.distractions.action3;
    }

    double avgDuration = sessionCount > 0 ? static_cast<double>(totalDuration) / sessionCount : 0;
    double efficiency = totalDuration > 0 ? static_cast<double>(totalFocus) / totalDuration * 100.0 : 0;

    // 设置表格行列数（只做一次初始化）
    ui->totalReport->setRowCount(1);
    ui->totalReport->setColumnCount(7);

    // 插入数据
    ui->totalReport->setItem(0, 0, new QTableWidgetItem(QString::number(totalDuration)));
    ui->totalReport->setItem(0, 1, new QTableWidgetItem(QString::number(totalFocus)));
    ui->totalReport->setItem(0, 2, new QTableWidgetItem(QString::number(avgDuration, 'f', 2)));
    ui->totalReport->setItem(0, 3, new QTableWidgetItem(QString::number(totalAction1)));
    ui->totalReport->setItem(0, 4, new QTableWidgetItem(QString::number(totalAction2)));
    ui->totalReport->setItem(0, 5, new QTableWidgetItem(QString::number(totalAction3)));
    ui->totalReport->setItem(0, 6, new QTableWidgetItem(QString::number(efficiency, 'f', 2) + "%"));
}

void ReportPage::updateTodayStatsTable() {
    QList<FocusSession> sessions = JsonHandler::readJson("data.json");

    int todayDuration = 0;
    int todayFocus = 0;
    int totalAction1 = 0;
    int totalAction2 = 0;
    int totalAction3 = 0;

    QDate today = QDate::currentDate();

    for (const FocusSession& s : sessions) {
        // 解析 datetime 日期部分，例如 "2025-05-22 20:00 Thu"
        QString datePart = s.datetime.section(' ', 0, 0);  // 得到 "2025-05-22"
        QDate date = QDate::fromString(datePart, "yyyy-MM-dd");

        if (date == today) {
            todayDuration += s.duration_minutes;
            todayFocus += s.focus_minutes;

            totalAction1 += s.distractions.action1;
            totalAction2 += s.distractions.action2;
            totalAction3 += s.distractions.action3;
        }
    }

    double efficiency = todayDuration > 0 ? static_cast<double>(todayFocus) / todayDuration * 100.0 : 0;

    // 设置表格行列数
    ui->todayReport->setRowCount(1);
    ui->todayReport->setColumnCount(6);

    // 插入数据
    ui->todayReport->setItem(0, 0, new QTableWidgetItem(QString::number(todayDuration)));
    ui->todayReport->setItem(0, 1, new QTableWidgetItem(QString::number(todayFocus)));
    ui->todayReport->setItem(0, 2, new QTableWidgetItem(QString::number(totalAction1)));
    ui->todayReport->setItem(0, 3, new QTableWidgetItem(QString::number(totalAction2)));
    ui->todayReport->setItem(0, 4, new QTableWidgetItem(QString::number(totalAction3)));
    ui->todayReport->setItem(0, 5, new QTableWidgetItem(QString::number(efficiency, 'f', 2) + "%"));
}

