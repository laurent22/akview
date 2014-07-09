#ifndef PROGRESSBARDIALOG_H
#define PROGRESSBARDIALOG_H

namespace Ui {
class ProgressBarDialog;
}

class ProgressBarDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ProgressBarDialog(QWidget *parent = 0);
	~ProgressBarDialog();

private:
	Ui::ProgressBarDialog *ui;
};

#endif // PROGRESSBARDIALOG_H
