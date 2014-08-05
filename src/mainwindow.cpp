#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "messageboxes.h"
#include "settings.h"
#include "simplefunctions.h"

XGraphicsView::XGraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
	clicked_ = false;
	setAcceptDrops(true);
}

void XGraphicsView::scrollContentsBy(int x, int y) {
	QGraphicsView::scrollContentsBy(x, y); // Override scrollContentsBy to disable scrolling
}

void XGraphicsView::mousePressEvent(QMouseEvent* event) {
	QGraphicsView::mousePressEvent(event);
	clicked_ = true;
	emit mousePress(event);
}

void XGraphicsView::mouseReleaseEvent(QMouseEvent* event) {
	QGraphicsView::mouseReleaseEvent(event);
	clicked_ = false;
	emit mouseRelease(event);
}

void XGraphicsView::mouseMoveEvent(QMouseEvent* event) {
	QGraphicsView::mouseMoveEvent(event);
	if (clicked_) emit mouseDrag(event);
}

// Will only return a non-empty string if the dropped item is supported
QString XGraphicsView::dragDropFilePath(QDropEvent* event) {
	QList<QUrl> urls = event->mimeData()->urls();
	if (!urls.size()) return "";
	QUrl url = urls[0];
	if (!url.isLocalFile()) return "";
	QString filePath = url.toLocalFile();
	if (!mv::Application::instance()->isSupportedFile(filePath)) return "";
	return filePath;
}

void XGraphicsView::dragEnterEvent(QDragEnterEvent* event) {
	QString filePath = dragDropFilePath((QDropEvent*)event);
	if (filePath == "") return;
	event->acceptProposedAction();
}

void XGraphicsView::dragMoveEvent(QDragMoveEvent* event) {
	QString filePath = dragDropFilePath((QDropEvent*)event);
	if (filePath == "") return;
	event->acceptProposedAction();
}

void XGraphicsView::dropEvent(QDropEvent* event) {
	QString filePath = dragDropFilePath(event);
	if (filePath == "") return;
	event->acceptProposedAction();
	mv::Application::instance()->setSource(filePath);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), pixmapCache_(3) {
	ready_ = false;
	updateDisplayTimer_ = NULL;
	loopPixmapItem_ = NULL;
	hideLoopItemTimer_ = NULL;
	loopPixmap_ = NULL;
	pixmap_ = NULL;
	rotation_ = 0;
	invalidated_ = true;
	selectionInvalidated_ = true;
	autoFit_ = true;
	loopAnimationPlaying_ = false;
	selectionP1_ = QPoint(0,0);
	selectionP2_ = QPoint(0,0);

	mv::messageBoxes::setParent(this);

	possibleZoomValues_.push_back(1.0/128.0);
	possibleZoomValues_.push_back(1.0/64.0);
	possibleZoomValues_.push_back(1.0/32.0);
	possibleZoomValues_.push_back(1.0/16.0);
	possibleZoomValues_.push_back(1.0/8.0);
	possibleZoomValues_.push_back(1.0/4.0);
	possibleZoomValues_.push_back(1.0/2.0);
	possibleZoomValues_.push_back(1.0/1.5);
	possibleZoomValues_.push_back(1.0);
	possibleZoomValues_.push_back(1.5);
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
	view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	pixmapItem_ = new QGraphicsPixmapItem();
	scene_->addItem(pixmapItem_);

	selectionRectItem_ = new QGraphicsRectItem();
	selectionRectItem_->setBrush(QBrush(QColor(255,255,255,25)));
	selectionRectItem_->setPen(QPen(Qt::black));
	selectionRectItem_->setZValue(1000);
	selectionRectItem_->setVisible(false);
	selectionRectItem_->setRect(0,0,0,0);
	scene_->addItem(selectionRectItem_);

	selectionRectItem2_ = new QGraphicsRectItem();
	selectionRectItem2_->setBrush(QBrush(QColor(255,255,255,0)));
	QPen pen;
	QVector<qreal> dashes;
	dashes << 4 << 4;
	pen.setDashPattern(dashes);
	pen.setColor(Qt::white);
	selectionRectItem2_->setPen(pen);
	selectionRectItem2_->setZValue(1001);
	selectionRectItem2_->setVisible(false);
	selectionRectItem2_->setRect(0,0,0,0);
	scene_->addItem(selectionRectItem2_);

	splitter_ = new QSplitter(this);
	splitter_->setOrientation(Qt::Vertical);
	console_ = new mv::ConsoleWidget(this);
	splitter_->addWidget(view_);
	splitter_->addWidget(console_);
	console_->hide();
	connect(splitter_, SIGNAL(splitterMoved(int, int)), this, SLOT(splitter_splitterMoved(int, int)));

	toolbar_ = new QToolBar(this);
	toolbar_->setStyleSheet("QToolButton:hover { background-color: rgba(0, 0, 0, 10%); } QToolButton:!hover { color: rgba(0, 0, 0, 0%); }");

	ui->centralwidget->layout()->addWidget(toolbar_);
	ui->centralwidget->layout()->addWidget(splitter_);

	connect(view_, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(view_mousePress(QMouseEvent*)));
	connect(view_, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(view_mouseRelease(QMouseEvent*)));
	connect(view_, SIGNAL(mouseDrag(QMouseEvent*)), this, SLOT(view_mouseDrag(QMouseEvent*)));

	view_->show();

	ready_ = true;
	invalidate();
}

