#include <AppImageUpdater.hpp>

using namespace AppImageUpdaterBridge;

/* DR => Dereference pointers. */
#define DR(pointer) (*pointer)

/* Access UserInterface. */
#define AUI(Object) DR(_pUi.Object)

/* AppImage Delta Revisioner Signals. */
#define ADRUA_SIGNAL &AppImageDeltaRevisioner::updateAvailable

/* Control Indices in the main widget. */
#define GOTO_INPUT_PAGE() AUI(mainStackedWidget).setCurrentIndex(INPUT);
#define GOTO_CHECKING_PAGE() AUI(mainStackedWidget).setCurrentIndex(CHECKING);
#define GOTO_UPDATING_PAGE() AUI(mainStackedWidget).setCurrentIndex(UPDATING);
#define GOTO_RESULT_PAGE() AUI(mainStackedWidget).setCurrentIndex(RESULT);

AppImageUpdater::AppImageUpdater()
	: QWidget()
{
    _pUi.setupUi(this);
    setAcceptDrops(true);
    /* Customize the looks. */
    AUI(mainStackedWidget).setCurrentIndex(INPUT);

    /* This is oftenly used pixmap , Thus to reduce overhead , We are caching it. */
    _pDropHere = QPixmap(QString::fromUtf8(":/dotted_square_drop.png"));
    _pDropNorm = QPixmap(QString::fromUtf8(":/dotted_square.png"));

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


    /*
     * Check for new version of AppImageUpdater in the background but 
     * don't really do the update unless or until the user asks for it.
    */
    _pSelfRevisioner = new AppImageDeltaRevisioner(/*single threaded=*/false ,/*parent=*/this);
    /* Avoid errors and everything , only do something if we have a new version. */
    connect(_pSelfRevisioner , ADRUA_SIGNAL , this , &AppImageUpdater::selfUpdateAvailable);
    _pSelfRevisioner->checkForUpdate(); // check it as soon as possible.

    /*
     * This is the main revisioner used to check update for the given
     * AppImages.
     * We are running this also on a new thread since we don't want to
     * disturb the main gui thread whatsoever.
    */
    _pAppImageRevisioner  = new AppImageDeltaRevisioner(false , this);

    /* Connect buttons. */
    connect(_pUi.exitBtn , &QPushButton::pressed , this , &AppImageUpdater::quit , Qt::DirectConnection);
    connect(_pUi.aboutBtn , &QPushButton::pressed , this , &AppImageUpdater::showAbout);
    return;
}

AppImageUpdater::~AppImageUpdater()
{

    return;
}

/* Show about dialog box. */
void AppImageUpdater::showAbout(void)
{
	_pAboutMessageBox.exec();
	return;
}

/* Handle errors. */
void AppImageUpdater::handleError(short errorCode)
{
	return;
}

/* Updates AppImages in queue. */
void AppImageUpdater::updateAppImagesInQueue(void)
{
	if(_pAppImagePaths.isEmpty())
		return;

	// setAcceptDrops(false); // Disable any further drops since we started update.
	_pCurrentAppImagePath = _pAppImagePaths.dequeue();

	_pAppImageRevisioner->setAppImage(_pCurrentAppImagePath);
	_pAppImageRevisioner->setShowLog(true);

		
	GOTO_CHECKING_PAGE();
	_pAppImageRevisioner->start();
	return;
}

/* Handles update check result for the current AppImage, Aka self update check. */
void AppImageUpdater::selfUpdateAvailable(bool updateIsAvailable , QString path)
{
	Q_UNUSED(path);
	if(updateIsAvailable){
		qInfo().noquote() << "A New Version of AppImage Updater is Available!\n"
			          << "NOT IMPLEMENTED YET.";
		/* yet to implement. */
	}
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
	QCoreApplication::processEvents();
    }
    updateAppImagesInQueue(); 
    return;
}
