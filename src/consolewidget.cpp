#include "consolewidget.h"

#include <QFontDatabase>
#include <QScrollBar>
#include <QVBoxLayout>

namespace mv {

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent) {
	fontIsSet_ = false;

	textbox_ = new QPlainTextEdit(this);
	textbox_->setReadOnly(true);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(textbox_);
	setLayout(layout);
}

QSizeF ConsoleWidget::documentSize() const {
	return textbox_->document()->size();
}

int ConsoleWidget::vScrollValue() const {
	return textbox_->verticalScrollBar()->value();
}

void ConsoleWidget::setVScrollValue(int v) {
	textbox_->verticalScrollBar()->setValue(v);
}

void ConsoleWidget::showEvent(QShowEvent*) {
	if (!fontIsSet_) {
		// Only set the font if and when the console is shown
		// since looking up font in the database is probably slow.
		QFontDatabase fontDatabase;
		QFont font = fontDatabase.systemFont(QFontDatabase::FixedFont);
		setFont(font);

		setStyleSheet("background-color: black; color: white;");

		fontIsSet_ = true;
	}
}

void ConsoleWidget::log(const QString& s) {
	textbox_->appendPlainText(s);
}

}