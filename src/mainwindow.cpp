#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

#include <QDebug>

XGraphicsView::XGraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {}
void XGraphicsView::scrollContentsBy(int x, int y) { QGraphicsView::scrollContentsBy(x, y); /* Override scrollContentsBy to disable scrolling */ }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), pixmapCache_(3) {
	ready_ = false;
	updateDisplayTimer_ = NULL;
	loopPixmapItem_ = NULL;
	hideLoopItemTimer_ = NULL;
	loopPixmap_ = NULL;
	pixmap_ = NULL;
	rotation_ = 0;
	invalidated_ = true;
	autoFit_ = true;
	loopAnimationPlaying_ = false;

	possibleZoomValues_.push_back(1.0/128.0);
	possibleZoomValues_.push_back(1.0/64.0);
	possibleZoomValues_.push_back(1.0/32.0);
	possibleZoomValues_.push_back(1.0/16.0);
	possibleZoomValues_.push_back(1.0/8.0);
	possibleZoomValues_.push_back(1.0/4.0);
	possibleZoomValues_.push_back(1.0/2.0);
	possibleZoomValues_.push_back(1.0);
	possibleZoomValues_.push_back(2.0);
	possibleZoomValues_.push_back(4.0);
	possibleZoomValues_.push_back(8.0);
	possibleZoomValues_.push_back(16.0);
	possibleZoomValues_.push_back(32.0);
	possibleZoomValues_.push_back(64.0);
	possibleZoomValues_.push_back(128.0);
	zoomIndex_ = possibleZoomValues_.size() / 2;
	noZoomIndex_ = zoomIndex_;

	ui->setupUi(this);

	scene_ = new QGraphicsScene(this);
	scene_->setBackgroundBrush(QBrush(Qt::black));

	view_ = new XGraphicsView(scene_, this);

	view_->setEnabled(false);
	view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//pixmap_ = new QPixmap();
	pixmapItem_ = new QGraphicsPixmapItem();

	scene_->addItem(pixmapItem_);

	ui->centralwidget->layout()->addWidget(view_);

	view_->show();

	ready_ = true;
	invalidate();
}

MainWindow::~MainWindow() {
	// TODO: delete objects
	delete ui;
}

void MainWindow::setStatusItem(const QString& name, const QString& value) {
	QLabel* label = NULL;

	if (statusLabels_.find(name) == statusLabels_.end()) {
		label = new QLabel(this);
		statusBar()->insertPermanentWidget(0, label);	
		statusLabels_[name] = label;	
	} else {
		label = statusLabels_[name];
	}
	
	label->setText(value);
}

void MainWindow::doLoopAnimation() {
	if (!loopPixmapItem_) {
		loopPixmap_ = new QPixmap(":/loop.png");
		loopPixmapItem_ = new QGraphicsPixmapItem(*loopPixmap_);
		loopPixmapItem_->setVisible(false);
		scene_->addItem(loopPixmapItem_);

		hideLoopItemTimer_ = new QTimer();
		hideLoopItemTimer_->setInterval(1000);
		hideLoopItemTimer_->setSingleShot(true);
		connect(hideLoopItemTimer_, SIGNAL(timeout()), this, SLOT(hideLoopItemTimer_timeout()));
	}

	QSize winSize = ui->centralwidget->size();

	loopPixmapItem_->setPos((winSize.width() - loopPixmap_->width()) / 2, (winSize.height() - loopPixmap_->height()) / 2);
	hideLoopItemTimer_->stop();
	hideLoopItemTimer_->start();
	loopAnimationPlaying_ = true;
	invalidate();
}

void MainWindow::hideLoopItemTimer_timeout() {
	loopAnimationPlaying_ = false;
	invalidate();
}

void MainWindow::invalidate() {
	invalidated_ = true;
	update();
}

QTimer* MainWindow::updateDisplayTimer() const {
	if (updateDisplayTimer_) return updateDisplayTimer_;

	updateDisplayTimer_ = new QTimer();
	updateDisplayTimer_->setSingleShot(true);
	updateDisplayTimer_->setInterval(600);
	connect(updateDisplayTimer_, SIGNAL(timeout()), this, SLOT(updateDisplayTimer_timeout()));
	return updateDisplayTimer_;
}

void MainWindow::updateDisplayTimer_timeout() {
	updateDisplay(FullRendering);
}

void MainWindow::showEvent(QShowEvent* event) {
	if (event->spontaneous()) return;
	updateDisplay(FullRendering);
}

void MainWindow::resizeEvent(QResizeEvent*) {
	updateDisplay(QuickRendering);

	updateDisplayTimer()->stop();
	updateDisplayTimer()->start();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	emit keypressed(event);
}

void MainWindow::clearSource() {
	pixmap_ = NULL; // QCache will take care of deleting the object
	invalidate();
}

// Loads the source in memory but doesn't display it
QPixmap* MainWindow::loadSource(const QString& sourcePath) {
	if (pixmapCache_.contains(sourcePath)) {
		return pixmapCache_[sourcePath];
	}

	QPixmap* pixmap = new QPixmap();
	pixmap->load(sourcePath);
	pixmapCache_.insert(sourcePath, pixmap);
	return pixmap;
}

