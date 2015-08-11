#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QList>
#include <math.h>

#include "upnp/device/device.h"
using namespace brisa::upnp;

class Button : public QPushButton
{
    Q_OBJECT

public:
    Button(QString text, int height, QLabel *label, QString *number1, QString *number2, QWidget *calc);
    ~Button();

    double getNumber(QString *number);

signals:

    void labelChanged();

private slots:

    void clickedButton();

private:

    QLabel *label;
    QString *number1;
    QString *number2;

};

#endif // BUTTON_H
