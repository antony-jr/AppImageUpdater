#include <AuthorizationDialog.hpp>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

#include <cstdlib>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/limits.h>
#include <pty.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SU_COMMAND "/usr/bin/sudo"
#define XDG_SU_COMMAND "/usr/bin/xdg-su"

AuthorizationDialog::AuthorizationDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowStaysOnTopHint)
{
    _pUi.setupUi(this);
    auto arguments = QCoreApplication::arguments();
    program = QFileInfo(arguments.at(0)).absolutePath() +
              QString::fromUtf8("/") +
              QFileInfo(arguments.at(0)).fileName();
}

AuthorizationDialog::~AuthorizationDialog()
{
}

void AuthorizationDialog::showError(QString eStr)
{
    QMessageBox box(this);
    box.setWindowTitle(QString::fromUtf8("Authorization Failed"));
    box.setIcon(QMessageBox::Critical);
    box.setText(QString::fromUtf8("Authorization failed because ") + eStr);
    box.exec();
    return;
}

void AuthorizationDialog::handleAuthorization(QString errorString, QString appimagePath)
{
    arguments.clear();

    // Also append all the program arguments to this
    auto prog_args = QCoreApplication::arguments();
    bool skipNext = true; // To skip the program name
    for(QString arg : prog_args){
	    if(skipNext){
		    skipNext = false;
		    continue;
	    }
	    if(arg == "--show-checking-for-update-dialog" || arg == "-c") {
		    continue;
	    }
	    if(arg == "--standalone-update-dialog" || arg == "-d"){
		    skipNext = true;
		    continue;
	    }
	    arguments << arg;
    } 
    arguments << QString::fromUtf8("--standalone-update-dialog")
              << appimagePath 
	      << QString::fromUtf8("--noconfirm");

    /// Check if xdg-su command is available then just use it.
    if(QFile::exists(XDG_SU_COMMAND)) {
	    QString commandString = program + " ";
	    for(const QString &arg : arguments) {
		    commandString += arg + " ";
	    }

	    QStringList commandArgs;
	    commandArgs << "-c";
	    commandArgs << commandString;
	    QProcess::startDetached(XDG_SU_COMMAND, commandArgs);
	    emit finished();
	    return;
    }

    const QString fallback = program + QLatin1String(" ") + arguments.join(QLatin1String(" "));
    (_pUi.reasonLbl)->setText(QString::fromUtf8("Authorization is required for %1 because %2").arg(appimagePath, errorString));
    (_pUi.passwordTxt)->setText(QString());
    /*
     * As we cannot pipe the password to sudo in QProcess, 
     * we need to setup a pseudo-terminal for it
    */
    int masterFD = -1;
    int slaveFD = -1;
    char ptsn[ PATH_MAX ];
                  
    if (::openpty(&masterFD, &slaveFD, ptsn, 0, 0)) {
        emit finished();
	return;
    }

    masterFD = ::posix_openpt(O_RDWR | O_NOCTTY);
    if (masterFD < 0) {
        showError(QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error)."));
        emit finished();
        return;
    }

    const QByteArray ttyName = ::ptsname(masterFD);

    if (::grantpt(masterFD)) {
        ::close(masterFD);
        showError(QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error)."));
        emit finished();
        return;
    }

    ::unlockpt(masterFD);

    slaveFD = ::open(ttyName, O_RDWR | O_NOCTTY);
    if (slaveFD < 0) {
        ::close(masterFD);
        showError(QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error)."));
        emit finished();
        return;
    }

    ::fcntl(masterFD, F_SETFD, FD_CLOEXEC);
    ::fcntl(slaveFD, F_SETFD, FD_CLOEXEC);
    int pipedData[2];
    if (pipe(pipedData) != 0) {
        showError(QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error)."));
        emit finished();
        return;
    }

    int flags = ::fcntl(pipedData[0], F_GETFD);
    if (flags != -1)
        ::fcntl(pipedData[0], F_SETFL, flags | O_NONBLOCK);

    pid_t child = fork();

    if (child < -1) {
        ::close(masterFD);
        ::close(slaveFD);
        ::close(pipedData[0]);
        ::close(pipedData[1]);
        showError(QString::fromUtf8("we cannot fork a child process(Internal error)."));
        emit finished();
        return;
    }

    // parent process
    else if (child > 0) {
        ::close(slaveFD);
        //close writing end of pipe
        ::close(pipedData[1]);

        QRegExp re(QLatin1String("[Pp]assword.*:"));
        QByteArray errData;
        flags = ::fcntl(masterFD, F_GETFD);
        int bytes = 0;
        char buf[1024];
        while (bytes >= 0) {
            int state;
            if (::waitpid(child, &state, WNOHANG) == -1)
                break;
            bytes = ::read(masterFD, buf, 1023);
            if (bytes > 0) {
                const QString line = QString::fromLatin1(buf, bytes);
		if (re.indexIn(line) != -1) {
                    this->exec();

		    const QString password = (_pUi.passwordTxt)->text();
                    if (this->result() != QDialog::Accepted) {
			    /* Note: 
			     *  Do not break here , if we do break here then on the end of this
			     *  loop , we wait for sudo to exit but it never does.
			     *  So we simply close and exit.
			    */  
			    ::close(pipedData[1]);
			    emit finished();
			    return;
		    }
                    QByteArray pwd = password.toUtf8();
                    ::write(masterFD, pwd.data(), pwd.length());
                    ::write(masterFD, "\n", 1);
                    ::read(masterFD, buf, pwd.length() + 1);
		}
            }
            if (bytes == 0)
                ::usleep(100000);
        }
        int status = 0;
        child = ::wait(&status);
        ::close(pipedData[1]);
        if (status) {
             showError(QString::fromUtf8("the password you gave has been rejected."));
        }
	emit finished();
        return;
    }

    // child process
    else {
        ::close(pipedData[0]);
        // Reset signal handlers
        for (int sig = 1; sig < NSIG; ++sig)
            signal(sig, SIG_DFL);
        signal(SIGHUP, SIG_IGN);

        ::setsid();

        ::ioctl(slaveFD, TIOCSCTTY, 1);
        int pgrp = ::getpid();
        ::tcsetpgrp(slaveFD, pgrp);

        ::dup2(slaveFD, 0);
        ::dup2(slaveFD, 1);
        ::dup2(pipedData[1], 2);

	// close all file descriptors
        struct rlimit rlp;
        getrlimit(RLIMIT_NOFILE, &rlp);
        for (int i = 3; i < static_cast<int>(rlp.rlim_cur); ++i)
            ::close(i);

        char **argp = (char **) ::malloc(arguments.count() + 4 * sizeof(char *));
        QList<QByteArray> args;
        args.push_back(SU_COMMAND);
        args.push_back("-b");
        args.push_back(program.toLocal8Bit());
        for (QStringList::const_iterator it = arguments.begin(); it != arguments.end(); ++it)
            args.push_back(it->toLocal8Bit());

        int i = 0;
        for (QList<QByteArray>::iterator it = args.begin(); it != args.end(); ++it, ++i)
            argp[i] = it->data();
        argp[i] = 0;

        ::unsetenv("LANG");
        ::unsetenv("LC_ALL");

        ::execv(SU_COMMAND, argp);
        _exit(0);
        emit finished();
        return;
    }

}
