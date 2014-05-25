#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHash>

namespace Ui {

class Dialog;

}

class Dialog : public QDialog {
	Q_OBJECT

public:

	explicit Dialog(QWidget *parent = 0);
	~Dialog();
	QHash<QString, QVariant> settings() const;
	void loadSettings(const QHash<QString, QVariant> &settings);

private:

	Ui::Dialog *ui;

};

#endif // DIALOG_H