void MainWindow::setSource(const QString& v) {
	if (source_ == v) return;
	source_ = v;
	if (pixmap_) pixmap_ = NULL; // QCache will take care of deleting the object
	pixmap_ = loadSource(source_);
	invalidate();
}

void MainWindow::reloadSource() {
	QString s = source();
	source_ = "";
	setSource(s);
}

QString MainWindow::source() const {
	return source_;
}

void MainWindow::setRotation(int v) {
	if (rotation_ == v) return;
	rotation_ = v;
	invalidate();
}

int MainWindow::rotation() const {
	return rotation_;
}

void MainWindow::setAutoFit(bool v) {
	if (autoFit_ == v) return;
	autoFit_ = v;
	
	view_->setEnabled(!autoFit_);

	if (autoFit_) {
		view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	} else {
		view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		view_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}

	if (autoFit_) zoomIndex_ = noZoomIndex_;
}

bool MainWindow::autoFit() const {
	return autoFit_;
}

int MainWindow::zoomIndex() const {
	return zoomIndex_;
}

void MainWindow::setZoomIndex(int v) {
	if (v >= (int)possibleZoomValues_.size()) v = (int)possibleZoomValues_.size() - 1;
	if (v < 0) v = 0;
	if (v == zoomIndex_) return;

	zoomIndex_ = v;
	setAutoFit(zoomIndex_ == noZoomIndex_);
	if (zoomIndex_ != noZoomIndex_) beforeScaleFitZoom_ = fitZoom();
	invalidate();
}

void MainWindow::resetZoom() {
	setZoomIndex(noZoomIndex_);
}

void MainWindow::zoomIn() {
	setZoomIndex(zoomIndex_ + 1);	
}

void MainWindow::zoomOut() {
	setZoomIndex(zoomIndex_ - 1);
}

float MainWindow::zoom() const {
	return possibleZoomValues_[zoomIndex_];
}

void MainWindow::paintEvent(QPaintEvent* event) {
	if (invalidated_) {
		invalidated_ = false;
		updateDisplay(FullRendering);
	}

	QMainWindow::paintEvent(event);
}

float MainWindow::fitZoom() const {
	if (!pixmap_) return 1;

	bool rotated = rotation_ != 0 && rotation_ != 360;
	QSize winSize = ui->centralwidget->size();

	int pixmapWidth = rotated ? pixmap_->height() : pixmap_->width();
	int pixmapHeight = rotated ? pixmap_->width() : pixmap_->height();

	float rw = (float)winSize.width() / (float)pixmapWidth;
	float rh = (float)winSize.height() / (float)pixmapHeight;

	return rw < rh ? rw : rh;
}

void MainWindow::updateDisplay(int renderingType) {
	if (!ready_) return;

	invalidated_ = false;

	QSize winSize = ui->centralwidget->size();
	
	if (winSize.width() <= 0 || winSize.height() <= 0) return;
	
	if (!pixmap_) {
		pixmapItem_->setPixmap(QPixmap());
	} else {
		bool rotated = rotation_ != 0 && rotation_ != 360;

		// Calculate scale factor so that the picture fits within the view
		int pixmapWidth = rotated ? pixmap_->height() : pixmap_->width();
		int pixmapHeight = rotated ? pixmap_->width() : pixmap_->height();

		// If not autofit, we use the zoom level as it was before zooming
		// so that if the window is resized the zoom remains constant.
		float zoom = autoFit_ ? fitZoom() : beforeScaleFitZoom_;

		// If we're not trying to fit the photo within the view, we apply the user supplied zoom
		if (!autoFit_) zoom = zoom * this->zoom();
		
		// If autoFit, we use a nicely scaled pixmap. If not, scaling is done
		// via QGraphicsItem (no smoothing).
		QPixmap drawnPixmap = autoFit_ ? pixmap_->scaled(zoom * (float)pixmapWidth, zoom * (float)pixmapHeight, Qt::KeepAspectRatio, renderingType == QuickRendering ? Qt::FastTransformation : Qt::SmoothTransformation) : *pixmap_;

		pixmapItem_->setPixmap(drawnPixmap);
		pixmapItem_->setScale(autoFit_ ? 1 : zoom);
		pixmapItem_->setTransformOriginPoint(QPointF((double)drawnPixmap.width() / 2.0, (double)drawnPixmap.height() / 2.0));
		pixmapItem_->setRotation(rotation_);
		pixmapItem_->setPos(
			floor((winSize.width() - drawnPixmap.size().width()) / 2),
			floor((winSize.height() - drawnPixmap.size().height()) / 2)
		);

		if (autoFit_) {
			scene_->setSceneRect(QRect(0, 0, winSize.width(), winSize.height()));
		} else {
			QRectF rect(0, 0, drawnPixmap.width(), drawnPixmap.height());
			scene_->setSceneRect(pixmapItem_->mapRectToScene(rect));
		}
	}

	if (loopPixmapItem_) loopPixmapItem_->setVisible(loopAnimationPlaying_);
}
