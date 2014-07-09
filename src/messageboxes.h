#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H

namespace mv {

namespace messageBoxes {

	void setParent(QWidget* parent);
	
	int info(const QString& message, const QString& title = QObject::tr("Information"), const QString& buttons = "ok");
	int confirmation(const QString& message, const QString& title = QObject::tr("Confirmation"), const QString& buttons = "yesNo");
	int error(const QString& message, const QString& title = QObject::tr("Error"), const QString& buttons = "ok");
	int warning(const QString& message, const QString& title = QObject::tr("Warning"), const QString& buttons = "ok");

}
}
#endif // MESSAGEBOXES_H
