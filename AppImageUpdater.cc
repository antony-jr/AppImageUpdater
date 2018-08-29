#include <AppImageUpdater.hpp>

using namespace AppImageUpdaterBridge;

#define SELF_UPDATER_IDLE_TIME 4000 // 4 Secs for now

/* DR => Dereference pointers. */
#define DR(pointer) (*pointer)

/* Access UserInterface. */
#define AUI(Object) DR(_pUi.Object)

/* AppImage Delta Revisioner Signals. */
#define ADRUA_SIGNAL &AppImageDeltaRevisioner::updateAvailable

AppImageUpdater::AppImageUpdater(QWidget *parent)
	: QWidget(parent , Qt::WindowStaysOnTopHint)
{
    _pUi.setupUi(this);
    setAcceptDrops(true);

    /* Construct tray icon. */
    _pTIcon = new QSystemTrayIcon(this);
    _pTIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
    connect(_pTIcon , &QSystemTrayIcon::activated , this , &AppImageUpdater::showHideWindow);
    _pTIcon->show();
    
    centerPos = QApplication::desktop()->screen()->rect().center() - this->rect().center();	
    this->move(centerPos);
    this->show();

    /* This is oftenly used pixmap , Thus to reduce overhead we are caching it. */
    _pDropHere = QPixmap(QString::fromUtf8(":/dotted_square_drop.png"));
    _pDropNorm = QPixmap(QString::fromUtf8(":/dotted_square.png"));
    _pWindowIcon = QIcon(QPixmap(QString::fromUtf8(":/logo.png")));

    /* Build about message box. */
    {
    QString AboutText;
    AboutText.append("AppImage Updater version ");
    AboutText.append(QString(APPIMAGE_UPDATER_VERSION));
    AboutText.append(" (commit ");
    AboutText.append(QString(APPIMAGE_UPDATER_COMMIT));
    AboutText.append(") , Build  ");
    AboutText.append(QString(APPIMAGE_UPDATER_BUILD_NO));
    AboutText.append(" , Built on ");
    AboutText.append(QString(APPIMAGE_UPDATER_BUILD_TIME));
    AboutText.append(".<br><br>");
    AboutText.append("The GNU Lesser General Public License v3.<br>");
    AboutText.append("Copyright &#169; 2018 , Antony Jr.<br><br>");
    AboutText.append("Icons by <a href=\"https://flaticon.com\">FlatIcon</a>.<br>");
    AboutText.append("Released under Creative Commons.<br><br>");
    AboutText.append("Report issues at <a href=\"https://github.com/antony-jr/AppImageUpdater\">github</a>.<br>");
    AboutText.append("This project is maintained by <a href=\"https://github.com/antony-jr\">Antony Jr</a>.");
    _pAboutMessageBox.setTextFormat(Qt::RichText);
    _pAboutMessageBox.setText(AboutText);
    _pAboutMessageBox.setIconPixmap(QPixmap(QString::fromUtf8(":/logo.png")).scaled( 128 , 128 , Qt::KeepAspectRatio));
    }


    /* Check for updates. */
    _bUpdateStarted = true;
    _pUpdateDialog = new AppImageUpdaterDialog(centerPos);
    _pUpdateDialog->setWindowIcon(_pWindowIcon);
    _pUpdateDialog->setShowLog(true);
    _pUpdateDialog->setIconPixmap(QPixmap(QString::fromUtf8(":/logo.png")));
    /* Special connect */
    connect(_pUpdateDialog , &AppImageUpdaterWidget::quit , this , &AppImageUpdater::quit , Qt::DirectConnection);

    _pUpdateDialog->init();

    /* Connect buttons. */
    connect(_pUi.exitBtn , &QPushButton::pressed , this , &AppImageUpdater::quit , Qt::DirectConnection);
    connect(_pUi.aboutBtn , &QPushButton::pressed , this , &AppImageUpdater::showAbout);
    
    /* Program logic. */
    connect(_pUpdateDialog , &AppImageUpdaterWidget::started , this , &AppImageUpdater::handleStarted);
    connect(_pUpdateDialog , &AppImageUpdaterWidget::canceled , this , &AppImageUpdater::handleCanceled);
    connect(_pUpdateDialog , &AppImageUpdaterWidget::error , this , &AppImageUpdater::handleError);
    connect(_pUpdateDialog , &AppImageUpdaterWidget::finished , this , &AppImageUpdater::handleFinished);
    return;
}

