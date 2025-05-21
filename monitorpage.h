#ifndef MONITORPAGE_H
#define MONITORPAGE_H

#include <QWidget>

namespace Ui {
class MonitorPage;
}

class MonitorPage : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorPage(QWidget *parent = nullptr);
    ~MonitorPage();

private:
    Ui::MonitorPage *ui;
};

#endif // MONITORPAGE_H
