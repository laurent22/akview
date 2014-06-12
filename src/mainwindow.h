#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QShowEvent>
#include <QTimer>

#include "simpletypes.h"

class XGraphicsView: public QGraphicsView {

public:

	XGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);

protected:

	void scrollContentsBy(int x, int y);

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
	void setRotation(int v);
	int rotation() const;
	void invalidate();
	void zoomIn();
	void zoomOut();
	float zoom() const;
	void setAutoFit(bool v);
	bool autoFit() const;

protected:

	void resizeEvent(QResizeEvent*);
	void showEvent(QShowEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void paintEvent(QPaintEvent* event);

private:

	QTimer* updateDisplayTimer() const;
	void setZoomIndex(int v);
	
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
	int rotation_;
	bool invalidated_;
	bool ready_;
	bool autoFit_;
	FloatVector possibleZoomValues_;
	int zoomIndex_;
	int noZoomIndex_;

public slots:

	void updateDisplayTimer_timeout();
	void hideLoopItemTimer_timeout();

signals:

	void keypressed(QKeyEvent* event);

};

#endif // MAINWINDOW_H
