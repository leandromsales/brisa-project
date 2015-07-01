/********************************************************************************
** Form generated from reading UI file 'controlpointgui.ui'
**
** Created: Thu Feb 24 12:07:35 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPOINTGUI_H
#define UI_CONTROLPOINTGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_controlpointFormCP
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidgetCP;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableWidget *tableWidget;
    QLabel *label_3;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *controlpointFormCP)
    {
        if (controlpointFormCP->objectName().isEmpty())
            controlpointFormCP->setObjectName(QString::fromUtf8("controlpointFormCP"));
        controlpointFormCP->resize(771, 630);
        QIcon icon;
        icon.addFile(QString::fromUtf8("Icons/brisa_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        controlpointFormCP->setWindowIcon(icon);
        centralwidget = new QWidget(controlpointFormCP);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeWidgetCP = new QTreeWidget(centralwidget);
        treeWidgetCP->setObjectName(QString::fromUtf8("treeWidgetCP"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidgetCP->sizePolicy().hasHeightForWidth());
        treeWidgetCP->setSizePolicy(sizePolicy);
        treeWidgetCP->setMaximumSize(QSize(300, 16777215));
        treeWidgetCP->setColumnCount(1);

        horizontalLayout->addWidget(treeWidgetCP);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setEnabled(true);
        textEdit->setMinimumSize(QSize(0, 0));
        textEdit->setLineWrapMode(QTextEdit::NoWrap);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);


        horizontalLayout->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        controlpointFormCP->setCentralWidget(centralwidget);
        menubar = new QMenuBar(controlpointFormCP);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 771, 27));
        controlpointFormCP->setMenuBar(menubar);
        statusbar = new QStatusBar(controlpointFormCP);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        controlpointFormCP->setStatusBar(statusbar);
        toolBar = new QToolBar(controlpointFormCP);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setFloatable(false);
        controlpointFormCP->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(controlpointFormCP);

        QMetaObject::connectSlotsByName(controlpointFormCP);
    } // setupUi

    void retranslateUi(QMainWindow *controlpointFormCP)
    {
        controlpointFormCP->setWindowTitle(QApplication::translate("controlpointFormCP", "Brisa UPnP Control Point", 0, QApplication::UnicodeUTF8));
        controlpointFormCP->setStyleSheet(QString());
        QTreeWidgetItem *___qtreewidgetitem = treeWidgetCP->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("controlpointFormCP", "Device/Service", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("controlpointFormCP", "Event Log:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("controlpointFormCP", "Action Result:", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("controlpointFormCP", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class controlpointFormCP: public Ui_controlpointFormCP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPOINTGUI_H
