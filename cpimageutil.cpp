#include "cpimageutil.h"
#include <QDebug>
#include <QBuffer>
#include <QPixmap>


CPImageUtil::CPImageUtil()
{
}


QImage CPImageUtil::resizeQImage(const QImage& image, QSize size) {
    qDebug() << "resizeQImage: " << image.width() << "-" << image.height() << "  to size: " << size.width() << "-" << size.height();
    if (image.width() > 0 && image.height() > 0) {
        if (size.width()/size.height()) {
            // height
            qDebug() << " resize: " << size.height() * image.width()/image.height() << "-" << size.height();
            return image.scaled(size.height() * image.width()/image.height(), size.height(), Qt::KeepAspectRatio);
        } else {
            // width
            qDebug() << " resize: " << size.width() << "-" << image.height() *size.width()/ image.width();
            return image.scaled(size.width(), image.height() *size.width()/ image.width(), Qt::KeepAspectRatio);
        }
    } else {
        return QImage();
    }
}

QString CPImageUtil::convertPixmapToBase64(const QPixmap* pixmap) {
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    if (pixmap->width() > 0 && pixmap->height() > 0) {
        pixmap->save(&buffer, "jpeg");
        qDebug() << "buffer size:" << bytes.size();
        //person.setPhotoBase64("data:image/jpeg;base64," + QString(bytes.toBase64()));
        return QString("data:image/jpeg;base64," + QString(bytes.toBase64()));
    } else {
        return QString("");
    }
}

QString CPImageUtil::convertImageToBase64(const QImage& image) {
    QPixmap pixmap = QPixmap::fromImage(image);
    return convertPixmapToBase64(&pixmap);
}

QImage CPImageUtil::convertToGray(const QImage& image) {
    return image.convertToFormat(QImage::Format_Grayscale8);
}
