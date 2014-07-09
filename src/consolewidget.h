#ifndef MV_CONSOLEWIDGET_H
#define MV_CONSOLEWIDGET_H

namespace mv {

class ConsoleWidget: public QWidget {

	Q_OBJECT

public:

	ConsoleWidget(QWidget* parent = 0);
	void log(const QString& s);
	int vScrollValue() const;
	void setVScrollValue(int v);
	QSizeF documentSize() const;

protected:

	void showEvent(QShowEvent* event);

private:

	QPlainTextEdit* textbox_;
	bool fontIsSet_;

};

}

#endif