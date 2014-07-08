#ifndef MV_JSAPI_UI_H
#define MV_JSAPI_UI_H

#include <QDialog>
#include <QScriptEngine>
#include <QVariant>

namespace jsapi {

class FormElement : public QObject {

	Q_OBJECT
	Q_PROPERTY(QString type READ type WRITE setType)
	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(QString description READ description WRITE setDescription)
	Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(QString validator READ validator WRITE setValidator)
	Q_PROPERTY(QVariantList options READ options WRITE setOptions)
	Q_PROPERTY(QVariant value READ value WRITE setValue)
	Q_PROPERTY(QScriptValue onChange READ onChange WRITE setOnChange)

public:

	static const int Text = 1;
	static const int Select = 2;
	static const int Checkbox = 3;

	FormElement(const QString& type, const QString& name, const QString& title = "", const QString& description = "");
	
	QString type() const;
	QString title() const;
	QString name() const;
	QVariantList options() const;
	QVariant value() const;
	QString description() const;
	QString validator() const;

	void setType(const QString& v);
	void setTitle(const QString& v);
	void setName(const QString& v);
	void setOptions(const QVariantList& v);
	void setValue(const QVariant& v);
	void setDescription(const QString& v);
	void setValidator(const QString& v);

	QScriptValue onChange() const;
	void setOnChange(const QScriptValue& v);
	void triggerEvent(const QString& eventName);

private:

	QString type_;
	QString title_;
	QString description_;
	QString name_;
	QVariantList options_;
	QVariant value_;
	QString validator_;
	mutable std::map<QString, QScriptValue> eventMap_;

};

typedef std::vector<FormElement*> FormElements;
typedef std::map<FormElement*, QWidget*> FormElementQWidgetMap;

class FormDialog : public QDialog {

	Q_OBJECT

public:

	FormDialog(const FormElements& elements);
	void updateFormElementsFromControls();
	FormElements formElements() const;

private:

	void build(const FormElements& elements);
	mutable FormElementQWidgetMap elementWidgetMap_;
	FormElement* widgetElement(QWidget* w) const;
	FormElements formElements_;

public slots:

	void lineEdit_textEdited(const QString& text);

};


class Ui : public QObject {

	Q_OBJECT

public:

	Ui(QScriptEngine* engine);

public slots:

	QObject* newFormElement(const QString& type, const QString& name, const QString& title = "", const QString& description = "");
	QScriptValue form(const QScriptValue& form, const QString& title = "");
	QString messageBox(const QString& message, const QString& type = "info");

private:

	QScriptEngine* engine_;
	bool formElementRegistered_;

};

}

#endif