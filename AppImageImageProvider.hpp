#ifndef APPIMAGE_IMAGE_PROVIDER_HPP_INCLUDED
#define APPIMAGE_IMAGE_PROVIDER_HPP_INCLUDED
#include <QQuickImageProvider>
#include <QHash>
#include <QString>
#include <QByteArray>
class AppImageImageProvider : public QQuickImageProvider
{
    QHash<QString, QByteArray*> m_DB;
public:
    AppImageImageProvider();
    ~AppImageImageProvider();
    void addImage(const QString&, QByteArray*);
    QPixmap requestPixmap(const QString&, QSize*, const QSize&) override;
};

#endif
