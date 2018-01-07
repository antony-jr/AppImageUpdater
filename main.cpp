#include <QApplication>
#include "AppImageUpdater.hpp"


int main(int argc , char **argv){
	QApplication app(argc , argv);
	AppImageUpdater MainApp;
	MainApp.show();
	return app.exec();
}
