#include "AppImageImageProvider.hpp"

AppImageImageProvider::AppImageImageProvider()
               : QQuickImageProvider(QQuickImageProvider::Pixmap) { }
AppImageImageProvider::~AppImageImageProvider() {
	for(auto it = m_DB.begin(),
		 end = m_DB.end();
		 it != end;
		 ++it) {
		delete (*it);
	}
}


void AppImageImageProvider::addImage(const QString &id, QByteArray *data) {
	if(m_DB.contains(id)){
		/// Remove old one and add this.
		auto ptr = m_DB.take(id);
		delete ptr;
		m_DB[id] = data;
	}else{
		m_DB[id] = data; 
	}
}


QPixmap AppImageImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
       int width = 100;
       int height = 100;

       if (size)
          *size = QSize(width, height);
       QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                      requestedSize.height() > 0 ? requestedSize.height() : height);
       
       if(!m_DB.contains(id)){
	       return pixmap;
       }

       auto data = m_DB.value(id);
       pixmap.loadFromData(*data);
       return pixmap;
}