AppImageUpdater::~AppImageUpdater()
{

    return;
}

void AppImageUpdater::handleStarted(void)
{
	_bUpdateStarted = true;
	return;
}

void AppImageUpdater::handleCanceled(void)
{
	_bUpdateStarted = false;
	updateAppImagesInQueue();
	return;
}

void AppImageUpdater::handleError(QString eStr , short errorCode)
{
	_bUpdateStarted = false;
	QMessageBox box(this);
	box.setWindowTitle("Update Failed!");
	box.move(centerPos);
	box.resize(QSize(400 , 120));
	if(errorCode == AppImageUpdaterBridge::INVALID_MAGIC_BYTES){
	box.setText(QString::fromUtf8("The given file is not a valid AppImage file with respect to AppImage Specification!"));
	}else{
	box.setText(QString::fromUtf8("Update failed for the following reason , ") +
		    AppImageDeltaRevisioner::errorCodeToString(errorCode) + 
		    QString::fromUtf8("."));
	}
	box.setIcon(QMessageBox::Critical);
	box.exec();
	updateAppImagesInQueue();
	return;
}

void AppImageUpdater::handleFinished(QJsonObject info)
{
	auto r = (AppImageUpdaterDialog*)QObject::sender();
	(void)info;
	r->deleteLater();
	_bUpdateStarted = false;
	updateAppImagesInQueue();
	return;
}

/* Show hide window. */
void AppImageUpdater::showHideWindow(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		this->move(centerPos);
		if(this->isVisible()){
			this->hide();
  			_pTIcon->showMessage(QString::fromUtf8("Running in the Background!") , 
					     QString::fromUtf8("Click on the system tray icon to use AppImage Updater."));
   
		}else{
			this->show();
		}
	}
	return;
}

/* Show about dialog box. */
void AppImageUpdater::showAbout(void)
{
	_pAboutMessageBox.exec();
	return;
}

/* Updates AppImages in queue. */
void AppImageUpdater::updateAppImagesInQueue(void)
{
	if(_bUpdateStarted == true){
		return;
	}
	if(_pAppImagePaths.isEmpty()){
		_pTIcon->showMessage("All Updates Completed!" , "AppImageUpdater finished all queued updates.");
		return;
	}	

	_bUpdateStarted = true;
	_pCurrentAppImagePath = _pAppImagePaths.dequeue();

    	_pUpdateDialog = new AppImageUpdaterDialog(centerPos);
	_pUpdateDialog->setWindowIcon(_pWindowIcon);
    	_pUpdateDialog->setIconPixmap(QPixmap(QString::fromUtf8(":/default_icon.png")));
	_pUpdateDialog->setShowLog(true);
	_pUpdateDialog->setAppImage(_pCurrentAppImagePath);
	_pUpdateDialog->init();

    	/* Program logic. */
    	connect(_pUpdateDialog , &AppImageUpdaterWidget::started , this , &AppImageUpdater::handleStarted);
    	connect(_pUpdateDialog , &AppImageUpdaterWidget::canceled , this , &AppImageUpdater::handleCanceled);
    	connect(_pUpdateDialog , &AppImageUpdaterWidget::error , this , &AppImageUpdater::handleError);
    	connect(_pUpdateDialog , &AppImageUpdaterWidget::finished , this , &AppImageUpdater::handleFinished);
	return;
}

/* drop move event override. */
void AppImageUpdater::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
    /* Lets react to the user's drag. */
    AUI(dragAndDropArea).setPixmap(_pDropHere);
    return;
}

/* drop leave event override. */
void AppImageUpdater::dragLeaveEvent(QDragLeaveEvent *e)
{
    Q_UNUSED(e);
    /* Reset back to normal. */
    AUI(dragAndDropArea).setPixmap(_pDropNorm);
    return;
}

/* drop enter event override. */
void AppImageUpdater::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
    return;
}

/* handle drop event. */
void AppImageUpdater::dropEvent(QDropEvent *e)
{
    /* Reset back to normal. */
    AUI(dragAndDropArea).setPixmap(_pDropNorm);
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        _pAppImagePaths.enqueue(fileName);
	QString msg;
	msg.append(fileName);
	msg.append(" is queued for update.");
	_pTIcon->showMessage(QString::fromUtf8("AppImage Queued!") , msg);
	QCoreApplication::processEvents();
    }

    if(_bUpdateStarted != true){
	updateAppImagesInQueue(); 
    }
    return;
}
