#include <QDebug>

#include "application.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QtGui>

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
	QString txt;
	switch (type) {
	case QtDebugMsg:
		txt = QString("Debug: %1").arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("Warning: %1").arg(msg);
	break;
	case QtCriticalMsg:
		txt = QString("Critical: %1").arg(msg);
	break;
	case QtFatalMsg:
		txt = QString("Fatal: %1").arg(msg);
	break;
	}
	QFile outFile("/Users/laurent/mv.log");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}

int main(int argc, char *argv[]) {
	mv::Application app(argc, argv);
	qInstallMessageHandler(myMessageHandler);
	app.initialize();
	return app.exec();
}
