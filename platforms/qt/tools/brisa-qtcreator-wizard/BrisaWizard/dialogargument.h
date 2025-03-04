#ifndef DIALOGARGUMENT_H
#define DIALOGARGUMENT_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/brisaargument.h>
#include <BrisaUpnp/brisaaction.h>
#include <BrisaUpnp/brisastatevariable.h>
#include <BrisaUpnp/BrisaService>
#include <QDialog>
#include<QComboBox>


using namespace Brisa;

Q_DECLARE_METATYPE(BrisaArgument*)
Q_DECLARE_METATYPE(BrisaStateVariable*)
Q_DECLARE_METATYPE(QComboBox*);

namespace Ui {
    class DialogArgument;
}

class DialogArgument : public QDialog {
    Q_OBJECT
public:
    DialogArgument(QWidget *parent = 0);
    ~DialogArgument();
    void setArgumentList(QList<BrisaArgument*> actionList);
    QList<BrisaArgument*> getArgumentList();
    void setAction(QString action);
    void setStateVariables(QList<BrisaStateVariable*> stateVariableListsA);
    QList<BrisaStateVariable*> getStateVariables();
    void updateTableWidget();
    void addStateVariable(BrisaStateVariable *v);
    void clearStateVariables();

protected:
    void changeEvent(QEvent *e);

private:
    QString actionName;
    QList<BrisaArgument*> argumentList;
    QList<BrisaStateVariable*> stateVariableList;
    Ui::DialogArgument *ui;
    int validateInformation();
    

private slots:
    void on_btn_add_clicked();
    void on_btn_remove_clicked();
    void on_btnRemove_stateVariable_clicked();
    void on_btnRemove_argument_clicked();
    void on_btn_relationship_clicked();
    void on_btnInsert_argument_clicked();
    void on_btninsert_stateVariable_clicked();
    void on_pushButton_clicked();
    void on_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_btnRemove_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGARGUMENT_H
