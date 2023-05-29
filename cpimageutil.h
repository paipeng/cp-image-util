#ifndef CPIMAGEUTIL_H
#define CPIMAGEUTIL_H

#include "cp-image-util_global.h"
#include <QImage>
#include <QGraphicsEffect>

class CPIMAGEUTIL_EXPORT CPImageUtil
{
public:
    CPImageUtil();

    static QImage resizeQImage(const QImage& image, QSize size);
    static QString convertImageToBase64(const QImage& image);
    static QString convertPixmapToBase64(const QPixmap* pixmap);
    static QImage convertToGray(const QImage& image);
    static QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent);
    static QImage crop(const QImage& image, QVector<QPoint> cropPoints, float  expandFactor, int orientation);
    static QImage convertGrayLevels(const QImage& image, int grayLevel = 16);
    static int* calculateHistogram(const QImage& image);
};

#endif // CPIMAGEUTIL_H
