#include <QDebug>

#include "application.h"

#include <QFile>
#include <QTextStream>
#include <QtGlobal>
#include <QtGui>


#ifdef QT_DEBUG
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
	QFile outFile(QDir::homePath() + "/mv.log");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}
#endif // QT_DEBUG

int main(int argc, char *argv[]) {
	mv::Application app(argc, argv);
#ifdef QT_DEBUG
	qInstallMessageHandler(myMessageHandler);
#endif // QT_DEBUG
	app.initialize();
	return app.exec();
}
