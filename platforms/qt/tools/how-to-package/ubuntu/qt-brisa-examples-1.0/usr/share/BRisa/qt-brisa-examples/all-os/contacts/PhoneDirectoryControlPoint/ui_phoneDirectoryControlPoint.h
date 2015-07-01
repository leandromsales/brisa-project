/********************************************************************************
** Form generated from reading UI file 'phoneDirectoryControlPoint.ui'
**
** Created: Sat Aug 28 12:51:49 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHONEDIRECTORYCONTROLPOINT_H
#define UI_PHONEDIRECTORYCONTROLPOINT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PhoneDirectoryControlPoint
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QListWidget *li_devices;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *edt_search;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QLineEdit *edt_Result;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PhoneDirectoryControlPoint)
    {
        if (PhoneDirectoryControlPoint->objectName().isEmpty())
            PhoneDirectoryControlPoint->setObjectName(QString::fromUtf8("PhoneDirectoryControlPoint"));
        PhoneDirectoryControlPoint->resize(448, 280);
        centralWidget = new QWidget(PhoneDirectoryControlPoint);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        li_devices = new QListWidget(centralWidget);
        li_devices->setObjectName(QString::fromUtf8("li_devices"));

        horizontalLayout->addWidget(li_devices);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        edt_search = new QLineEdit(centralWidget);
        edt_search->setObjectName(QString::fromUtf8("edt_search"));

        verticalLayout->addWidget(edt_search);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        edt_Result = new QLineEdit(centralWidget);
        edt_Result->setObjectName(QString::fromUtf8("edt_Result"));

        verticalLayout->addWidget(edt_Result);


        horizontalLayout->addLayout(verticalLayout);

        PhoneDirectoryControlPoint->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PhoneDirectoryControlPoint);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 448, 25));
        PhoneDirectoryControlPoint->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PhoneDirectoryControlPoint);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PhoneDirectoryControlPoint->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PhoneDirectoryControlPoint);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PhoneDirectoryControlPoint->setStatusBar(statusBar);

        retranslateUi(PhoneDirectoryControlPoint);

        QMetaObject::connectSlotsByName(PhoneDirectoryControlPoint);
    } // setupUi

    void retranslateUi(QMainWindow *PhoneDirectoryControlPoint)
    {
        PhoneDirectoryControlPoint->setWindowTitle(QApplication::translate("PhoneDirectoryControlPoint", "Phone Directory Control Point", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PhoneDirectoryControlPoint", "Search:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PhoneDirectoryControlPoint", "Result:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PhoneDirectoryControlPoint: public Ui_PhoneDirectoryControlPoint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHONEDIRECTORYCONTROLPOINT_H
