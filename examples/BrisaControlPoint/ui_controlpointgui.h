/********************************************************************************
** Form generated from reading UI file 'controlpointgui.ui'
**
** Created: Sat 29. Jan 21:46:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPOINTGUI_H
#define UI_CONTROLPOINTGUI_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QTableWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#ifdef Q_OS_ANDROID

#define OS 1

#else

#define OS 0

#endif

QT_BEGIN_NAMESPACE

class Ui_controlpointFormCP
{
public:
    //Widgets
    QWidget *centralWidget;
    //Labels
    QLabel *logBoxTitle;
    QLabel *actionsBoxTitle;
    //Layouts
    QGridLayout *centralWidgetLayout;
    QHBoxLayout *centralLayout;
    QTreeWidget *devicesTreeWidget;
    QVBoxLayout *textBoxLayout;
    //TextBox's
    QTextEdit *actionsTextEdit;
    QTableWidget *logBoxTable;
    //Bars
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty()) {
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        }

        //Redimensionando para um formato legível
        mainWindow->resize(771, 630);

        //Setando o ícone da janela
        QIcon windowIcon;
        windowIcon.addFile(QString::fromUtf8(":/rsc/brisa_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainWindow->setWindowIcon(windowIcon);

        //Instanciando o widget central do aplicativo
        centralWidget = new QWidget(mainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

        //Instanciando o layout (GridLayout) do widget central
        centralWidgetLayout = new QGridLayout(centralWidget);
        centralWidgetLayout->setObjectName(QString::fromUtf8("centralWidgetLayout"));

        //Instanciando o layout que abrigará quase todos os widgets
        centralLayout = new QHBoxLayout();
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));

        //Instanciando a árvore de devices
        devicesTreeWidget = new QTreeWidget();
        devicesTreeWidget->setObjectName(QString::fromUtf8("devicesTreeWidget"));
        devicesTreeWidget->setColumnCount(1);

        //Junk
        //        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //        sizePolicy.setHorizontalStretch(0);
        //        sizePolicy.setVerticalStretch(0);
        //        sizePolicy.setHeightForWidth(devicesTreeWidget->sizePolicy().hasHeightForWidth());
        //        devicesTreeWidget->setSizePolicy(sizePolicy);

        //Instanciando o layout que conterá apenas as caixas de textos
        textBoxLayout = new QVBoxLayout();
        textBoxLayout->setObjectName(QString::fromUtf8("textBoxLayout"));

        //Instanciando o label de título da caixa de logs
        logBoxTitle = new QLabel(centralWidget);
        logBoxTitle->setObjectName(QString::fromUtf8("label"));

        //Instanciando o log de ações em formato de tabela
        logBoxTable = new QTableWidget(centralWidget);
        logBoxTable->setObjectName(QString::fromUtf8("tableWidget"));

        //Instanciando o label de título da caixa de ações
        actionsBoxTitle = new QLabel(centralWidget);
        actionsBoxTitle->setObjectName(QString::fromUtf8("label_3"));

        //Instanciando o log de ações
        actionsTextEdit = new QTextEdit(centralWidget);
        actionsTextEdit->setObjectName(QString::fromUtf8("textEdit"));
//        actionsTextEdit->setMouseTracking(false);
        actionsTextEdit->setReadOnly(true);

        //Junk
        //        actionsTextEdit->setEnabled(true);
        //        actionsTextEdit->setMinimumSize(QSize(0, 0));
        //        actionsTextEdit->setLineWrapMode(QTextEdit::NoWrap);

        //Adicionando os widgets ao layout central
        centralLayout->addLayout(setWidgets());

        //Adicionando o layout central ao widget central
        centralWidgetLayout->addLayout(centralLayout, 0, 0, 1, 1);

        //Adicionando o widget central a janela principal
        mainWindow->setCentralWidget(centralWidget);

        //Adicionando as barras a janela principal
        mainWindow->setMenuBar(setMenuBar());
        mainWindow->setStatusBar(setStatusBar());
        mainWindow->addToolBar(Qt::TopToolBarArea, setToolBar());

        retranslateUi(mainWindow);

        //Conectando os slots pelos objectNames de cada objeto
        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    QVBoxLayout *setWidgets()
    {
        //OS igual a 1 -> ANDROID
        //OS igual a 0 -> DESKTOP
        if(OS)
        {
            textBoxLayout->addWidget(devicesTreeWidget);

            QVBoxLayout *logBox = new QVBoxLayout();
            QVBoxLayout *actionBox = new QVBoxLayout();

            logBox->addWidget(logBoxTitle);
            logBox->addWidget(logBoxTable);
            actionBox->addWidget(actionsBoxTitle);
            actionBox->addWidget(actionsTextEdit);

            textBoxLayout->addLayout(logBox);
            textBoxLayout->addLayout(actionBox);

            //JUNK
            //            QHBoxLayout *majorBox = new QHBoxLayout();
            //            textBoxLayout->addLayout(majorBox);
        }
        else
        {
            devicesTreeWidget->setMaximumSize(QSize(300, 16777215));
            centralLayout->addWidget(devicesTreeWidget);
            textBoxLayout->addWidget(logBoxTitle);
            textBoxLayout->addWidget(logBoxTable);
            textBoxLayout->addWidget(actionsBoxTitle);
            textBoxLayout->addWidget(actionsTextEdit);
        }
        return textBoxLayout;
    }

    QMenuBar *setMenuBar()
    {
        //Definindo a barra de menus
        menubar = new QMenuBar();
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 771, 27));
        return menubar;
    }

    QStatusBar *setStatusBar()
    {
        //Definindo a barra de status
        statusbar = new QStatusBar();
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        return statusbar;
    }

    QToolBar *setToolBar()
    {
        //Definindo a barra de ferramentas
        toolBar = new QToolBar();
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setFloatable(false);
        return toolBar;
    }

    void retranslateUi(QMainWindow *mainWindow)
    {
        //Defininido o título da janela principal
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "Brisa UPnP Control Point", 0));

        //Junk
        //        mainWindow->setStyleSheet(QString());

        //Definindo o primeiro item da árvore de devices
        QTreeWidgetItem *firstItemTree = devicesTreeWidget->headerItem();
        firstItemTree->setText(0, QApplication::translate("mainWindow", "Device/Service", 0));

        //Definindo os títulos das caixas de log e da toolBar
        logBoxTitle->setText(QApplication::translate("mainWindow", "Event Log:", 0));
        actionsBoxTitle->setText(QApplication::translate("mainWindow", "Action Result:", 0));
        toolBar->setWindowTitle(QApplication::translate("mainWindow", "toolBar", 0));

    } // retranslateUi

};

namespace Ui {
class controlpointFormCP: public Ui_controlpointFormCP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPOINTGUI_H
