#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QShowEvent>

#include "pluginmanager.h"
#include "simpletypes.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {

	Q_OBJECT

public:

	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();

private:

	void rebuildCurrentTab();
	void refreshShortcutControls();
	mv::Action* selectedShortcutAction() const;
	mv::ActionListWidgetItem *selectedShortcutItem() const;
	QWidgetBoolMap openedTabs_;
	Ui::PreferencesDialog *ui;

protected:

	void showEvent(QShowEvent* event);

public slots:

	void tabWidget_currentChanged(int);
	void shortcutListWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void shortcutKeySequenceEdit_editingFinished();
	void buttonBox_accepted();
	void shortcutResetButton_clicked(bool);
	void shortcutClearButton_clicked(bool);

};

#endif // PREFERENCESDIALOG_H
