#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "application.h"
#include "settings.h"

#include <QDebug>
#include <QKeySequence>

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PreferencesDialog) {
	ui->setupUi(this);

	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_currentChanged(int)));
	connect(ui->shortcutListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(shortcutListWidget_currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
	connect(ui->shortcutKeySequenceEdit, SIGNAL(editingFinished()), this, SLOT(shortcutKeySequenceEdit_editingFinished()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBox_accepted()));
	connect(ui->shortcutResetButton, SIGNAL(clicked(bool)), this, SLOT(shortcutResetButton_clicked(bool)));
	connect(ui->shortcutClearButton, SIGNAL(clicked(bool)), this, SLOT(shortcutClearButton_clicked(bool)));
}

PreferencesDialog::~PreferencesDialog() {
	delete ui;
	// TODO: delete ActionListWidgetItems
}

void PreferencesDialog::shortcutResetButton_clicked(bool) {
	mv::ActionListWidgetItem* item = selectedShortcutItem();
	if (!item) return;

	QKeySequence shortcut;
	item->setShortcut(shortcut);
	item->setShortcutIsOverridden(false);
	item->updateDisplay();

	refreshShortcutControls();
}

void PreferencesDialog::shortcutClearButton_clicked(bool) {
	mv::ActionListWidgetItem* item = selectedShortcutItem();
	if (!item) return;

	QKeySequence shortcut;
	item->setShortcut(shortcut);
	item->setShortcutIsOverridden(true);
	item->updateDisplay();
	
	refreshShortcutControls();
}

void PreferencesDialog::rebuildCurrentTab() {
	QWidget* currentWidget = ui->tabWidget->currentWidget();
	openedTabs_[currentWidget] = true;

	if (currentWidget == ui->shortcutsTab) {
		ui->shortcutListWidget->clear();

		mv::ActionVector actions = mv::Application::instance()->actions();
		for (unsigned i = 0; i < actions.size(); i++) {
			mv::Action* action = actions[i];
			mv::ActionListWidgetItem* item = new mv::ActionListWidgetItem(action);
			QKeySequence ks = mv::Application::instance()->actionShortcut(action->name());
			item->setShortcutIsOverridden(mv::Application::instance()->actionShortcutIsOverridden(action->name()));
			item->setShortcut(ks);
			item->updateDisplay();
			ui->shortcutListWidget->addItem(item);
		}

		refreshShortcutControls();
	} else if (currentWidget == ui->generalTab) {

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
	rebuildCurrentTab();
}

void PreferencesDialog::tabWidget_currentChanged(int) {
	rebuildCurrentTab();
}

void PreferencesDialog::shortcutListWidget_currentItemChanged(QListWidgetItem*, QListWidgetItem*) {
	mv::ActionListWidgetItem* item = selectedShortcutItem();
	if (!item) return;
	
	refreshShortcutControls();
}

void PreferencesDialog::shortcutKeySequenceEdit_editingFinished() {
	mv::ActionListWidgetItem* item = selectedShortcutItem();
	if (!item) return;
	
	item->setShortcutIsOverridden(true);
	item->setShortcut(ui->shortcutKeySequenceEdit->keySequence());
	item->updateDisplay();

	refreshShortcutControls();
}

void PreferencesDialog::buttonBox_accepted() {
	mv::Settings settings;

	if (openedTabs_.find(ui->shortcutsTab) != openedTabs_.end()) {
		settings.beginGroup("shortcuts");
		for (int i = 0; i < ui->shortcutListWidget->count(); i++) {
			mv::ActionListWidgetItem* item = dynamic_cast<mv::ActionListWidgetItem*>(ui->shortcutListWidget->item(i));
			if (!item->shortcutIsOverridden()) {
				settings.remove(item->action()->name());
			} else {
				settings.setValue(item->action()->name(), item->shortcut().toString());
			}
		}
		settings.endGroup();
	}
}

void PreferencesDialog::refreshShortcutControls() {
	mv::ActionListWidgetItem* item = selectedShortcutItem();
	bool itemSelected = item ? true : false;
	ui->shortcutKeySequenceLabel->setEnabled(itemSelected);
	ui->shortcutKeySequenceEdit->setEnabled(itemSelected);
	ui->shortcutClearButton->setEnabled(itemSelected);
	ui->shortcutResetButton->setEnabled(itemSelected);

	if (!itemSelected) return;

	ui->shortcutKeySequenceEdit->setKeySequence(item->shortcut());
	ui->shortcutClearButton->setEnabled(!item->shortcut().isEmpty());
	ui->shortcutResetButton->setEnabled(item->shortcutIsOverridden());
}
