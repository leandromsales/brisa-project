#include "phoneDirectoryDevice.h"
#include "ui_phoneDirectoryDevice.h"
#include "phoneDirectoryFunctions.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>

PhoneDirectoryDevice::PhoneDirectoryDevice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhoneDirectoryDevice),
    phoneDirectoryDevice(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
                DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
                DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_phone,
                DEVICE_MODEL_URL, DEVICE_SERIAL_phone, getCompleteUuid())
{
    ui->setupUi(this);
    list();

    //preparando o device para funcionar na rede UPNP
    PhoneDirectoryFunctions *phoneDirectoryFunctions = new PhoneDirectoryFunctions();
    phoneDirectoryFunctions->setDescriptionFile("PhoneDirectoryFunctions.xml");
    phoneDirectoryDevice.addService(phoneDirectoryFunctions);
    phoneDirectoryDevice.start();


    name = phoneDirectoryDevice.getServiceByType("urn:schemas-upnp-org:service:PhoneDirectoryFunctions:1")->getVariable("name");

    phone = phoneDirectoryDevice.getServiceByType("urn:schemas-upnp-org:service:PhoneDirectoryFunctions:1")->getVariable("phone");

    connect(name,SIGNAL(changed(BrisaStateVariable*)),this,SLOT(updateName(BrisaStateVariable*)));

}

PhoneDirectoryDevice::~PhoneDirectoryDevice()
{
    delete ui;
}

void PhoneDirectoryDevice::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//this method returns the phone of the person request into controlPoint Interface
void PhoneDirectoryDevice::updateName(BrisaStateVariable *name){
    QString key = name->getValue().toString();
    if (!key.isEmpty())
        phone->setAttribute(BrisaStateVariable::Value,search(key));

}

//this method copies records from file to list
void PhoneDirectoryDevice::list(){
    QFile f("PhoneDirectoryItems.txt");
    if (!f.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    else{
        while (!f.atEnd()) {
            QByteArray line = f.readLine();
            QString registro = line;
            QString key = registro.section(" | ", 0, 0);
            QString value = registro.section(" | ", 1, 1);
            if (!key.isEmpty() && !value.isEmpty())
                items.insert(key,value);
        }
    }
    ui->tb_list->setColumnWidth(0,450);
    ui->tb_list->setColumnCount(2);
    updateTableView();
}

//this method updates interface table when one new item is added
void PhoneDirectoryDevice::updateTableView(){
    int i = 0;
    QString key;
    ui->tb_list->setRowCount(items.count());
    foreach(key,items.keys()){
        QTableWidgetItem *newName = new QTableWidgetItem(tr("name"),0);
        newName->setText(key);
        ui->tb_list->setItem(i,0,newName);
        QTableWidgetItem *newPhone = new QTableWidgetItem(tr("phone"),0);
        newPhone->setText(items.value(key));
        ui->tb_list->setItem(i,1,newPhone);
        i++;
     }

}

//this method searches key and returns one string with value of key if find it
QString PhoneDirectoryDevice::search(QString key){
    if(items.contains(key)){
        return items.value(key);
    }
    else{
        return "";
    }

}

void PhoneDirectoryDevice::on_edt_search_returnPressed()
{
    QString result = "";
    if(!ui->edt_search->text().isEmpty()){
        result = search(ui->edt_search->text());
        if(result.isEmpty())
            QMessageBox::information(this,"contact not found","Did not possible find the contact",QMessageBox::Ok);
        else
            ui->edt_result->setText(result);
    }
}

//this slot request dates of new contact when add request
void PhoneDirectoryDevice::on_actionAdd_User_triggered()
{
    bool okusr = false;
    bool okphone = false;
    QString phone;
    QString user = QInputDialog::getText(this,"contact","Inform the contact name:",QLineEdit::Normal,"",&okusr);
    if(items.contains(user))
        QMessageBox::information(this, "Duplicate Contact","The contact informed already exists.",QMessageBox::Ok);
    else
        if(okusr)
            phone = QInputDialog::getText(this,"phone", "Informe the contact phone:", QLineEdit::Normal,"",&okphone);

    if(okusr && okphone){
        if(!user.isEmpty() && !phone.isEmpty()){
            items.insert(user,phone);
            updateFile(user,phone);
            ui->tb_list->clear();
            updateTableView();
        }
     }
}

//this method add new record in the file
void PhoneDirectoryDevice::updateFile(QString key, QString value){
    QFile f("PhoneDirectoryItems.txt");
    if (!f.open(QIODevice::Append | QIODevice::Text))
        return;
    else{
        f.write(qPrintable(key +" | "+ value + "\n"));
    }
}

//this method updates file
void PhoneDirectoryDevice::removeFromFile(){
    QFile f("PhoneDirectoryItems.txt");
    f.remove("PhoneDirectoryItems.txt");
    if (!f.open(QIODevice::Append | QIODevice::Text))
        return;
    else{
        QString key;
        foreach(key, items.keys())
            f.write(qPrintable(key + " | " + items.value(key)));
    }
}

void PhoneDirectoryDevice::on_actionRemove_User_triggered()
{
    if(ui->tb_list->selectedItems().count()==1){
        int row = ui->tb_list->currentItem()->row();
        QString key = ui->tb_list->item(row,0)->text();
        items.remove(key);
        removeFromFile();
        ui->tb_list->clear();
        updateTableView();
    }

}
