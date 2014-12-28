#include "batchdialog.h"
#include "ui_batchdialog.h"

#include "application.h"
#include "settings.h"
#include "stringutil.h"

BatchDialog::BatchDialog(QWidget *parent) : QDialog(parent), ui(new Ui::BatchDialog) {
	ui->setupUi(this);

	firstShow_ = true;

	ui->fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

	connect(ui->addFilesButton, SIGNAL(clicked()), this, SLOT(addFilesButton_clicked()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeButton_clicked()));
	connect(ui->removeAllButton, SIGNAL(clicked()), this, SLOT(removeAllButton_clicked()));
	connect(ui->fileListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(fileListWidget_currentRowChanged(int)));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBox_accepted()));

	mv::ActionVector actions = mv::Application::instance()->actions();
	for (unsigned int i = 0; i < actions.size(); i++) {
		mv::Action* action = actions[i];
		if (!action->batchModeSupported()) continue;
		ui->actionCombobox->addItem(action->text(), QVariant(action->id()));
	}

	updateButtons();
}

BatchDialog::~BatchDialog() {
	delete ui;
}

void BatchDialog::showEvent(QShowEvent* event) {
	QDialog::showEvent(event);

	if (firstShow_) {
		firstShow_ = false;
		QTimer::singleShot(500, this, SLOT(addFilesButton_clicked()));
	}
}

void BatchDialog::buttonBox_accepted() {
	QStringList filePaths = addedFilePaths();
	if (!filePaths.size()) return; // normally not possible
	QString actionId = ui->actionCombobox->currentData().toString();
	
	mv::Application::instance()->execAction(actionId, filePaths);
	accept();
}

void BatchDialog::addFilesButton_clicked() {
	mv::Settings settings;

	QString lastOpenFileDirectory = settings.value("lastOpenFileDirectory").toString();

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(mv::Application::instance()->supportedFilesFilter());
	if (lastOpenFileDirectory != "") dialog.setDirectory(lastOpenFileDirectory);
	int result = dialog.exec();
	if (!result) return;

	settings.setValue("lastOpenFileDirectory", QVariant(dialog.directory().absolutePath()));

	QStringList files = dialog.selectedFiles();
	for (int i = 0; i < ui->fileListWidget->count(); i++) {
		 QListWidgetItem* item = ui->fileListWidget->item(i);
		 files.append(item->text());
	}

	std::sort(files.begin(), files.end());
	files.erase(std::unique(files.begin(), files.end()), files.end());
	std::sort(files.begin(), files.end(), mv::stringutil::NaturalSortCompare());

	ui->fileListWidget->clear();
	ui->fileListWidget->addItems(files);

	updateButtons();
}

void BatchDialog::removeButton_clicked() {
	QList<QListWidgetItem*> items = ui->fileListWidget->selectedItems();
	int newIndex = -1;
	for (int i = ui->fileListWidget->count() - 1; i >= 0; i--) {
		QListWidgetItem* item = ui->fileListWidget->item(i);
		if (!item->isSelected()) continue;
		newIndex = i;
		item = ui->fileListWidget->takeItem(i);
		delete item;
	}

	if (newIndex >= 0 && ui->fileListWidget->count()) {
		if (newIndex >= ui->fileListWidget->count()) newIndex = ui->fileListWidget->count() - 1;
		ui->fileListWidget->setCurrentRow(newIndex);
	}

	updateButtons();
}

QStringList BatchDialog::addedFilePaths() const {
	QStringList output;
	for (int i = 0; i < ui->fileListWidget->count(); i++) {
		 QListWidgetItem* item = ui->fileListWidget->item(i);
		 output << item->text();
	}
	return output;
}

void BatchDialog::removeAllButton_clicked() {
	ui->fileListWidget->clear();
	updateButtons();
}

void BatchDialog::fileListWidget_currentRowChanged(int) {
	updateButtons();
}

void BatchDialog::updateButtons() {
	QStringList filePaths = addedFilePaths();
	ui->removeButton->setEnabled(ui->fileListWidget->currentItem() != NULL);
	ui->removeAllButton->setEnabled(filePaths.size() > 0);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(filePaths.size() > 0);
}