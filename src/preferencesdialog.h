#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QShowEvent>

#include "pluginmanager.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {

	Q_OBJECT

public:

	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

private:

	void refreshCurrentTab();
	mv::Action* selectedShortcutAction() const;
	mv::ActionListWidgetItem *selectedShortcutItem() const;

	Ui::PreferencesDialog *ui;

protected:

	void showEvent(QShowEvent* event);

public slots:

	void tabWidget_currentChanged(int);
	void shortcutListWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void shortcutKeySequenceEdit_editingFinished();

};

#endif // PREFERENCESDIALOG_H
