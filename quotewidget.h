#ifndef QUOTEWIDGET_H
#define QUOTEWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QStringList>
#include <QTimer>

namespace Ui {
class QuoteWidget;
}

class QuoteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuoteWidget(QWidget *parent = nullptr);
    ~QuoteWidget();

private:
    Ui::QuoteWidget *ui;
    QLabel *quoteLabel;
    QStringList quotes;
    QTimer *quoteTimer;
    void updateQuote();
};

#endif // QUOTEWIDGET_H
