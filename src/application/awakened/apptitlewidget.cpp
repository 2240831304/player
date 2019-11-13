
#include "apptitlewidget.h"
#include <QHBoxLayout>
#include "systempath.h"
#include <QLabel>
#include <QPushButton>

namespace eink{

    AppTitleWidget::AppTitleWidget(QWidget *parent)
        : QWidget(parent)
    {
        setupUI();
    }

    AppTitleWidget::~AppTitleWidget()
    {
    }

    void AppTitleWidget::setupUI()
    {
        pushButton = new QPushButton(this);
        titleNameLab = new QLabel(this);

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(titleNameLab);
        layout->setSpacing(0);
        layout->addWidget(pushButton);
        setLayout(layout);
    }

    void AppTitleWidget::setTitleName(const QString &name)
    {
        titleNameLab->setText(name);
    }

    void AppTitleWidget::setCloseButShow(bool flag)
    {
        pushButton->setVisible(flag);
    }

}