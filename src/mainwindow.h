#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QShowEvent>
#include <QTimer>

class XGraphicsView: public QGraphicsView {

public:

	XGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);

protected:

	void scrollContentsBy(int, int);

};

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
	void setSource(const QString& v);
	QString source() const;
	void reloadSource();
	void doLoopAnimation();

protected:

	void resizeEvent(QResizeEvent*);
	void showEvent(QShowEvent* event);
	void keyPressEvent(QKeyEvent* event);

private:

	QTimer* updateDisplayTimer() const;
	
	Ui::MainWindow *ui;
	QGraphicsPixmapItem* pixmapItem_;
	QGraphicsScene* scene_;
	QGraphicsView* view_;
	QString lastUpdateTag_;
	QPixmap* pixmap_;
	mutable QTimer* updateDisplayTimer_;
	QString source_;
	QGraphicsPixmapItem* loopPixmapItem_;
	QTimer* hideLoopItemTimer_;

public slots:

	void updateDisplayTimer_timeout();
	void hideLoopItemTimer_timeout();

signals:

	void keypressed(QKeyEvent* event);

};

#endif // MAINWINDOW_H
