#include "quotewidget.h"
#include "ui_quotewidget.h"
#include <QVBoxLayout>
#include <QFont>
#include <QtGlobal>
#include <QRandomGenerator>

QuoteWidget::QuoteWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuoteWidget)
{
    ui->setupUi(this);
    quoteLabel = new QLabel(this);
    quoteLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setPointSize(16);
    font.setItalic(true);
    quoteLabel->setFont(font);

    quotes << "坚持下去，总会遇见更好的自己"
           << "伟大的成果来自坚持"
           << "每一秒都算数"
           << "专注是一种力量"
           << "你正在变得更好";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(quoteLabel);
    setLayout(layout);

    quoteTimer = new QTimer(this);
    connect(quoteTimer, &QTimer::timeout, this, &QuoteWidget::updateQuote);
    quoteTimer->start(5000); // 每8秒换一句

    updateQuote(); // 初始化显示第一句
}

QuoteWidget::~QuoteWidget()
{
    delete ui;
}

void QuoteWidget::updateQuote() {
    int index = QRandomGenerator::global()->bounded(quotes.size());
    quoteLabel->setText(quotes[index]);
}
