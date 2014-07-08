#include "jsapi_ui.h"

#include <QDebug>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSpinBox>

#include "../messageboxes.h"

namespace jsapi {

FormElement::FormElement(const QString& type, const QString& name, const QString& title, const QString& description) {
	type_ = type;
	name_ = name;
	title_ = title;
	description_ = description;
}

QString FormElement::type() const {
	return type_;
}

QString FormElement::title() const {
	return title_;
}

QString FormElement::description() const {
	return description_;
}

QString FormElement::name() const {
	return name_;
}

QString FormElement::validator() const {
	return validator_;
}

QVariantList FormElement::options() const {
	return options_;
}

QVariant FormElement::value() const {
	return value_;
}

void FormElement::setType(const QString& v) {
	if (type_ == v) return;
	type_ = v;
}

void FormElement::setTitle(const QString& v) {
	if (title_ == v) return;
	title_ = v;
}

void FormElement::setDescription(const QString& v) {
	if (description_ == v) return;
	description_ = v;
}

void FormElement::setName(const QString& v) {
	if (name_ == v) return;
	name_ = v;
}

void FormElement::setOptions(const QVariantList& v) {
	if (options_ == v) return;
	options_ = v;
}

void FormElement::setValue(const QVariant& v) {
	if (value_ == v) return;
	value_ = v;
}

void FormElement::setValidator(const QString& v) {
	if (value_ == v) return;
	validator_ = v;
}

QScriptValue FormElement::onChange() const {
	return eventMap_["onChange"];
}

void FormElement::setOnChange(const QScriptValue& v) {
	eventMap_["onChange"] = v;
}

void FormElement::triggerEvent(const QString& eventName) {
	for (std::map<QString, QScriptValue>::iterator it = eventMap_.begin(); it != eventMap_.end(); ++it) {
		if (eventName == it->first) {
			QScriptValue fn = (QScriptValue)it->second;
			fn.call();
		}
	}
}

FormDialog::FormDialog(const FormElements& elements) : QDialog(NULL) {
	formElements_ = elements;
	build(elements);
}

void FormDialog::build(const FormElements& elements) {
	QFormLayout* formLayout = new QFormLayout(this);

	for (unsigned int i = 0; i < elements.size(); i++) {
		FormElement* e = elements[i];

		if (e->type() == "text") {

			QLineEdit* w = new QLineEdit(this);
			formLayout->addRow(e->title(), w);
			if (e->validator() == "int") {
				 w->setValidator(new QIntValidator());
			}
			w->setText(e->value().toString());
			elementWidgetMap_[e] = w;

		} else if (e->type() == "select") {

			QComboBox* w = new QComboBox(this);
			QVariantList options = e->options();
			for (int j = 0; j < options.size(); j++) {
				QVariantMap v = options[j].toMap();
				// TODO: check for missing property "title"
				QVariant value = v.contains("value") ? v["value"] : v["title"];
				w->addItem(v["title"].toString(), value);
			}

			if (e->value().toString() != "") {
				for (int j = 0; j < w->count(); j++) {
					QVariant d = w->itemData(j);
					if (d.toString() == e->value().toString()) {
						w->setCurrentIndex(j);
						break;
					}
				}
			}
			
			formLayout->addRow(e->title(), w);
			elementWidgetMap_[e] = w;

		} else if (e->type() == "checkbox") {

			QCheckBox* w = new QCheckBox(this);
			w->setText(e->title());
			w->setChecked(e->value().toBool());
			formLayout->addRow("", w);
			elementWidgetMap_[e] = w;

		} else if (e->type() == "jpegQuality") {

			QSpinBox* w = new QSpinBox(this);
			w->setMinimum(0);
			w->setMaximum(100);
			w->setSuffix("%");
			w->setValue(90);
			formLayout->addRow(tr("JPEG Quality"), w);
			elementWidgetMap_[e] = w;

		} else {

			qWarning() << "Unsupported element type:" << e->type() << e->name();
			continue;

		}

		if (e->description() != "") {
			QLabel* l = new QLabel(e->description(), this);
			l->setWordWrap(true);
			formLayout->addRow("", l);
		}

	}
	
	QWidget* formWidget = new QWidget(this);
	formWidget->setLayout(formLayout);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(formWidget);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	layout->addWidget(buttonBox);

	setLayout(layout);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

FormElement* FormDialog::widgetElement(QWidget* w) const {
	for (FormElementQWidgetMap::iterator it = elementWidgetMap_.begin(); it != elementWidgetMap_.end(); ++it) {
		if (it->second == w) return it->first;
	}
	return NULL;
}

void FormDialog::updateFormElementsFromControls() {
	for (unsigned int i = 0; i < formElements_.size(); i++) {
		FormElement* e = formElements_[i];
		QWidget* widget = elementWidgetMap_[e];

		if (e->type() == "text") {

			QLineEdit* w = dynamic_cast<QLineEdit*>(widget);
			e->setValue(w->text());

		} else if (e->type() == "select") {

			QComboBox* w = dynamic_cast<QComboBox*>(widget);
			QVariant v = w->currentData();
			e->setValue(v.isNull() ? w->currentText() : v);

		} else if (e->type() == "checkbox") {

			QCheckBox* w = dynamic_cast<QCheckBox*>(widget);
			e->setValue(w->isChecked());

		} else if (e->type() == "jpegQuality") {

			QSpinBox* w = dynamic_cast<QSpinBox*>(widget);
			e->setValue(w->value());

		} else {

			qWarning() << "Unsupported control:" << e->type();

		}
	}
}

FormElements FormDialog::formElements() const {
	return formElements_;
}

void FormDialog::lineEdit_textEdited(const QString& text) {
	Q_UNUSED(text)
	// QLineEdit* w = dynamic_cast<QLineEdit*>(sender());
	// FormElement* e = widgetElement(w);
	// if (!e) return;
	// e->triggerEvent("onChange");
}

Ui::Ui(QScriptEngine* engine) {
	engine_ = engine;
	formElementRegistered_ = false;
}

QObject* Ui::newFormElement(const QString& type, const QString& name, const QString& title, const QString& description) {
	FormElement* e = new FormElement(type, name, title, description);
	return dynamic_cast<QObject*>(e);
}

QScriptValue Ui::variantToScriptValue(const QVariant& v) const {
	if (v.type() == QVariant::String) return QScriptValue(v.toString());
	if (v.type() == QVariant::Int) return QScriptValue(v.toInt());
	if (v.type() == QVariant::Bool) return QScriptValue(v.toBool());
	if (v.type() == QVariant::Double) return QScriptValue(v.toDouble());
	return engine_->newVariant(v);
}

QString Ui::messageBox(const QString& message, const QString& type) {
	if (type == "info") {
		mv::messageBoxes::info(message);
	} else if (type == "warn" || type == "warning") {
		mv::messageBoxes::warning(message);
	} else if (type == "err" || type == "error") {
		mv::messageBoxes::error(message);
	} else if (type == "confirmation") {
		int r = mv::messageBoxes::confirmation(message);
		if (r == QMessageBox::Yes) return "yes";
		return "no";
	}
	return "ok";
}

QScriptValue Ui::form(const QScriptValue& form, const QString& title) {
	FormElements formElements;
	QVariantList elements = form.toVariant().toList();
	for (int i = 0; i < elements.size(); i++) {
		FormElement* e = elements[i].value<FormElement*>();
		formElements.push_back(e);
	}

	FormDialog dialog(formElements);
	dialog.setWindowTitle(title);
	dialog.setModal(true);
	int result = dialog.exec();

	QScriptValue output = engine_->newObject();

	if (result != QDialog::Accepted) {
		output = QScriptValue(false);
		return output;
	}

	dialog.updateFormElementsFromControls();

	formElements = dialog.formElements();
	for (unsigned int i = 0; i < formElements.size(); i++) {
		FormElement* e = formElements[i];
		output.setProperty(e->name(), variantToScriptValue(e->value()));
	}

	return output;
}

}