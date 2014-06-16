#include "consolewidget.h"

#include <QVBoxLayout>

namespace mv {

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent) {
	textbox_ = new QPlainTextEdit(this);
	textbox_->setReadOnly(true);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(textbox_);
	setLayout(layout);
}

// QSize ConsoleWidget::sizeHint() const {
// 	return QSize(100, 100);
// }

void ConsoleWidget::log(const QString& s) {
	textbox_->appendPlainText(s);
} 

}