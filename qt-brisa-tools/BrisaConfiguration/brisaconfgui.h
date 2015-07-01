#ifndef BRISACONFGUI_H
#define BRISACONFGUI_H

#include <QtGui/QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QSplashScreen>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QToolBar>
#include <QMessageBox>
#include <QKeyEvent>
#include <BrisaCore/BrisaConfigurationManager>
#include "ui_brisaconfgui.h"

using namespace Brisa;

class brisaConfGui : public QMainWindow , public Ui::brisaConfGuiClass
{
    Q_OBJECT

public:
    brisaConfGui(QWidget *parent = 0);
    ~brisaConfGui();
    QSplashScreen *splash;
    QString savePath;

protected:
	void keyPressEvent(QKeyEvent* event);

signals:
    void timeStop();
    void testTree();

private slots:
    void initTable();
    void clearTable();
    void listSection(QString section);
    void add();
    void remove();
    void save();
    void processSplashScreen();
    void aboutBrisa();
    void aboutUpnp();
    void aboutConfiguration();
    void enableRemoveButton();
    void collapseItems();
    void expandItems();
    void setLinesEdit(QTreeWidgetItem* newParameterItem);
    void updateConfigFilePath();
    void enableChangeButton(bool marked);


private:
    int linecont;
    QList<QTreeWidgetItem> listItems;
    BrisaConfigurationManager *manager;
    int contSplashScreen;

    //QToolBar *toolBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *addAction;;
    QAction *saveAction;
    QAction *removeAction;
    QAction *exitAction;

    QAction *aboutConfAction;
    QAction *aboutQtAction;
    QAction *aboutBrisaAction;
    QAction *aboutUpnpAction;

    void createActions();
    void createMenus();

};

#endif // BRISACONFGUI_H
