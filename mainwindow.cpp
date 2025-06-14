#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tomatoPage= new TomatoPage();
    this->reportPage= new ReportPage();
    connect(tomatoPage, &TomatoPage::tomatoDurationChanged, reportPage, &ReportPage::setTomatoDuration);
    connect(tomatoPage, &TomatoPage::updateData, reportPage, &ReportPage::updateJSON);
    //界面交互逻辑
    connect(ui->tomatoButton,&QPushButton::clicked,[=](){
        //把主窗口隐藏，第二个窗口界面显示
        this->hide();
        this->tomatoPage->show();
    });
    connect(this->tomatoPage,&TomatoPage::back,[=](){
        this->tomatoPage->hide();
        this->show();
    });
    connect(ui->reportButton,&QPushButton::clicked,[=](){
        //把主窗口隐藏，第二个窗口界面显示
        this->hide();
        this->reportPage->show();
    });
    connect(this->reportPage,&ReportPage::back,[=](){
        this->reportPage->hide();
        this->show();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

