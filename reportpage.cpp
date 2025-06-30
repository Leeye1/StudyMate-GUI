#include "reportpage.h"
#include "ui_reportpage.h"
#include "jsonhandler.h"
#include <QDebug>
#include <QDateTime>

/*在loadDataFromJson中有一个专注有效性判断需要完善
 *
 */
ReportPage::ReportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReportPage)
{
    ui->setupUi(this);
    this->showMaximized();
    pythonProcess = new QProcess(this);
    loadDataFromJson(dataPath);
    updateTotalStatsTable();
    updateTodayStatsTable();

}

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::updateJSON(){
    saveSession();//->append session
    loadDataFromJson(dataPath);
    updateTotalStatsTable();
    updateTodayStatsTable();
    callUpdataCloud();
}

void ReportPage::loadDataFromJson(const QString& path)
{
    QList<FocusSession> sessions = JsonHandler::readJson(path);
    ui->tableReport->setRowCount(sessions.size());

    for (int i = 0; i < sessions.size(); ++i) {
        FocusSession& s = sessions[i];
        ui->tableReport->setItem(i, 0, new QTableWidgetItem(s.datetime));
        ui->tableReport->setItem(i, 1, new QTableWidgetItem(QString::number(s.duration_minutes)));
        ui->tableReport->setItem(i, 2, new QTableWidgetItem(QString::number(s.distractions.action0)));
        ui->tableReport->setItem(i, 3, new QTableWidgetItem(QString::number(s.distractions.action1)));
        ui->tableReport->setItem(i, 4, new QTableWidgetItem(QString::number(s.distractions.action2)));
        ui->tableReport->setItem(i, 5, new QTableWidgetItem(QString::number(s.distractions.action3)));
        ui->tableReport->setItem(i, 6, new QTableWidgetItem(QString::number(s.distractions.action4)));
        ui->tableReport->setItem(i, 7, new QTableWidgetItem(s.effective ? "✔" : "✘"));
    }

    ui->tableReport->horizontalHeader()->setStretchLastSection(true);
    ui->tableReport->resizeColumnsToContents();
}

void ReportPage::callUpdataCloud(){
    pythonProcess->start("python3", QStringList() << cloudPath);
}

void ReportPage::saveSession() {
    FocusSession session;
    /*
struct DistractionData {
    int action1 = 0;//行为1的分神次数
    int action2 = 0;
    int action3 = 0;
    int total_time = 0;//所有分神动作的累积分神时间，把秒当分钟
};

struct FocusSession {
    QString datetime;//日期+时间
    int duration_minutes = 0;//本次番茄钟时长
    int focus_minutes = 0;//本次专注时间等于durationtime-totaltime
    DistractionData distractions;//distractions: 嵌套了上面定义的 DistractionData。
};
     */
    /*这个函数读取了当前时间，番茄钟持续时间
     * 再从temp_distraction.json文件中读取分神数据
     * 然后把DIstraction data和FocusSession的data都处理完后把数据加载到total_time.json
     */
    session.datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm ddd");
    session.duration_minutes = tomatoDuration;

    DistractionData distraction = JsonHandler::readDistractionFromFile("temp_distraction.json");
    session.focus_minutes = tomatoDuration - distraction.total_time;
    session.distractions.action0 = distraction.action0;
    session.distractions.action1 = distraction.action1;
    session.distractions.action2 = distraction.action2;
    session.distractions.action3 = distraction.action3;
    session.distractions.action4 = distraction.action4;
    session.distractions.total_time = (int) distraction.total_time;
    // 有效性判断逻辑：专注时长 > 90% 则为“有效” 后面还需要增加检测逻辑
    int distractCount = distraction.action0 +
                        distraction.action1 +
                        distraction.action2 +
                        distraction.action3 +
                        distraction.action4 ;
    switch(tomatoDuration){
    case 25:
        session.effective = (distractCount>=4) ? false:(session.focus_minutes >= (0.9 * session.duration_minutes));
        break;
    case 45:
        session.effective = (distractCount>=5) ? false:(session.focus_minutes >= (0.9 * session.duration_minutes));
        break;
    case 60:
        session.effective = (distractCount>=6) ? false:(session.focus_minutes >= (0.9 *  session.duration_minutes));
        break;
    case 90:
        session.effective = (distractCount>=7) ? false:(session.focus_minutes >= (0.9 *  session.duration_minutes));
        break;
    default:
        session.effective = (session.focus_minutes >= (0.9 * session.duration_minutes));
    }

    JsonHandler::appendSession(dataPath, session);
}

void ReportPage::setTomatoDuration(int duration)
{
    tomatoDuration = duration;
    // 如果需要，更新界面或做其他处理
}

void ReportPage::on_testButton_clicked()
{
    tomatoDuration = 25;  // 模拟25分钟的番茄时长，或者用你自己的变量赋值
    updateJSON();
}

void ReportPage::on_returnButton_clicked()
{
    emit this->back();
}

