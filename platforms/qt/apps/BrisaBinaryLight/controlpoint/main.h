#ifndef LIGHTSWITCHER_MAIN_H
#define LIGHTSWITCHER_MAIN_H

#include <QtGui/QApplication>

class MyQApplication: public QApplication
{
	Q_OBJECT
public:
	MyQApplication(int, char**);
	
	bool notify(QObject *, QEvent *);

private:
	int countQtEventLoop;

};

#endif // LIGHTSWITCHER_MAIN_H
