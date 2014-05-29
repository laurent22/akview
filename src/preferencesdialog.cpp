#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "application.h"

#include <QDebug>

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PreferencesDialog) {
	ui->setupUi(this);

	QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_currentChanged(int)));
	QObject::connect(ui->shortcutListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(shortcutListWidget_currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
	QObject::connect(ui->shortcutKeySequenceEdit, SIGNAL(editingFinished()), this, SLOT(shortcutKeySequenceEdit_editingFinished()));
}

PreferencesDialog::~PreferencesDialog() {
	delete ui;
}

void PreferencesDialog::refreshCurrentTab() {
	if (ui->tabWidget->currentWidget() == ui->shortcutsTab) {
		ui->shortcutListWidget->clear();

		mv::ActionVector actions = mv::Application::instance()->actions();
		for (unsigned i = 0; i < actions.size(); i++) {
			mv::ActionListWidgetItem* item = new mv::ActionListWidgetItem(actions[i]);
			ui->shortcutListWidget->addItem(item);
		}
	} else if (ui->tabWidget->currentWidget() == ui->generalTab) {

	}
}

mv::Action *PreferencesDialog::selectedShortcutAction() const {
	mv::ActionListWidgetItem* item = selectedShortcutItem();
	if (!item) return NULL;
	return item->action();
}

mv::ActionListWidgetItem *PreferencesDialog::selectedShortcutItem() const {
	mv::ActionListWidgetItem* item = dynamic_cast<mv::ActionListWidgetItem*>(ui->shortcutListWidget->currentItem());
	return item;
}

void PreferencesDialog::showEvent(QShowEvent *event) {
	if (event->spontaneous()) return;
	refreshCurrentTab();
}

void PreferencesDialog::tabWidget_currentChanged(int) {
	refreshCurrentTab();
}

void PreferencesDialog::shortcutListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
	mv::Action* action = selectedShortcutAction();
	ui->shortcutKeySequenceEdit->setKeySequence(action->shortcut());
}

void PreferencesDialog::shortcutKeySequenceEdit_editingFinished() {
	mv::Action* action = selectedShortcutAction();
	action->setShortcut(ui->shortcutKeySequenceEdit->keySequence());
//	ui->shortcutListWidget->update();

	mv::ActionListWidgetItem* item = selectedShortcutItem();
	item->updateText();
//	QString t = item->text();
//	item->setText(t + "-");
//	//item->setText(t);
}
