#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
	ui->setupUi(this);

	//ui->losslessRotation90rb->setChecked(true);
}

Dialog::~Dialog() {
	delete ui;
}

QHash<QString, QVariant> Dialog::settings() const {
	QHash<QString, QVariant> output;
	int r = 0;
	if (ui->losslessRotation90rb->isChecked()) r = 90;
	if (ui->losslessRotation180rb->isChecked()) r = 180;
	if (ui->losslessRotation270rb->isChecked()) r = 270;
	output.insert("losslessRotation", r);
	return output;
}

void Dialog::loadSettings(const QHash<QString, QVariant>& settings) {
	if (settings.contains("losslessRotation")) {
		int r = settings.value("losslessRotation").toInt();
		if (r == 0) ui->losslessRotation0rb->setChecked(true);
		if (r == 90) ui->losslessRotation90rb->setChecked(true);
		if (r == 180) ui->losslessRotation180rb->setChecked(true);
		if (r == 270) ui->losslessRotation270rb->setChecked(true);
	} else {
		ui->losslessRotation0rb->setChecked(true);
	}
}
