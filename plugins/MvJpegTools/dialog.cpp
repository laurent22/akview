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
		QRadioButton* rb = NULL;
		if (r == 0) rb = ui->losslessRotation0rb;
		if (r == 90) rb = ui->losslessRotation90rb;
		if (r == 180) rb = ui->losslessRotation180rb;
		if (r == 270) rb = ui->losslessRotation270rb;
		rb->setChecked(true);
		rb->setFocus();
	} else {
		ui->losslessRotation0rb->setChecked(true);
	}
}
