#include "button.h"
#include "upnpcalculator.h"


Button::Button(QString text, int height, QLabel *label, QString *number1, QString *number2, QWidget *calc)
{
    UpnpCalculator *uc = (UpnpCalculator *) calc;
    setMaximumHeight(height/6);
    setStyleSheet("background-color:lightgray;");
    setText(text);
    setFont(QFont("Arial", this->height()/6));
    connect(this, SIGNAL(clicked()), this, SLOT(clickedButton()));
    connect(this, SIGNAL(labelChanged()), uc, SLOT(labelChanged()));
    this->label = label;
    this->number1 = number1;
    this->number2 = number2;
}

Button::~Button()
{

}

double Button::getNumber(QString *number)
{
    return number->toDouble();
}

void Button::clickedButton()
{
    QPushButton *btn = (QPushButton *) sender();

    if(label->text() == "0")
    {
        label->setText("");
    }

    if((btn->text() == "+" || btn->text() == "-" || btn->text() == "*" || btn->text() == "/" ) && (number1->size() != 0))
    {
        label->setText(label->text() + btn->text());
        *number2 = *number1;
        *number1 = "";
    }
    else if(btn->text() == "=")
    {
        double value;

        if(label->text().contains("+"))
        {
            value = getNumber(number2) + getNumber(number1);
            label->setText(QString::number(value));
            number1->clear();
            *number1 = QString::number(value);
        }
        else if(label->text().contains("-"))
        {
            value = getNumber(number2) - getNumber(number1);
            label->setText(QString::number(value));
            number1->clear();
            *number1 = QString::number(value);
        }
        else if(label->text().contains("*"))
        {
            value = getNumber(number2) * getNumber(number1);
            label->setText(QString::number(value));
            number1->clear();
            *number1 = QString::number(value);
        }
        else if(label->text().contains("/"))
        {
            value = getNumber(number2) / getNumber(number1);
            label->setText(QString::number(value));
            number1->clear();
            *number1 = QString::number(value);
        }

        number2->clear();
        emit labelChanged();
    }
    else if(btn->text() == "c")
    {
        label->setText("");
        number1->clear();
        number2->clear();
    }
    else if(btn->text() == " ")
    {

    }
    else
    {
        number1->append(btn->text());
        label->setText(label->text() + btn->text());
    }
}
