#ifndef APPIMAGE_UPDATER_DIALOG_HPP_INCLUDED
#define APPIMAGE_UPDATER_DIALOG_HPP_INCLUDED
#include <AppImageUpdaterBridge>
#include <QMessageBox>

class AppImageUpdaterDialog : public AppImageUpdaterBridge::AppImageUpdaterWidget 
{
	Q_OBJECT
public:
	explicit AppImageUpdaterDialog(QPoint center , int idleSeconds = 0 , QWidget *parent = nullptr)
		: AppImageUpdaterWidget(idleSeconds , parent),
		  _pCenterPoint(center)
	{
		this->move(_pCenterPoint);
		return;
	}

	bool continueWithUpdate(QJsonObject info)
	{
		QMessageBox box(this);
		box.setText(QString::fromUtf8("A New version of ") + 
			    QFileInfo(info["AppImageFilePath"].toString()).baseName() + 
			    QString::fromUtf8(" is available , Do you want to update ?"));
		box.addButton(QMessageBox::Yes);
		box.addButton(QMessageBox::No);
		box.setIcon(QMessageBox::Question);
		box.setWindowTitle("Update Available!");
		box.resize(QSize(400 , 120));
		box.move(_pCenterPoint);
		return (box.exec() == QMessageBox::Yes);	
	}

	bool openNewVersion(QJsonObject info)
	{
		QMessageBox box(this);
		box.setText(QString::fromUtf8("Update completed successfully! The new version is saved at ") + 
			    info["AbsolutePath"].toString() + 
			    QString::fromUtf8(" , Do you want to open it ?"));
		box.addButton(QMessageBox::Yes);
		box.addButton(QMessageBox::No);
		box.setIcon(QMessageBox::Question);
		box.setWindowTitle("Update Complete!");
		box.resize(QSize(400 , 120));
		box.move(_pCenterPoint);
		return (box.exec() == QMessageBox::Yes);
	}
private:
	QPoint _pCenterPoint;
};

#endif // APPIMAGE_UPDATER_DIALOG_HPP_INCLUDED
