#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCache>
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QShowEvent>
#include <QSplitter>
#include <QTimer>

#include "consolewidget.h"
#include "simpletypes.h"

class XGraphicsView: public QGraphicsView {

	Q_OBJECT

public:

	XGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);

protected:

	void scrollContentsBy(int x, int y);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);

private:

	QString dragDropFilePath(QDropEvent* event);
	bool clicked_;

signals:

	void mousePress(QMouseEvent* event);
	void mouseRelease(QMouseEvent* event);
	void mouseDrag(QMouseEvent* event);

};

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:

	static const int FullRendering = 1;
	static const int QuickRendering = 2;

	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void updateDisplay(int renderingType);
	void updateSelectionDisplay();
	void setSource(const QString& v);
	QString source() const;
	void clearSource();
	void reloadSource();
	void doLoopAnimation();
	void setRotation(int v);
	int rotation() const;
	void invalidate();
	void invalidateSelection();
	void zoomIn();
	void zoomOut();
	void resetZoom();
	float zoom() const;
	QRect selectionRect() const;
	void setAutoFit(bool v);
	bool autoFit() const;
	float fitZoom() const;
	void setStatusItem(const QString& name, const QString& value);
	int zoomIndex() const;
	QPixmap* loadSource(const QString& sourcePath);
	QPixmap* pixmap() const;
	mv::ConsoleWidget* console() const;
	void showConsole(bool doShow = true);
	void toggleConsole();
	bool selectionOn() const;
	void clearSelection();

protected:

	void resizeEvent(QResizeEvent*);
	void showEvent(QShowEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void paintEvent(QPaintEvent* event);

private:

	QTimer* updateDisplayTimer() const;
	void setZoomIndex(int v);
	QSize viewContainerSize() const;
	QPoint mapViewToPixmapItem(const QPoint& point) const;
	QRectF mapPixmapItemToView(const QRect& rect) const;
	
	Ui::MainWindow *ui;
	QGraphicsPixmapItem* pixmapItem_;
	QGraphicsScene* scene_;
	QGraphicsView* view_;
	QPixmap* pixmap_;
	mutable QTimer* updateDisplayTimer_;
	QString source_;
	QGraphicsPixmapItem* loopPixmapItem_;
	QPixmap* loopPixmap_;
	QTimer* hideLoopItemTimer_;
	bool loopAnimationPlaying_;
	int rotation_;
	bool invalidated_;
	bool selectionInvalidated_;
	bool ready_;
	bool autoFit_;
	FloatVector possibleZoomValues_;
	int zoomIndex_;
	int noZoomIndex_;
	float beforeScaleFitZoom_;
	QStringQLabelMap statusLabels_;
	QCache<QString, QPixmap> pixmapCache_;
	QSplitter* splitter_;
	mv::ConsoleWidget* console_;
	QGraphicsRectItem* selectionRectItem_;
	QGraphicsRectItem* selectionRectItem2_;
	QRect selectionRect_;
	QPoint selectionP1_;
	QPoint selectionP2_;

public slots:

	void updateDisplayTimer_timeout();
	void hideLoopItemTimer_timeout();
	void splitter_splitterMoved(int pos, int index);
	void view_mousePress(QMouseEvent* event);
	void view_mouseRelease(QMouseEvent* event);
	void view_mouseDrag(QMouseEvent* event);

signals:

	void keypressed(QKeyEvent* event);

};

#endif // MAINWINDOW_H
