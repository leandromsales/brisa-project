/********************************************************************************
** Form generated from reading UI file 'light.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHT_H
#define UI_LIGHT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BinaryLightForm
{
public:
    QPushButton *pushButton1;

    void setupUi(QWidget *BinaryLightForm)
    {
        if (BinaryLightForm->objectName().isEmpty())
            BinaryLightForm->setObjectName(QStringLiteral("BinaryLightForm"));
        BinaryLightForm->resize(200, 250);
        BinaryLightForm->setMinimumSize(QSize(200, 250));
        BinaryLightForm->setMaximumSize(QSize(200, 250));
        pushButton1 = new QPushButton(BinaryLightForm);
        pushButton1->setObjectName(QStringLiteral("pushButton1"));
        pushButton1->setEnabled(true);
        pushButton1->setGeometry(QRect(0, 0, 200, 250));
        pushButton1->setMinimumSize(QSize(200, 250));
        pushButton1->setMaximumSize(QSize(200, 250));
        QIcon icon;
        icon.addFile(QStringLiteral("usr/share/pixmaps/onlight1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton1->setIcon(icon);
        pushButton1->setIconSize(QSize(200, 250));
        pushButton1->setCheckable(true);
        pushButton1->setFlat(true);

        retranslateUi(BinaryLightForm);

        QMetaObject::connectSlotsByName(BinaryLightForm);
    } // setupUi

    void retranslateUi(QWidget *BinaryLightForm)
    {
        BinaryLightForm->setWindowTitle(QApplication::translate("BinaryLightForm", "Binary Light", 0));
        BinaryLightForm->setStyleSheet(QApplication::translate("BinaryLightForm", "background-color: rgb(255, 255, 255);", 0));
        pushButton1->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BinaryLightForm: public Ui_BinaryLightForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHT_H
