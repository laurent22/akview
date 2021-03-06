#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>

namespace Ui {
	class BatchDialog;
}

class BatchDialog : public QDialog {
	Q_OBJECT

public:

	explicit BatchDialog(QWidget *parent = 0);
	~BatchDialog();

protected:

	void showEvent(QShowEvent* event);

private:

	Ui::BatchDialog *ui;
	void updateButtons();
	QStringList addedFilePaths() const;
	bool firstShow_;

public slots:

	void addFilesButton_clicked();
	void removeButton_clicked();
	void removeAllButton_clicked();
	void fileListWidget_currentRowChanged(int);
	void buttonBox_accepted();

};

#endif // BATCHDIALOG_H
