#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED 
#include <QObject>
#include <QFileInfo>

class Helpers : public QObject
{
    Q_OBJECT
public:
  Q_INVOKABLE QString fileNameFromPath(const QString &filePath) const; 
  Q_INVOKABLE bool isMinimized() const;
  Q_INVOKABLE bool removeFile(const QString &filePath) const;
};
#endif /// HELPERS_HPP_INCLUDED