MainWindow::~MainWindow() {
	// TODO: delete objects
	delete ui;
}

QToolBar* MainWindow::toolbar() const {
	return toolbar_;
}

void MainWindow::clearSelection() {
	selectionP1_ = QPoint(0,0);
	selectionP2_ = QPoint(0,0);
	invalidateSelection();
}

bool MainWindow::selectionOn() const {
	QRect r = selectionRect();
	return r.isValid();
}

void MainWindow::showConsole(bool doShow) {
	if (console()->isHidden() && !doShow) return;
	if (!console()->isHidden() && doShow) return;

	if (doShow) {
		console()->show();
		mv::Settings settings;
		QVariant v = settings.value("applicationWindow/consoleHeight");
		int consoleHeight = v.isNull() ? 200 : v.toInt();
		QSize winSize = ui->centralwidget->size();
		QList<int> sizes;
		sizes << winSize.height() - consoleHeight << consoleHeight;
		splitter_->setSizes(sizes);
	} else {
		console()->hide();
	}

	invalidate();
}

QRect MainWindow::selectionRect() const {
	int x1 = selectionP1_.x();
	int x2 = selectionP2_.x();
	int y1 = selectionP1_.y();
	int y2 = selectionP2_.y();
	QRect output;
	output.setLeft(x1 < x2 ? x1 : x2);
	output.setTop(y1 < y2 ? y1 : y2);
	output.setWidth((x1 > x2 ? x1 : x2) - output.x());
	output.setHeight((y1 > y2 ? y1 : y2) - output.y());
	if (output.x() < 0) output.setX(0);
	if (output.y() < 0) output.setY(0);
	if (pixmap_) {
		if (output.right() > pixmap_->width()) output.setWidth(pixmap_->width() - output.x());
		if (output.bottom() > pixmap_->height()) output.setHeight(pixmap_->height() - output.y());
	}
	return output;
}

QPoint MainWindow::mapViewToPixmapItem(const QPoint& point) const {
	QPointF p = view_->mapToScene(point);
	QPointF p2 = pixmapItem_->mapFromScene(p);
	QPoint output(floor(p2.x()), floor(p2.y()));
	if (autoFit()) {
		float z = fitZoom();
		if (z != 0) {
			output.setX((float)output.x() / z);
			output.setY((float)output.y() / z);
		}
	}
	return output;
}

QRectF MainWindow::mapPixmapItemToView(const QRect& rect) const {
	if (autoFit()) {
		float z = fitZoom();
		QRectF output;
		output.setX(rect.x() * z + pixmapItem_->x());
		output.setY(rect.y() * z + pixmapItem_->y());
		output.setWidth(rect.width() * z);
		output.setHeight(rect.height() * z);
		return output;
	}
	return pixmapItem_->mapToScene(rect).boundingRect();
}

void MainWindow::view_mousePress(QMouseEvent* event) {
	selectionP1_ = mapViewToPixmapItem(event->pos());
	selectionP2_ = selectionP1_;
	invalidateSelection();
}

void MainWindow::view_mouseRelease(QMouseEvent* event) {
	selectionP2_ = mapViewToPixmapItem(event->pos());
	invalidateSelection();
}

void MainWindow::view_mouseDrag(QMouseEvent* event) {
	selectionP2_ = mapViewToPixmapItem(event->pos());
	invalidateSelection();
}

void MainWindow::toggleConsole() {
	showConsole(console()->isHidden());
}

void MainWindow::showStatusBar(bool doShow) {
	if (doShow) {
		statusBar()->show();
	} else {
		statusBar()->hide();
	}
}

bool MainWindow::statusBarShown() const {
	return !statusBar()->isHidden();
}

void MainWindow::toggleStatusBar() {
	showStatusBar(statusBar()->isHidden());
}

void MainWindow::showToolbar(bool doShow) {
	if (doShow) {
		toolbar()->show();
	} else {
		toolbar()->hide();
	}
}

