#ifndef CPIMAGEUTIL_H
#define CPIMAGEUTIL_H

#include "cp-image-util_global.h"
#include <QImage>

class CPIMAGEUTIL_EXPORT CPImageUtil
{
public:
    CPImageUtil();

    static QImage resizeQImage(const QImage& image, QSize size);
    static QString convertImageToBase64(const QImage& image);
    static QString convertPixmapToBase64(const QPixmap* pixmap);
    static QImage convertToGray(const QImage& image);
};

#endif // CPIMAGEUTIL_H
