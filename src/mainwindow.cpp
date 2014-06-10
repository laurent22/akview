#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

#include <QDebug>

XGraphicsView::XGraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {}
void XGraphicsView::scrollContentsBy(int, int) { /* Override scrollContentsBy to disable scrolling */ }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ready_ = false;
	updateDisplayTimer_ = NULL;
	lastUpdateTag_ = "";
	loopPixmapItem_ = NULL;
	hideLoopItemTimer_ = NULL;
	rotation_ = 0;
	invalidated_ = true;

	ui->setupUi(this);

	scene_ = new QGraphicsScene(this);
	scene_->setBackgroundBrush(QBrush(Qt::black));

	view_ = new XGraphicsView(scene_, this);

	view_->setEnabled(false);
	view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	pixmap_ = new QPixmap();
	pixmapItem_ = new QGraphicsPixmapItem();
	pixmapItem_->setTransformationMode(Qt::SmoothTransformation);

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

void MainWindow::doLoopAnimation() {
	if (!loopPixmapItem_) {
		loopPixmapItem_ = new QGraphicsPixmapItem(QPixmap(":/loop.png"));
		hideLoopItemTimer_ = new QTimer();
		hideLoopItemTimer_->setInterval(1000);
		hideLoopItemTimer_->setSingleShot(true);
		connect(hideLoopItemTimer_, SIGNAL(timeout()), this, SLOT(hideLoopItemTimer_timeout()));
	}

	scene_->addItem(loopPixmapItem_);
	loopPixmapItem_->setPos(10, 10);
	hideLoopItemTimer_->stop();
	hideLoopItemTimer_->start();
}

void MainWindow::hideLoopItemTimer_timeout() {
	scene_->removeItem(loopPixmapItem_);
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

void MainWindow::setSource(const QString& v) {
	if (source_ == v) return;
	source_ = v;

	pixmap_->load(source_);

	invalidate();
}

void MainWindow::reloadSource() {
	lastUpdateTag_ = "";
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

void MainWindow::paintEvent(QPaintEvent* event) {
	if (invalidated_) {
		invalidated_ = false;
		updateDisplay(FullRendering);
	}

	QMainWindow::paintEvent(event);
}

void MainWindow::updateDisplay(int renderingType) {
	if (!ready_) return;

	invalidated_ = false;

	QSize winSize = ui->centralwidget->size();

	QString updateTag = QString("%1_%2_%3_%4").arg(winSize.width()).arg(winSize.height()).arg(renderingType).arg(source_);
	if (lastUpdateTag_ == updateTag) return;
	lastUpdateTag_ = updateTag;
	
	if (winSize.width() <= 0 || winSize.height() <= 0) return;
	
	if (pixmap_->isNull()) {
		pixmapItem_->setPixmap(*pixmap_);
	} else {
		bool rotated = rotation_ != 0 && rotation_ != 360;

		QPixmap scaledPixmap = pixmap_->scaled(rotated ? winSize.height() : winSize.width(), rotated ? winSize.width() : winSize.height(), Qt::KeepAspectRatio, renderingType == QuickRendering ? Qt::FastTransformation : Qt::SmoothTransformation);
		pixmapItem_->setPixmap(scaledPixmap);
		pixmapItem_->setTransformOriginPoint(QPointF((double)scaledPixmap.width() / 2.0, (double)scaledPixmap.height() / 2.0));
		pixmapItem_->setRotation(rotation_);
		pixmapItem_->setPos(
			floor((winSize.width() - scaledPixmap.size().width()) / 2),
			floor((winSize.height() - scaledPixmap.size().height()) / 2)
		);
	}

	scene_->setSceneRect(QRect(0, 0, winSize.width(), winSize.height()));
}
