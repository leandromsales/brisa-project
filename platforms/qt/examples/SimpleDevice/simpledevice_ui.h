/*
 * Esse arquivo é responsável por gerar a Ui do projeto.
 */

#ifndef SIMPLEDEVICE_UI
#define SIMPLEDEVICE_UI

#include <QLabel>
#include <QWidget>
#include <QLayout>

class SimpleDevice_Ui
{

public:

    /*
     * Definindo as propriedades gráficas da janela principal
     */
    static void setupUi(QWidget *window)
    {
        QHBoxLayout *windowLayout = new QHBoxLayout();
        windowLayout->setMargin(50);
        window->setLayout(windowLayout);
    }

    /*
     * Definindo as propriedades gráficas do label de status
     */
    static QLabel *setStatusLabel()
    {
        QLabel *status = new QLabel("Status");
        status->setFont(QFont("Arial", 100));
        status->setAlignment(Qt::AlignCenter);
        return status;
    }
};


#endif // SIMPLEDEVICE_UI

