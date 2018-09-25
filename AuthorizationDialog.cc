#include <AuthorizationDialog.hpp>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QDebug>
#include <QEventLoop>
#include <QMetaObject>
#include <QMetaMethod>

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

AuthorizationDialog::AuthorizationDialog(QWidget *parent)
    : QDialog(parent)
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

void AuthorizationDialog::handleAuthorization(QString errorString, short errorCode, QString appimagePath)
{
    QFuture<void> future = QtConcurrent::run([this, errorString, errorCode, appimagePath]() {
        doAuthorize(errorString, errorCode, appimagePath);
        return;
    });
    return;
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

void AuthorizationDialog::doAuthorize(QString errorString, short errorCode, QString appimagePath)
{
    emit started();
    arguments.clear();
    arguments << QString::fromUtf8("--standalone-update-dialog")
              << appimagePath;
    const QString fallback = program + QLatin1String(" ") + arguments.join(QLatin1String(" "));
    (_pUi.reasonLbl)->setText(QString::fromUtf8("Authorization is required for %1 because %2").arg(appimagePath, errorString));
    (_pUi.passwordTxt)->setText(QString());
    /*
     * As we cannot pipe the password to su in QProcess, we need to setup a pseudo-terminal for it
    */
    int masterFD = -1;
    int slaveFD = -1;
    char ptsn[ PATH_MAX ];
    auto metaObject = this->metaObject();
    QEventLoop loop;

    if (::openpty(&masterFD, &slaveFD, ptsn, 0, 0)) {
        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
        .invoke(this, Q_ARG(QString, QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error).")));

        emit finished(QJsonObject());
        return;
    }

    masterFD = ::posix_openpt(O_RDWR | O_NOCTTY);
    if (masterFD < 0) {
        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
        .invoke(this, Q_ARG(QString, QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error).")));


        emit finished(QJsonObject());
        return;
    }

    const QByteArray ttyName = ::ptsname(masterFD);

    if (::grantpt(masterFD)) {
        ::close(masterFD);
        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
        .invoke(this, Q_ARG(QString, QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error).")));


        emit finished(QJsonObject());
        return;
    }

    ::unlockpt(masterFD);

    slaveFD = ::open(ttyName, O_RDWR | O_NOCTTY);
    if (slaveFD < 0) {
        ::close(masterFD);
        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
        .invoke(this, Q_ARG(QString, QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error).")));


        emit finished(QJsonObject());
        return;
    }

    ::fcntl(masterFD, F_SETFD, FD_CLOEXEC);
    ::fcntl(slaveFD, F_SETFD, FD_CLOEXEC);
    int pipedData[2];
    if (pipe(pipedData) != 0) {
        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
        .invoke(this, Q_ARG(QString, QString::fromUtf8("we cannot setup a pseudo-terminal(Internal error).")));


        emit finished(QJsonObject());
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
        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
        .invoke(this, Q_ARG(QString, QString::fromUtf8("we cannot fork a child process(Internal error).")));


        emit finished(QJsonObject());
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
        int errBytes = 0;
        char buf[1024];
        char errBuf[1024];
        while (bytes >= 0) {
            int state;
            if (::waitpid(child, &state, WNOHANG) == -1)
                break;
            bytes = ::read(masterFD, buf, 1023);
            errBytes = ::read(pipedData[0], errBuf, 1023);
            if (errBytes > 0) {
                errData.append(buf, errBytes);
                errBytes=0;
            }
            if (bytes > 0) {
                const QString line = QString::fromLatin1(buf, bytes);
                if (re.indexIn(line) != -1) {
                    connect(this, &QDialog::accepted, &loop, &QEventLoop::quit);
                    connect(this, &QDialog::rejected, &loop, &QEventLoop::quit);
                    metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("exec(void)")))
                    .invoke(this);
                    loop.exec();

                    const QString password = (_pUi.passwordTxt)->text();
                    if (password.isEmpty() || this->result() != QDialog::Accepted) {
                        QByteArray pwd = password.toLatin1();
                        for (int i = 0; i < 3; ++i) {
                            ::write(masterFD, pwd.data(), pwd.length());
                            ::write(masterFD, "\n", 1);
                        }
                        metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
                        .invoke(this, Q_ARG(QString, QString::fromUtf8("you never gave any valid password.")));

                        emit finished(QJsonObject());
                        return;
                    }
                    QByteArray pwd = password.toLatin1();
                    ::write(masterFD, pwd.data(), pwd.length());
                    ::write(masterFD, "\n", 1);
                    ::read(masterFD, buf, pwd.length() + 1);
                }
            }
            if (bytes == 0)
                ::usleep(100000);
        }
        if (QString(errData).split(QString::fromUtf8("[sudo]")).count() >= 3) {
            metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("showError(QString)")))
            .invoke(this, Q_ARG(QString,
                                QString::fromUtf8("the password you gave has been rejected.")));

            emit finished(QJsonObject());
            return;
        }

        int status;
        child = ::wait(&status);
        const int exited = WIFEXITED(status);
        const int exitStatus = WEXITSTATUS(status);
        ::close(pipedData[1]);
        emit finished(QJsonObject());
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
        emit finished(QJsonObject());
        return;
    }

}
