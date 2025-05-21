#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tomatopage.h"
#include "reportpage.h"
#include "monitorpage.h"
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    TomatoPage* tomatoPage;


private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
