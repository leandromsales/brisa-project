#ifndef UI_CALCULATOR
#define UI_CALCULATOR

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QDesktopWidget>
#include <QRect>

#include "button.h"

QT_BEGIN_NAMESPACE

class Ui_UpnpCalculatorForm
{

public:

    QLabel *result;
    QWidget *UpnpCalculatorForm;
    Button *btn;
    QList<QString> keys;
    QList<Button *> functionBtns;
    QString *number1;
    QString *number2;

    void setupUi(QWidget *calculatorWidget)
    {
        UpnpCalculatorForm = calculatorWidget;
        initialize();
        setResult();
        setKeyboard();
        retranslateUi();

        QMetaObject::connectSlotsByName(UpnpCalculatorForm);
    } // setupUi

    void retranslateUi()
    {
        UpnpCalculatorForm->setWindowTitle(QApplication::translate("UpnpCalculatorForm", "UPnP Calculator", 0));
        UpnpCalculatorForm->setStyleSheet(QApplication::translate("UpnpCalculatorForm", "background-color: rgb(255, 255, 255);", 0));
    } // retranslateUi

    void initialize() {

        QRect size = QApplication::desktop()->screen()->rect();

        if (UpnpCalculatorForm->objectName().isEmpty())
        {
            UpnpCalculatorForm->setObjectName(QStringLiteral("UpnpCalculatorForm"));
        }
        UpnpCalculatorForm->resize(size.width(), size.height());
        UpnpCalculatorForm->setMinimumSize(QSize(size.width(), size.height()));
        UpnpCalculatorForm->setMaximumSize(QSize(size.width(), size.height()));
        UpnpCalculatorForm->setLayout(new QVBoxLayout());
        setList();
        number1 = new QString();
        number2 = new QString();

    }

    void setList() {

        keys.append("1");
        keys.append("2");
        keys.append("3");
        keys.append("+");
        keys.append("4");
        keys.append("5");
        keys.append("6");
        keys.append("-");
        keys.append("7");
        keys.append("8");
        keys.append("9");
        keys.append("*");
        keys.append("c");
        keys.append("0");
        keys.append(".");
        keys.append("/");
        keys.append("=");

    }

    void setResult() {

        result = new QLabel();
        UpnpCalculatorForm->layout()->addWidget(result);
        result->resize(UpnpCalculatorForm->width(),UpnpCalculatorForm->height()/6);
        result->setObjectName("result");
        result->setText("");
        result->setStyleSheet("background-color:lightgray;border: 2px solid black");
        result->setFont(QFont("Arial", result->height()/4));
        result->setAlignment(Qt::AlignCenter);

    }

    void setKeyboard() {

        for(int j = 0; j < 4; j++)
        {
            QWidget *widget = new QWidget();
            widget->setLayout(new QHBoxLayout());

            for(int i = 0; i < 4; i++)
            {
                widget->layout()->addWidget(createButton());
            }

            UpnpCalculatorForm->layout()->addWidget(widget);
        }

            UpnpCalculatorForm->layout()->addWidget(createButton());

    }

    Button * createButton()
    {
        btn = new Button(keys.at(0), UpnpCalculatorForm->height(), result, number1, number2, UpnpCalculatorForm);

        if(keys.at(0) == "+" || keys.at(0) == "-" || keys.at(0) == "*" || keys.at(0) == "/")
        {
            functionBtns.append(btn);
        }

        keys.removeAt(0);
        return btn;
    }
};

namespace Ui {
class UpnpCalculatorForm: public Ui_UpnpCalculatorForm {};
} // namespace Ui

QT_END_NAMESPACE


#endif // UI_CALCULATOR

