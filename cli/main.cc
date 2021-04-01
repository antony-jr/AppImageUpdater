#include <QCoreApplication>
#include <QAppImageUpdate>
#include <cutelog.h>
#include <assert.h>
#include <QJsonObject>
#include <QFile>
#include <QTimer>
#include <QFileInfo>

#include <cstdio>
#include <signal.h>

#ifndef APPIMAGE_UPDATER_VERSION
#define APPIMAGE_UPDATER_VERSION "2"
#endif
#ifndef APPIMAGE_UPDATER_COMMIT
#define APPIMAGE_UPDATER_COMMIT "none"
#endif
#ifndef APPIMAGE_UPDATER_BUILD_TIME
#define APPIMAGE_UPDATER_BUILD_TIME "Unknown"
#endif
#ifndef APPIMAGE_UPDATER_BUILD_NO
#define APPIMAGE_UPDATER_BUILD_NO "1"
#endif

static void signal_handler(int);
void init_signals(void);

struct sigaction sigact;

bool updateRunning = false;
QAppImageUpdate *updater = nullptr; // This is made global for make it available for signal handlers

static const char *star_emoji = "🌟";
static const char *heart_emoji = "💖";

static void print_header();
static void print_usage(const char *);

int main(int ac, char **av){
	int r = 0;	
	init_signals();
	QCoreApplication app(ac, av);
	updater = new QAppImageUpdate(/*singleThreaded=*/false, &app);
	bool moveOldVersion = false,
	     deleteOldVersion = false,
	     debug = false,
	     avoidRunMsg = false,
	     checkForUpdateAndExit = false,
	     useTorrent = false;
	double megaBytesTotal = 0;
	QStringList appImagesToUpdate;
	QString currentOperatingAppImage;
	cutelog_t ctx = cutelog_new();
	assert(ctx != NULL);
	print_header();	
	if(ac == 1){
		print_usage(*av);
		return r;
	}

	bool appImageGiven = false;
        std::string progName(*av);
	++av; // Go past program name.
	while(*av){
		if(!qstrcmp(*av, "-t") ||
		   !qstrcmp(*av, "--use-torrent")){
			useTorrent = true;
		}else if(!qstrcmp(*av, "-j") ||
		   !qstrcmp(*av, "--check-for-update")){
			checkForUpdateAndExit = true;
		}else if(!qstrcmp(*av, "-m") ||
		   !qstrcmp(*av, "--move-old-version")){
			moveOldVersion = true;
		}else if(!qstrcmp(*av, "-D") ||
		   !qstrcmp(*av, "--delete-old-version")){
			deleteOldVersion = true;
		}else if(!qstrcmp(*av, "-d") ||
		   !qstrcmp(*av, "--debug")){
			debug = true;
		}else if(!qstrcmp(*av, "-v") ||
		   !qstrcmp(*av, "--version")){
			return r;
		}else if(!qstrcmp(*av, "-h") ||
		   !qstrcmp(*av, "--help")){
			print_usage(progName.c_str());
			return r;
		}else{
			// Append this to our list of
			// AppImage paths.
			appImagesToUpdate << QString::fromUtf8(*av);
			appImageGiven = true;
		}
		++av;	
	}
	if(!appImageGiven){
		print_usage(progName.c_str());
		return r;
	}

	QObject::connect(updater, &QAppImageUpdate::started, [&](short action){
		// Put variable initialization here.
		if(action == QAppImageUpdate::Action::CheckForUpdate) {
			cutelog_mode(ctx,cutelog_multiline_mode);
			cutelog_info(ctx,
		     	"[Running on] %s" ,
		      	currentOperatingAppImage.toStdString().c_str());
			return;
		}
		megaBytesTotal = 0;
		updateRunning = true;
		cutelog_mode(ctx, cutelog_multiline_mode);
		cutelog_info(ctx,
			     "[Started Update] %s" ,
			     currentOperatingAppImage.toStdString().c_str());
	});
	

	if(debug){	
		QObject::connect(updater, &QAppImageUpdate::logger,
		[&](QString msg, QString path) {
			
			cutelog_mode(ctx, cutelog_multiline_mode);
			cutelog_debug(ctx, "%s", msg.toStdString().c_str() + 9);
		});
	}

	QObject::connect(updater, &QAppImageUpdate::progress,
	[&](int percent, qint64 br, qint64 bt, double speed, QString unit, short action) {
		if(action == QAppImageUpdate::Action::CheckForUpdate) {
			cutelog_mode(ctx, cutelog_non_multiline_mode);
        		cutelog_progress(ctx, 
			"[%i%% Done] Downloading and parsing meta file.",
			percent);
			return;
		}
	        if(!br && !bt ){
			return;
		}
	        double MegaBytesReceived = br / 1048576;
    		if(!megaBytesTotal) {
			megaBytesTotal = bt / 1048576;
    		}
        	cutelog_mode(ctx, cutelog_non_multiline_mode);
        	cutelog_progress(ctx, 
		"[%i%% Done] %2.2f/%2.2f MiB at %2.2f %s.",
		percent, MegaBytesReceived, megaBytesTotal, speed, unit.toStdString().c_str());
        	return;
	});

	QObject::connect(updater, &QAppImageUpdate::error,
	[&](short errorCode, short action){
		updateRunning = false;
	        QString reason = QAppImageUpdate::errorCodeToDescriptionString(errorCode);
		cutelog_mode(ctx,cutelog_multiline_mode);
		cutelog_fatal(
			ctx, 
			"%s",
			reason.toStdString().c_str());

		
		if(appImagesToUpdate.isEmpty()){
			QCoreApplication::quit();
		}else{
			auto path = appImagesToUpdate.takeFirst();
			currentOperatingAppImage = QFileInfo(path).fileName();

			updater->setAppImage(path);
			updater->start(QAppImageUpdate::Action::CheckForUpdate);
		}
	});

	QObject::connect(updater, &QAppImageUpdate::finished, 
	[&](QJsonObject info, short action) {
		if(action == QAppImageUpdate::Action::CheckForUpdate) {
			if(!info["UpdateAvailable"].toBool()){
				cutelog_mode(ctx,cutelog_multiline_mode);
				cutelog_success(ctx, 
				"[No Update Needed] %s" ,
				currentOperatingAppImage.toStdString().c_str());
				if(appImagesToUpdate.isEmpty()){
					QCoreApplication::quit();
				}else{
					auto path = appImagesToUpdate.takeFirst();
					currentOperatingAppImage = QFileInfo(path).fileName();

					updater->setAppImage(path);
					updater->start(QAppImageUpdate::Action::CheckForUpdate);
				}
			 return;
			}
			cutelog_mode(ctx,cutelog_multiline_mode);		
			cutelog_info(ctx, 
			"[Update Needed] %s" ,
			currentOperatingAppImage.toStdString().c_str());
			if(checkForUpdateAndExit) {
				if(appImagesToUpdate.isEmpty()){
					QCoreApplication::quit();
				}else{
					auto path = appImagesToUpdate.takeFirst();
					currentOperatingAppImage = QFileInfo(path).fileName();

					updater->setAppImage(path);
					updater->start(QAppImageUpdate::Action::CheckForUpdate);	
				}
				return;
			}
			if(useTorrent) {
				updater->start(QAppImageUpdate::Action::UpdateWithTorrent);
			}else{
				updater->start(QAppImageUpdate::Action::Update);
			}
			return;
		}

		updateRunning = false;
	        QString newVer = info.value("NewVersionPath").toString();
	        cutelog_mode(ctx, cutelog_multiline_mode);
        	cutelog_success(ctx, "[Updated] %s", currentOperatingAppImage.toStdString().c_str());
        	cutelog_info(ctx, 
		"[New AppImage] %s", 
		newVer.toStdString().c_str());

		if(moveOldVersion){
			QString movePath = info.value("OldVersionPath").toString();
			movePath.append(".old-version");
			QFile::rename(info.value("OldVersionPath").toString(), movePath);
			cutelog_info(ctx, "[Moved] %s", 
				     movePath.toStdString().c_str());	
		}else if(deleteOldVersion){
			QFile::remove(info.value("OldVersionPath").toString());
			cutelog_info(ctx, "[Removed] %s", currentOperatingAppImage.toStdString().c_str());
		}

		
		if(appImagesToUpdate.isEmpty()){
			QCoreApplication::quit();
		}else{
			auto path = appImagesToUpdate.takeFirst();
			currentOperatingAppImage = QFileInfo(path).fileName();

			updater->setAppImage(path);
			updater->start(QAppImageUpdate::Action::CheckForUpdate);
		}
        	return;
	});

	QObject::connect(updater, &QAppImageUpdate::canceled,
	[&](){
		QCoreApplication::quit();
	});

	{
		auto path = appImagesToUpdate.takeFirst();
		currentOperatingAppImage = QFileInfo(path).fileName();

		updater->setAppImage(path);
		updater->start(QAppImageUpdate::Action::CheckForUpdate);
	}

	r = app.exec();
	cutelog_safe_finish(ctx);
	cutelog_free(ctx);
        
	printf(
	   "\nThank you for using AppImageUpdater %s,"
	   " if you find this project useful then please\n"
	   "consider to %s this project at https://github.com/antony-jr/AppImageUpdater\n",
	   heart_emoji,
	   star_emoji);
	return r;
}

