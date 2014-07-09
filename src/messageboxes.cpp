#include "messageboxes.h"

namespace mv {

namespace messageBoxes {

	namespace {
	
		// Stuff declared here are private
	
		QWidget* parent_ = NULL;
		
		void centerMessageBox(QMessageBox& m) {
			if (!parent_) return;
			QSize s = m.size();
			m.move(parent_->x() + (parent_->width() - s.width()) / 2, parent_->y() + (parent_->height() - s.height()) / 2);
		}
		
		
		void setupMessageBox(QMessageBox& m, const QString& message, const QString& title, const QString& buttons) {
			QString btns = buttons;
			if (buttons == "okCancel") btns = QString("%1|%2").arg(QObject::tr("OK")).arg(QObject::tr("Cancel"));
			if (buttons == "yesNo") btns =  QString("%1|%2").arg(QObject::tr("Yes")).arg(QObject::tr("No"));
			if (buttons == "yesNoCancel") btns =  QString("%1|%2|%3").arg(QObject::tr("Yes")).arg(QObject::tr("No")).arg(QObject::tr("Cancel"));
			if (buttons == "ok") btns = QObject::tr("OK");
			QStringList buttonLabels = btns.split("|");
			for (int i = 0; i < buttonLabels.length(); i++) {
				QString label = buttonLabels[i];
				QMessageBox::ButtonRole role = QMessageBox::ActionRole;
				QString l = label.toLower();
				if (l == QObject::tr("OK").toLower()) role = QMessageBox::AcceptRole;
				if (l == QObject::tr("Cancel").toLower()) role = QMessageBox::RejectRole;
				if (l == QObject::tr("Yes").toLower()) role = QMessageBox::YesRole;
				if (l == QObject::tr("No").toLower()) role = QMessageBox::NoRole;
				if (l == QObject::tr("Exit").toLower()) role = QMessageBox::RejectRole;
				QPushButton* button = m.addButton(label, role);
				button->setMinimumHeight(26);
				
				int padding = 20;
				QFontMetrics metrics(button->font());
				int buttonWidth = metrics.width(label) + padding * 2;
				if (buttonWidth < 100) buttonWidth = 100;
				
				button->setMinimumWidth(buttonWidth);
				button->setMaximumWidth(buttonWidth);
				button->resize(buttonWidth, 26);
			}
			
			m.setWindowTitle(title);
			m.setText(message);
			m.show(); // Must show the message box, otherwise it cannot be centered (size hints are not always accurate)			
			centerMessageBox(m);
		}
		
		
		int customExec(QMessageBox& m) {
			int output = m.exec();
			QMessageBox::ButtonRole role = m.buttonRole(m.clickedButton());
			if (role == QMessageBox::AcceptRole) return QMessageBox::Ok;
			if (role == QMessageBox::RejectRole) return QMessageBox::Cancel;
			if (role == QMessageBox::YesRole) return QMessageBox::Yes;
			if (role == QMessageBox::NoRole) return QMessageBox::NoRole;
			return output;
		}
	
	}
		
void setParent(QWidget* parent) {
	parent_ = parent;	
}


int info(const QString& message, const QString& title, const QString& buttons) {
	QMessageBox m;
	setupMessageBox(m, message, title == "" ? QObject::tr("Information") : title, buttons);
	m.setIcon(QMessageBox::Information);
	return customExec(m);
}


int confirmation(const QString& message, const QString& title, const QString& buttons) {
	QMessageBox m;
	setupMessageBox(m, message, title == "" ? QObject::tr("Confirmation") : title, buttons);
	m.setIcon(QMessageBox::Question);
	return customExec(m);
}


int error(const QString& message, const QString& title, const QString& buttons) {
	QMessageBox m;
	setupMessageBox(m, message, title == "" ? QObject::tr("Error") : title, buttons);
	m.setIcon(QMessageBox::Critical);
	return customExec(m);
}


int warning(const QString& message, const QString& title, const QString& buttons) {
	QMessageBox m;
	setupMessageBox(m, message, title == "" ? QObject::tr("Warning") : title, buttons);
	m.setIcon(QMessageBox::Warning);
	return customExec(m);
}

} // namespace msg
} // namespace hanzi
