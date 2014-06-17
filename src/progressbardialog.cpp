#include "progressbardialog.h"
#include "ui_progressbardialog.h"

ProgressBarDialog::ProgressBarDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProgressBarDialog)
{
	ui->setupUi(this);
}

ProgressBarDialog::~ProgressBarDialog()
{
	delete ui;
}
