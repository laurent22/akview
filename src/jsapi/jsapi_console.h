#ifndef MV_JSAPI_CONSOLE_H
#define MV_JSAPI_CONSOLE_H

namespace jsapi {

class Console : public QObject {

	Q_OBJECT

public:

	Console();
	void saveVScrollValue(int v);
	void restoreVScrollValue();

public slots:

	void info(const QString& s);
	void warn(const QString& s);
	void error(const QString& s);
	void dir(const QScriptValue& v);
	void showLastOutput();
	void show();
	void hide();

private:

	int savedScrollValue_;
	QString dir_(const QScriptValue& v, int indent);

};

}

#endif