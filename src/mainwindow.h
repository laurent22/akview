#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QShowEvent>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	static const int FullRendering = 1;
	static const int QuickRendering = 2;

	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void updateDisplay(int renderingType);

protected:

	void resizeEvent(QResizeEvent*);
	void showEvent(QShowEvent* event);

private:

	QTimer* updateDisplayTimer() const;
	
	Ui::MainWindow *ui;
	QGraphicsPixmapItem* pixmapItem_;
	QGraphicsScene* scene_;
	QGraphicsView* view_;
	QString lastUpdateTag_;
	QPixmap* pixmap_;
	mutable QTimer* updateDisplayTimer_;

public slots:

	void updateDisplayTimer_timeout();

};

#endif // MAINWINDOW_H
