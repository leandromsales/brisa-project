#include "brisaconfgui.h"
#include <QList>
#include <QFileDialog>

#ifdef Q_OS_UNIX
#define DEFAULT_CONFIG_PATH "/etc/brisa"
#else
#define DEFAULT_CONFIG_PATH "C:/brisa"
#endif

brisaConfGui::brisaConfGui(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    connect(pushButtonAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(pushButtonRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(pushButtonSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(enableRemoveButton()));
    connect(this, SIGNAL(testTree()), this, SLOT(enableRemoveButton()));
    connect(expandButton, SIGNAL(clicked()), this, SLOT(expandItems()));
    connect(collapseButton, SIGNAL(clicked()), this, SLOT(collapseItems()));
    connect(pushButtonChange, SIGNAL(clicked()), this, SLOT(updateConfigFilePath()));
    connect(checkBoxSaveDefault, SIGNAL(toggled(bool)), this, SLOT(enableChangeButton(bool)));

    createActions();
    createMenus();

    splash = new QSplashScreen(QPixmap("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/BrisaSplashScreenConfiguration.png"));
    contSplashScreen = 0;
    linecont = 0;
    manager = BrisaConfigurationManager::getInstance();
    savePath = manager->getConfigFilePath();
    manager->setDirectAccess(true);
    manager->update();
    lineEditConfigPath->setText(savePath);
    lineEditConfigPath->setEnabled(false);

    initTable();
    emit testTree();
}

brisaConfGui::~brisaConfGui()
{
    delete manager;
}

void brisaConfGui::initTable()
{
    foreach (QString section, manager->getSectionNames())
        listSection(section);
}

void brisaConfGui::clearTable()
{
    treeWidget->clear();
}

void brisaConfGui::listSection(QString section)
{
    QTreeWidgetItem *newSection = new QTreeWidgetItem(treeWidget);
    newSection->setText(0, section);


    QHash<QString,QString> sectionDictionary = manager->items(section);
    QList<QString> sections = sectionDictionary.keys();
    qSort(sections.begin(), sections.end());
    foreach (QString section, sections) {
        QTreeWidgetItem *newItem = new QTreeWidgetItem(newSection);
        newItem->setText(1, section);
        newItem->setText(2, sectionDictionary.value(section));
        connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                        this, SLOT(setLinesEdit(QTreeWidgetItem*)));
    }

}

void brisaConfGui::add()
{
    QString newSection = lineEditSection->text();
    QString newParameter = lineEditParameter->text();
    QString newValue = lineEditValue->text();

    if ((newSection != "") and (newParameter != "") and (newValue != "")) {
        QList<QTreeWidgetItem*> itemList = treeWidget->findItems(newSection, Qt::MatchExactly, 0);

        //add new section
        if (itemList.size() == 0) {
            QTreeWidgetItem *newSectionItem = new QTreeWidgetItem(treeWidget);
            newSectionItem->setText(0, newSection);
            itemList.append(newSectionItem);
        }

        //add parameter as child of section
        QTreeWidgetItem *parent = itemList[0];
        bool edit =  false;
        int i;
        for (i = 0; i < parent->childCount(); i++) {
            if (parent->child(i)->text(1) == newParameter) {
                edit = true;
                break;
            }
        }

        if ( edit )
            parent->child(i)->setText(2, newValue);
        else {
            QTreeWidgetItem *newParameterItem = new QTreeWidgetItem(parent);
            newParameterItem->setText(1, newParameter);
            newParameterItem->setText(2, newValue);

            connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                this, SLOT(setLinesEdit(QTreeWidgetItem*)));
        }
        //

        manager->setParameter(newSection, newParameter, newValue);
        treeWidget->sortItems(0, Qt::AscendingOrder);
        parent->sortChildren(1, Qt::AscendingOrder);
    } else{
        QMessageBox::warning(this, tr("Error at Saving"),
                tr("The attributes are blank.\n""You can't save!"),QMessageBox::Cancel);
    }
}

void brisaConfGui::setLinesEdit(QTreeWidgetItem* newParameterItem)
{
    if (newParameterItem->parent() != NULL)
        lineEditSection->setText(newParameterItem->parent()->text(0));

    lineEditParameter->setText(newParameterItem->text(1));
    lineEditValue->setText(newParameterItem->text(2));

}

void brisaConfGui::remove()
{
    QList<QTreeWidgetItem *> selectedItem  = treeWidget->selectedItems();
    QTreeWidgetItem *parent = selectedItem[0]->parent();
    int index;

    if (!parent) {
        index = treeWidget->indexOfTopLevelItem(treeWidget->currentItem());
        treeWidget->takeTopLevelItem(index);
        manager->removeSection(selectedItem[0]->text(0));
    }

    treeWidget->sortItems(0, Qt::AscendingOrder);
    emit testTree();
}

void brisaConfGui::save()
{
    if (checkBoxSaveDefault->isChecked()) {
        QString defaulPath = DEFAULT_CONFIG_PATH;
        manager->setConfigFilePath(defaulPath);
    }
    int numberSections = treeWidget->topLevelItemCount();

    for (int i = 0; i < numberSections; i++) {
        QTreeWidgetItem *section = treeWidget->topLevelItem(i);
        int numberParameters = section->childCount();

        for (int j = 0; j < numberParameters; j++) {
            manager->setParameter(section->text(0), section->child(j)->text(1),
            section->child(j)->text(2));
        }
   }

    QMessageBox msgBox;
    msgBox.setText("The document has been saved.");
    msgBox.exec();
}