bool MainWindow::toolbarShown() const {
	return !toolbar()->isHidden();
}

void MainWindow::toggleToolbar() {
	showToolbar(toolbar()->isHidden());
}

mv::ConsoleWidget* MainWindow::console() const {
	return console_;
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
	
	label->setText(" " + value + " ");
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

	QSize winSize = viewContainerSize();

	loopPixmapItem_->setPos((winSize.width() - loopPixmap_->width()) / 2, (winSize.height() - loopPixmap_->height()) / 2);
	hideLoopItemTimer_->stop();
	hideLoopItemTimer_->start();
	loopAnimationPlaying_ = true;
	invalidate();
}

void MainWindow::splitter_splitterMoved(int, int) {
	mv::Settings settings;
	settings.setValue("applicationWindow/consoleHeight", splitter_->sizes()[1]);
	invalidate();
}

void MainWindow::hideLoopItemTimer_timeout() {
	loopAnimationPlaying_ = false;
	invalidate();
}

QSize MainWindow::viewContainerSize() const {
	int h = splitter_->sizes()[0];
	int w = splitter_->size().width();
	return QSize(w, h);
}

void MainWindow::invalidate() {
	invalidated_ = true;
	selectionInvalidated_ = true;
	update();
}

void MainWindow::invalidateSelection() {
	selectionInvalidated_ = true;
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

void MainWindow::closeEvent(QCloseEvent*) {
	emit closed();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	emit keypressed(event);
}

void MainWindow::clearSourceAndCache() {
	source_ = "";
	pixmap_ = NULL;
	pixmapCache_.clear();
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
	setRotation(0);
	source_ = v;
	pixmap_ = source_ == "" ? NULL : loadSource(source_);
	clearSelection();
	invalidate();
}

void MainWindow::reloadSource() {
	int previousWidth = 0;
	int previousHeight = 0;
	if (pixmap_) {
		previousWidth = pixmap_->width();
		previousHeight = pixmap_->height();
	}
	pixmapCache_.remove(source());
	pixmap_ = loadSource(source_);
	if (!pixmap_ || pixmap_->width() != previousWidth || pixmap_->height() != previousHeight) {
		clearSelection();
	}
	invalidate();
}

QString MainWindow::source() const {
	return source_;
}

void MainWindow::setRotation(int v) {
	v = v % 360;
	if (v < 0) v = 360 + v;
	if (rotation_ == v) return;
	rotation_ = v;
	invalidate();
}

QPixmap* MainWindow::pixmap() const {
	return pixmap_;
}

int MainWindow::rotation() const {
	return rotation_;
}

bool MainWindow::rotated() const {
	return rotation_ == 90 || rotation_ == 270;
}

void MainWindow::setAutoFit(bool v) {
	if (autoFit_ == v) return;
	autoFit_ = v;

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

	if (selectionInvalidated_) {
		selectionInvalidated_ = false;
		updateSelectionDisplay();
	}

	QMainWindow::paintEvent(event);
}

float MainWindow::fitZoom() const {
	if (!pixmap_) return 1;

	bool rotated = this->rotated();
	QSize winSize = viewContainerSize();

	int pixmapWidth = rotated ? pixmap_->height() : pixmap_->width();
	int pixmapHeight = rotated ? pixmap_->width() : pixmap_->height();

	float rw = (float)winSize.width() / (float)pixmapWidth;
	float rh = (float)winSize.height() / (float)pixmapHeight;

	return rw < rh ? rw : rh;
}

void MainWindow::updateDisplay(int renderingType) {
	if (!ready_) return;

	invalidated_ = false;

	QSize winSize = viewContainerSize();
	
	if (winSize.width() <= 0 || winSize.height() <= 0) return;
	
	if (!pixmap_) {
		pixmapItem_->setPixmap(QPixmap());
	} else {
		// Calculate scale factor so that the picture fits within the view
		int pixmapWidth = pixmap_->width();
		int pixmapHeight = pixmap_->height();

		// If not autofit, we use the zoom level as it was before zooming
		// so that if the window is resized the zoom remains constant.
		if (!beforeScaleFitZoom_) beforeScaleFitZoom_ = fitZoom();
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

	updateSelectionDisplay();
}

void MainWindow::updateSelectionDisplay() {
	selectionInvalidated_ = false;

	if (!selectionOn()) {
		selectionRectItem_->setVisible(false);
		selectionRectItem2_->setVisible(false);
	} else {
		QRectF r = mapPixmapItemToView(selectionRect());
		selectionRectItem_->setVisible(true);
		selectionRectItem_->setRect(r);
		selectionRectItem2_->setVisible(true);
		selectionRectItem2_->setRect(r);
	}
}
