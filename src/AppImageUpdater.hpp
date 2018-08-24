#ifndef APPIMAGEUPDATER_HPP_INCLUDED
#define APPIMAGEUPDATER_HPP_INCLUDED
#define APPIMAGEUPDATER_VERSION_STR VERSION

#include <QtCore>
#include <QtGlobal>
#include <QMovie>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QQueue>
#include <QMessageBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <AIUpdaterBridge.hpp>

QT_BEGIN_NAMESPACE

class Ui_AppImageUpdater
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGridLayout *MainGrid;
    QHBoxLayout *StaticControls;
    QPushButton *mainBtn;
    QSpacerItem *StatciControlsSpace;
    QPushButton *exitOrCancelBtn;
    QStackedWidget *MainStack;
    QWidget *Home;
    QGridLayout *gridLayout_3;
    QGridLayout *HomeGrid;
    QLabel *AppCopyleft;
    QLabel *VersionInfo;
    QLabel *HomeLogo;
    QLabel *AppInfo;
    QWidget *Loading;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_2;
    QLabel *LoadingGif;
    QWidget *Updating;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QLabel *AppImagePath;
    QProgressBar *progress;
    QLabel *progressText;
    QLabel *banner;
    QLabel *AppImageLogo;
    QWidget *InstallSuccess;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_7;
    QLabel *sckFoot;
    QLabel *sckImg;
    QLabel *sckHead;
    QWidget *InstallFail;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_9;
    QLabel *failFoot;
    QLabel *failImg;
    QLabel *failHead;
    QStatusBar *statusbar;
    QMovie* loader_movie;

    void setupUi(QMainWindow *AppImageUpdater)
    {
        if (AppImageUpdater->objectName().isEmpty())
            AppImageUpdater->setObjectName(QStringLiteral("AppImageUpdater"));
        AppImageUpdater->resize(807, 491);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/logo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        AppImageUpdater->setWindowIcon(icon);
        AppImageUpdater->setAutoFillBackground(false);
        AppImageUpdater->setStyleSheet(QLatin1String("background-color: #141318;\n"
                                       "color: white;"));
        loader_movie = new QMovie(":/resources/loader.gif");
        centralwidget = new QWidget(AppImageUpdater);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        MainGrid = new QGridLayout();
        MainGrid->setObjectName(QStringLiteral("MainGrid"));
        StaticControls = new QHBoxLayout();
        StaticControls->setObjectName(QStringLiteral("StaticControls"));
        mainBtn = new QPushButton(centralwidget);
        mainBtn->setObjectName(QStringLiteral("mainBtn"));
        mainBtn->setAutoFillBackground(false);
        mainBtn->setStyleSheet(QLatin1String("background-color: #709cb7;\n"
                                             "color: white;"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resources/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainBtn->setIcon(icon1);
        mainBtn->setFlat(false);

        StaticControls->addWidget(mainBtn);

        StatciControlsSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        StaticControls->addItem(StatciControlsSpace);

        exitOrCancelBtn = new QPushButton(centralwidget);
        exitOrCancelBtn->setObjectName(QStringLiteral("exitOrCancelBtn"));
        exitOrCancelBtn->setAutoFillBackground(false);
        exitOrCancelBtn->setStyleSheet(QLatin1String("background-color: #709cb7;\n"
                                       "color: white;"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/resources/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitOrCancelBtn->setIcon(icon2);
        exitOrCancelBtn->setFlat(false);

        StaticControls->addWidget(exitOrCancelBtn);


        MainGrid->addLayout(StaticControls, 1, 0, 1, 1);

        MainStack = new QStackedWidget(centralwidget);
        MainStack->setObjectName(QStringLiteral("MainStack"));
        Home = new QWidget();
        Home->setObjectName(QStringLiteral("Home"));
        gridLayout_3 = new QGridLayout(Home);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        HomeGrid = new QGridLayout();
        HomeGrid->setObjectName(QStringLiteral("HomeGrid"));

        VersionInfo = new QLabel(Home);
        VersionInfo->setObjectName(QStringLiteral("VersionInfo"));
        VersionInfo->setAlignment(Qt::AlignCenter);

        HomeGrid->addWidget(VersionInfo, 9, 0, 1, 3);

        HomeLogo = new QLabel(Home);
        HomeLogo->setObjectName(QStringLiteral("HomeLogo"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(HomeLogo->sizePolicy().hasHeightForWidth());
        HomeLogo->setSizePolicy(sizePolicy);
        HomeLogo->setMinimumSize(QSize(200, 200));
        HomeLogo->setMaximumSize(QSize(200, 200));
        HomeLogo->setPixmap(QPixmap(QString::fromUtf8(":/resources/enlarged_logo.png")));
        HomeLogo->setScaledContents(true);

        HomeGrid->addWidget(HomeLogo, 3, 1, 1, 1);

        AppInfo = new QLabel(Home);
        AppInfo->setObjectName(QStringLiteral("AppInfo"));
        AppInfo->setMaximumSize(QSize(600, 80));
        QFont font;
        font.setPointSize(10);
        AppInfo->setFont(font);
        AppInfo->setScaledContents(false);
        AppInfo->setAlignment(Qt::AlignCenter);

        HomeGrid->addWidget(AppInfo, 5, 0, 1, 3, Qt::AlignHCenter);

        gridLayout_3->addLayout(HomeGrid, 0, 1, 1, 1);

        MainStack->addWidget(Home);
        Loading = new QWidget();
        Loading->setObjectName(QStringLiteral("Loading"));
        gridLayout_4 = new QGridLayout(Loading);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        LoadingGif = new QLabel(Loading);
        LoadingGif->setObjectName(QStringLiteral("LoadingGif"));
        LoadingGif->setMinimumSize(QSize(256, 256));
        LoadingGif->setMaximumSize(QSize(256, 256));
        LoadingGif->setScaledContents(true);
        LoadingGif->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(LoadingGif, 0, 0, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);


        gridLayout_4->addLayout(gridLayout_2, 0, 0, 1, 1);

        MainStack->addWidget(Loading);
        Updating = new QWidget();
        Updating->setObjectName(QStringLiteral("Updating"));
        gridLayout_6 = new QGridLayout(Updating);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        AppImagePath = new QLabel(Updating);
        AppImagePath->setObjectName(QStringLiteral("AppImagePath"));
        AppImagePath->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        AppImagePath->setWordWrap(true);

        gridLayout_5->addWidget(AppImagePath, 0, 1, 1, 4);

        progress = new QProgressBar(Updating);
        progress->setObjectName(QStringLiteral("progress"));
        progress->setValue(0);

        gridLayout_5->addWidget(progress, 1, 1, 1, 4);


        AppImageLogo = new QLabel(Updating);
        AppImageLogo->setObjectName(QStringLiteral("AppImageLogo"));
        AppImageLogo->setMinimumSize(QSize(180, 180));
        AppImageLogo->setMaximumSize(QSize(180, 180));
        AppImageLogo->setPixmap(QPixmap(QString::fromUtf8(":/resources/enlarged_logo.png")));
        AppImageLogo->setScaledContents(true);

        gridLayout_5->addWidget(AppImageLogo, 0, 4, 1, 1);

        progressText = new QLabel(Updating);
        progressText->setObjectName(QStringLiteral("progressText"));
        progressText->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(progressText, 2, 1, 1, 4, Qt::AlignHCenter);

        banner = new QLabel(Updating);
        banner->setObjectName(QStringLiteral("banner"));
        banner->setMinimumSize(QSize(0, 0));
        banner->setMaximumSize(QSize(779, 168));
        banner->setPixmap(QPixmap(QString::fromUtf8(":/resources/thankyou.png")));
        banner->setScaledContents(false);
        banner->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(banner, 3, 1, 1, 4, Qt::AlignHCenter);

        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        MainStack->addWidget(Updating);
        InstallSuccess = new QWidget();
        InstallSuccess->setObjectName(QStringLiteral("InstallSuccess"));
        gridLayout_8 = new QGridLayout(InstallSuccess);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        sckFoot = new QLabel(InstallSuccess);
        sckFoot->setObjectName(QStringLiteral("sckFoot"));
        sckFoot->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(sckFoot, 3, 0, 1, 1);

        sckImg = new QLabel(InstallSuccess);
        sckImg->setObjectName(QStringLiteral("sckImg"));
        sckImg->setMinimumSize(QSize(312, 312));
        sckImg->setMaximumSize(QSize(312, 312));
        sckImg->setPixmap(QPixmap(QString::fromUtf8(":/resources/success.png")));
        sckImg->setScaledContents(true);

        gridLayout_7->addWidget(sckImg, 2, 0, 1, 1, Qt::AlignHCenter);

        sckHead = new QLabel(InstallSuccess);
        sckHead->setObjectName(QStringLiteral("sckHead"));
        sckHead->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(sckHead, 1, 0, 1, 1);


        gridLayout_8->addLayout(gridLayout_7, 0, 0, 1, 1);

        MainStack->addWidget(InstallSuccess);
        InstallFail = new QWidget();
        InstallFail->setObjectName(QStringLiteral("InstallFail"));
        gridLayout_10 = new QGridLayout(InstallFail);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        failFoot = new QLabel(InstallFail);
        failFoot->setObjectName(QStringLiteral("failFoot"));
        failFoot->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(failFoot, 3, 0, 1, 1);

        failImg = new QLabel(InstallFail);
        failImg->setObjectName(QStringLiteral("failImg"));
        failImg->setMinimumSize(QSize(312, 312));
        failImg->setMaximumSize(QSize(312, 312));
        failImg->setPixmap(QPixmap(QString::fromUtf8(":/resources/exit.png")));
        failImg->setScaledContents(true);

        gridLayout_9->addWidget(failImg, 2, 0, 1, 1, Qt::AlignHCenter);

        failHead = new QLabel(InstallFail);
        failHead->setObjectName(QStringLiteral("failHead"));
        failHead->setAlignment(Qt::AlignCenter);

        gridLayout_9->addWidget(failHead, 1, 0, 1, 1);


        gridLayout_10->addLayout(gridLayout_9, 0, 0, 1, 1);

        MainStack->addWidget(InstallFail);

        MainGrid->addWidget(MainStack, 0, 0, 1, 1);


        gridLayout->addLayout(MainGrid, 0, 0, 1, 1);

        AppImageUpdater->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(AppImageUpdater);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        AppImageUpdater->setStatusBar(statusbar);

        retranslateUi(AppImageUpdater);

        MainStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AppImageUpdater);
    } // setupUi

    void retranslateUi(QMainWindow *AppImageUpdater)
    {
        AppImageUpdater->setWindowTitle(QApplication::translate("AppImageUpdater", "AppImage Updater", nullptr));
#ifndef QT_NO_TOOLTIP
        AppImageUpdater->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        mainBtn->setText(QApplication::translate("AppImageUpdater", "Updater", nullptr));
        mainBtn->setEnabled(false);
        exitOrCancelBtn->setText(QApplication::translate("AppImageUpdater", "Exit", nullptr));
        LoadingGif->setText(QString());
        LoadingGif->setMovie(loader_movie);
        VersionInfo->setText(QString());
        HomeLogo->setText(QString());
        AppInfo->setText(QString());
        AppImagePath->setText(QString());
        AppImageLogo->setText(QString());
        sckFoot->setText(QString());
        sckImg->setText(QString());
        sckHead->setText(QApplication::translate("AppImageUpdater", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">Successfully Updated your AppImage! See ya!</span></p></body></html>", nullptr));
        failFoot->setText(QString());
        failImg->setText(QString());
        failHead->setText(QApplication::translate("AppImageUpdater", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">Failed to Update your AppImage, Sorry.</span></p></body></html>", nullptr));
    } // retranslateUi

};

// QSslSocket::sslLibraryBuildVersionString() -> to get Openssl build version.
// QT_VERSION_STR -> to get qt version in string.

namespace Ui
{
class AppImageUpdater: public Ui_AppImageUpdater {};
} // namespace Ui

class AppImageUpdater : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppImageUpdater();
    // explicit AppImageUpdater(const QString& AppImage);
    ~AppImageUpdater()
    {
        return;
    }
public slots:
    void checkForUpdates(void);
    void checkForUpdates(const QString&);
private slots:
    void progress(float, qint64, qint64, double, QString);
    void updateFinished(QString, QString);
    void noUpdatesAvailable(QString, QString);
    void updatesAvailable(QString, QString);
    void handleError(QString, short);
    void cancel(void);
    void exit(void);
signals:
    void safeToCloseApplication();
private:
    /*
     * Indexes for Stack
    */
    enum {
        HOME,
        LOADING,
        UPDATING,
        SUCCESS,
        FAILED
    };
    bool selfUpdate = false;
    Ui::AppImageUpdater *Ui;
    AIUpdaterBridge *Bridge;
    QQueue<QString> AppImages;
protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
};

QT_END_NAMESPACE

#endif // APPIMAGEUPDATER_HPP_INCLUDED
