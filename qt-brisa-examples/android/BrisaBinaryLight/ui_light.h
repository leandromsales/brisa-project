/********************************************************************************
** Form generated from reading UI file 'light.ui'
**
** Created: Sun Sep 11 21:41:37 2011
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHT_H
#define UI_LIGHT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BinaryLightForm
{
public:
    QPushButton *pushButton1;
    QLabel *label;

    void setupUi(QWidget *BinaryLightForm)
    {
        if (BinaryLightForm->objectName().isEmpty())
            BinaryLightForm->setObjectName(QString::fromUtf8("BinaryLightForm"));
        BinaryLightForm->resize(200, 250);
        BinaryLightForm->setMinimumSize(QSize(200, 250));
        BinaryLightForm->setMaximumSize(QSize(800, 600));
        pushButton1 = new QPushButton(BinaryLightForm);
        pushButton1->setObjectName(QString::fromUtf8("pushButton1"));
        pushButton1->setEnabled(true);
        pushButton1->setGeometry(QRect(20, 10, 150, 200));
        pushButton1->setMinimumSize(QSize(150, 200));
        pushButton1->setMaximumSize(QSize(150, 200));
        QIcon icon;
        icon.addFile(QString::fromUtf8("usr/share/pixmaps/onlight1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton1->setIcon(icon);
        pushButton1->setIconSize(QSize(50, 50));
        pushButton1->setCheckable(true);
        pushButton1->setFlat(true);
        label = new QLabel(BinaryLightForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 220, 181, 17));

        retranslateUi(BinaryLightForm);

        QMetaObject::connectSlotsByName(BinaryLightForm);
    } // setupUi

    void retranslateUi(QWidget *BinaryLightForm)
    {
        BinaryLightForm->setWindowTitle(QApplication::translate("BinaryLightForm", "Binary Light", 0, QApplication::UnicodeUTF8));
        BinaryLightForm->setStyleSheet(QApplication::translate("BinaryLightForm", "background-color: rgb(255, 255, 255);", 0, QApplication::UnicodeUTF8));
        pushButton1->setText(QString());
        label->setText(QApplication::translate("BinaryLightForm", "Brisa Binary Light", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BinaryLightForm: public Ui_BinaryLightForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHT_H
