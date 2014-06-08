#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

#include <QDebug>

XGraphicsView::XGraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {}
void XGraphicsView::scrollContentsBy(int, int) { /* Override scrollContentsBy to disable scrolling */ }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	updateDisplayTimer_ = NULL;
	lastUpdateTag_ = "";

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

	updateDisplay(FullRendering);
}

MainWindow::~MainWindow() {
	// TODO: delete objects
	delete ui;
}

QTimer* MainWindow::updateDisplayTimer() const {
	if (updateDisplayTimer_) return updateDisplayTimer_;

	updateDisplayTimer_ = new QTimer();
	updateDisplayTimer_->setSingleShot(true);
	updateDisplayTimer_->setInterval(1000);
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

	updateDisplay(QuickRendering);

	updateDisplayTimer()->stop();
	updateDisplayTimer()->start();
}

void MainWindow::reloadSource() {
	QString s = source();
	source_ = "";
	setSource(s);
}

QString MainWindow::source() const {
	return source_;
}

void MainWindow::updateDisplay(int renderingType) {	
	QSize winSize = ui->centralwidget->size();

	QString updateTag = QString("%1_%2_%3_%4").arg(winSize.width()).arg(winSize.height()).arg(renderingType).arg(source_);
	if (lastUpdateTag_ == updateTag) return;
	lastUpdateTag_ = updateTag;
	
	if (winSize.width() <= 0 || winSize.height() <= 0) return;
	
	if (pixmap_->isNull()) {
		pixmapItem_->setPixmap(*pixmap_);
	} else {
		QPixmap scaledPixmap = pixmap_->scaled(winSize.width(), winSize.height(), Qt::KeepAspectRatio, renderingType == QuickRendering ? Qt::FastTransformation : Qt::SmoothTransformation);
		pixmapItem_->setPixmap(scaledPixmap);
	
		pixmapItem_->setPos(
			floor((winSize.width() - scaledPixmap.size().width()) / 2),
			floor((winSize.height() - scaledPixmap.size().height()) / 2)
		);
	}

	scene_->setSceneRect(QRect(0, 0, winSize.width(), winSize.height()));
}
