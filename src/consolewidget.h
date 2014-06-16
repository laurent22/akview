#ifndef MV_CONSOLEWIDGET_H
#define MV_CONSOLEWIDGET_H

#include <QPlainTextEdit>

namespace mv {

class ConsoleWidget: public QWidget {

	Q_OBJECT

public:

	ConsoleWidget(QWidget* parent = 0);
	void log(const QString& s);
	// QSize sizeHint() const;

private:

	QPlainTextEdit* textbox_;

};

}

#endif