void init_signals(void){
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, (struct sigaction *)NULL);
}

static void signal_handler(int sig){
    if (sig == SIGINT){
	    // This is important, 
	    // we need to cancel anything that is updating
	    // if we are running any update.
	    if(!updateRunning){
		    QCoreApplication::quit();
	    }else{
		    updater->cancel();
	    }
    }
}


static void print_header() {
    printf("AppImageUpdater (commit %s ), build %s built on %s\n",
	   APPIMAGE_UPDATER_COMMIT,
	   APPIMAGE_UPDATER_BUILD_NO,
	   APPIMAGE_UPDATER_BUILD_TIME
          );
    printf("Copyright (C) %s, Antony Jr.\n", __DATE__ + 7);
    putchar('\n');

}

static void print_usage(const char *prog) {
    printf(
        "Usage: %s [OPTIONS] [APPIMAGE(S)]\n\n"
        "OPTIONS:\n"
	"    -t,--use-torrent		Enable BitTorrent Usage for Decentralized Update.\n"
        "    -j,--check-for-update	Check for update and exit.\n"
	"    -m,--move-old-version      Move the old version with suffix .old-version\n"
        "    -D,--delete-old-version    Delete the old version after update.\n"
        "    -d,--debug                 Debug mode.\n"
        "    -v,--version               Show version.\n"
        "    -h,--help                  Show this help.\n\n"
        , prog);
}