void ReportPage::updateTotalStatsTable(){
    QList<FocusSession> sessions = JsonHandler::readJson(dataPath);

    int totalDuration = 0;
    int totalFocus = 0;
    int sessionCount = sessions.size();

    int totalAction0 = 0;
    int totalAction1 = 0;
    int totalAction2 = 0;
    int totalAction3 = 0;
    int totalAction4 = 0;

    for (const FocusSession& s : sessions) {
        if(s.effective==false){
            totalDuration += s.duration_minutes;
            totalAction0 += s.distractions.action0;
            totalAction1 += s.distractions.action1;
            totalAction2 += s.distractions.action2;
            totalAction3 += s.distractions.action3;
            totalAction4 += s.distractions.action4;
            continue;
        }
        totalDuration += s.duration_minutes;
        totalFocus += s.focus_minutes;

        totalAction0 += s.distractions.action0;
        totalAction1 += s.distractions.action1;
        totalAction2 += s.distractions.action2;
        totalAction3 += s.distractions.action3;
        totalAction4 += s.distractions.action4;
    }

    double avgDuration = sessionCount > 0 ? static_cast<double>(totalFocus) / sessionCount : 0;
    double efficiency = totalDuration > 0 ? static_cast<double>(totalFocus) / totalDuration * 100.0 : 0;

    // 设置表格行列数（只做一次初始化）
    ui->totalReport->setRowCount(1);
    ui->totalReport->setColumnCount(8);

    // 插入数据
    ui->totalReport->setItem(0, 0, new QTableWidgetItem(QString::number(totalDuration)));
    ui->totalReport->setItem(0, 1, new QTableWidgetItem(QString::number(totalFocus)));
    // ui->totalReport->setItem(0, 2, new QTableWidgetItem(QString::number(avgDuration, 'f', 2)));
    ui->totalReport->setItem(0, 2, new QTableWidgetItem(QString::number(efficiency, 'f', 2) + "%"));
    ui->totalReport->setItem(0, 3, new QTableWidgetItem(QString::number(totalAction0)));//不在座位
    ui->totalReport->setItem(0, 4, new QTableWidgetItem(QString::number(totalAction1)));//吃手指
    ui->totalReport->setItem(0, 5, new QTableWidgetItem(QString::number(totalAction2)));//脑头发
    ui->totalReport->setItem(0, 6, new QTableWidgetItem(QString::number(totalAction3)));//玩手机
    ui->totalReport->setItem(0, 7, new QTableWidgetItem(QString::number(totalAction4)));//发呆

}

void ReportPage::updateTodayStatsTable() {
    QList<FocusSession> sessions = JsonHandler::readJson(dataPath);

    int todayDuration = 0;
    int todayFocus = 0;
    int totalAction0 = 0;
    int totalAction1 = 0;
    int totalAction2 = 0;
    int totalAction3 = 0;
    int totalAction4 = 0;

    QDate today = QDate::currentDate();

    for (const FocusSession& s : sessions) {
        // 解析 datetime 日期部分，例如 "2025-05-22 20:00 Thu"
        QString datePart = s.datetime.section(' ', 0, 0);  // 得到 "2025-05-22"
        QDate date = QDate::fromString(datePart, "yyyy-MM-dd");
        if (date == today) {
            if(s.effective==false){
                todayDuration += s.duration_minutes;
                totalAction0 += s.distractions.action0;
                totalAction1 += s.distractions.action1;
                totalAction2 += s.distractions.action2;
                totalAction3 += s.distractions.action3;
                totalAction4 += s.distractions.action4;
                continue;
            }
            todayDuration += s.duration_minutes;
            todayFocus += s.focus_minutes;
            totalAction0 += s.distractions.action0;
            totalAction1 += s.distractions.action1;
            totalAction2 += s.distractions.action2;
            totalAction3 += s.distractions.action3;
            totalAction4 += s.distractions.action4;
        }
    }

    double efficiency = todayDuration > 0 ? static_cast<double>(todayFocus) / todayDuration * 100.0 : 0;

    // 设置表格行列数
    ui->todayReport->setRowCount(1);
    ui->todayReport->setColumnCount(8);

    // 插入数据
    ui->todayReport->setItem(0, 0, new QTableWidgetItem(QString::number(todayDuration)));
    ui->todayReport->setItem(0, 1, new QTableWidgetItem(QString::number(todayFocus)));
    ui->todayReport->setItem(0, 3, new QTableWidgetItem(QString::number(totalAction0)));
    ui->todayReport->setItem(0, 4, new QTableWidgetItem(QString::number(totalAction1)));
    ui->todayReport->setItem(0, 5, new QTableWidgetItem(QString::number(totalAction2)));
    ui->todayReport->setItem(0, 6, new QTableWidgetItem(QString::number(totalAction3)));
    ui->todayReport->setItem(0, 7, new QTableWidgetItem(QString::number(totalAction4)));
    ui->todayReport->setItem(0, 2, new QTableWidgetItem(QString::number(efficiency, 'f', 2) + "%"));
}