void brisaConfGui::processSplashScreen()
{
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa Configuration..."),topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Creating Brisa Configuration..."),topRight, Qt::blue);
        break;
    }

    if (contSplashScreen > 2) {
        this->show();
        splash->finish(this);
        emit timeStop();
        delete splash;
    }
    contSplashScreen++;
}

void brisaConfGui::createActions()
{

    addAction = new QAction(tr("&Add"), this);
    //addAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/plus.png"));
    addAction->setToolTip("Add Attributes");
    connect(addAction, SIGNAL(triggered()), this, SLOT(add()));

    removeAction = new QAction(tr("Remove"), this);
    //removeAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/minus.png"));
    removeAction->setToolTip("Remove Attributes");
    connect(removeAction, SIGNAL(triggered()), this, SLOT(remove()));

    saveAction = new QAction(tr("Save Attributes"), this);
    saveAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/save.png"));
    saveAction->setToolTip("Save Attributes");
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    aboutConfAction = new QAction(tr("About Brisa Configuration"), this);
    aboutConfAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/info.png"));
    connect(aboutConfAction, SIGNAL(triggered()), this, SLOT(aboutConfiguration()));

    aboutBrisaAction = new QAction(tr("About Brisa"), this);
    aboutBrisaAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/brisa.png"));
    connect(aboutBrisaAction, SIGNAL(triggered()), this, SLOT(aboutBrisa()));

    aboutUpnpAction = new QAction(tr("About UPnP"), this);
    aboutUpnpAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/upnp.png"));
    connect(aboutUpnpAction, SIGNAL(triggered()), this, SLOT(aboutUpnp()));

    aboutQtAction = new QAction(tr("About QT"), this);
    aboutQtAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/qt.png"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setIcon(QIcon("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/exit.png"));
    exitAction->setToolTip("Close the Application");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));


}

void brisaConfGui::createMenus()
{
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);


    editMenu = menubar->addMenu(tr("&Edit"));
    editMenu->addAction(addAction);
    editMenu->addAction(removeAction);
    editMenu->addSeparator();

    helpMenu = menubar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutConfAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutBrisaAction);
    helpMenu->addAction(aboutUpnpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAction);
}

void brisaConfGui::aboutBrisa()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Brisa");
    msgBox.setText("<center><h1>About Brisa</h1></center> "
                   "<p>"
                   "<center><b>BRisa</b> is a project focused on developing UPnP "
                   "technologies.</center>"
                   "<p>"
                   "<p>"
                   "<center>http://brisa.garage.maemo.org");
    msgBox.setIconPixmap(QPixmap("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/brisa_logo.png"));
    msgBox.exec();
}

void brisaConfGui::aboutUpnp()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About UPnP");
    msgBox.setText("<center><h1>About UPnP</h1></center>"
                   "<p>"
                   "<b>Universal Plug and Play (UPnP)</b> is a set of networking"
                   "protocols promulgated by the UPnP Forum. The goals of UPnP are to allow devices"
                   "to connect seamlessly and to simplify the implementation of networks in the "
                   "home (data sharing,communications, and entertainment) and in corporate "
                   "environments for simplified installation of computer components. UPnP achieves "
                   "this by defining and publishing UPnP device control protocols (DCP) built upon "
                   "open, Internet-based communication standards."
                   "<p>"
                   "<p>"
                   "<center>www.upnp.org");
    msgBox.setIconPixmap(QPixmap("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/upnp.png"));
    msgBox.exec();
}


void brisaConfGui::aboutConfiguration()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Control Point");
    msgBox.setText("<h2>About Brisa Configuration</h2>"
                   "<p>"
                   "<b>Basic Brisa Configuration Gui Example</b>");
    msgBox.setIconPixmap(QPixmap("../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/info.png"));
    msgBox.exec();
}


void brisaConfGui::enableRemoveButton()
{
    if (treeWidget->topLevelItemCount() != 0) {
       pushButtonRemove->setEnabled(true);
       removeAction->setEnabled(true);
    }
    else {
        pushButtonRemove->setEnabled(false);
        removeAction->setEnabled(false);
    }

}

void brisaConfGui::collapseItems()
{
	for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
	    treeWidget->collapseItem(treeWidget->topLevelItem(i));
}

void brisaConfGui::expandItems()
{

    for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
        treeWidget->expandItem(treeWidget->topLevelItem(i));
}

void brisaConfGui::keyPressEvent(QKeyEvent* event)
{
	if (event->matches(QKeySequence::InsertParagraphSeparator))
	    this->add();
}

void brisaConfGui::updateConfigFilePath()
{
    QString path = QFileDialog::getExistingDirectory(this, "",
                                                     lineEditConfigPath->text());
    if (path != lineEditConfigPath->text() && manager->setConfigFilePath(path)) {
        lineEditConfigPath->setText(path);
        savePath = path;
        clearTable();
        initTable();
    }
}

void brisaConfGui::enableChangeButton(bool marked)
{
    QString path = marked ? DEFAULT_CONFIG_PATH : savePath;
    pushButtonChange->setEnabled(marked ? false : true);
    manager->setConfigFilePath(path);
    lineEditConfigPath->setText(path);
    clearTable();
    initTable();
}
