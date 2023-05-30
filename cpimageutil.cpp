#include "cpimageutil.h"
#include <QDebug>
#include <QBuffer>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

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



QImage CPImageUtil::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent) {
    if(src.isNull()) return QImage();   //No need to do anything else!
    if(!effect) return src;             //No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), src.format());
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) );
    return res;
}


QImage CPImageUtil::crop(const QImage& image, QVector<QPoint> cropPoints, float  expandFactor, int orientation) {
    QRect rect(cropPoints.at(0).x(), cropPoints.at(0).y(), cropPoints.at(1).x() - cropPoints.at(0).x(), cropPoints.at(2).y() - cropPoints.at(1).y());

    if (expandFactor != 1) {
        int crop_width = rect.width();
        int crop_height = rect.height();

        int offset_x = (expandFactor * crop_width - crop_width)/2;
        int offset_y = (expandFactor * crop_height - crop_height)/2;

        rect.setLeft(rect.left() - offset_x);
        rect.setTop(rect.top() - offset_y);

        rect.setWidth(expandFactor * crop_width);
        rect.setHeight(rect.width());//23*crop_height/21);
    }
    qDebug() << "crop rect: " << rect;

    QImage cropImage = image.copy(rect);

    // rotate
    if (orientation != 0) {
        return cropImage.transformed(QTransform().rotate(orientation));
    } else {
        return cropImage;
    }
}



QImage CPImageUtil::convertGrayLevels(const QImage& image, int grayLevel) {
    qDebug() << "convertGrayLevels: " << grayLevel << " image size: " << image.width() << "-" << image.height();

    int width = (image.width() % 4 != 0)?(image.width() /4 * 4 +4): image.width();

    uchar* data = (uchar*) malloc(sizeof(uchar) * width * image.height());
    memset(data, 255, sizeof (uchar) * width * image.height());
    //const uchar *bits = image.bits();
    for (int i = 0; i < image.height(); i++) {
        for (int j = 0; j < image.width(); j++) {
            QRgb rgb =  image.pixel(j ,i);
            //QColor color(rgb);
            // Luminosity Method
            int pixel =  ((int)((float)qRed(rgb) * 0.3 + (float)qGreen(rgb) * 0.59 + (float)qBlue(rgb) * 0.11)) / grayLevel * grayLevel;
            //qDebug() << pixel;
            data[i*width+j] = pixel > 255 ? 255:pixel;
        }
    }
    QImage grayImage = createGrayImage(data, image.width(), image.height(), width);
    free(data);
    //grayImage.save("/Users/paipeng/passfoto.bmp", "bmp");

    qDebug() << "convertGrayLevels done";
    return grayImage;
}

int* CPImageUtil::calculateHistogram(const QImage& image) {
    int *histogram = (int*) malloc(sizeof (int)*256);
    memset(histogram, 0, sizeof (int) * 256);

    for (int i = 0; i < image.height(); i++) {
        for (int j = 0; j < image.width(); j++) {
            QRgb rgb =  image.pixel(j ,i);
            //QColor color(rgb);
            // Luminosity Method
            int pixel = qGray(rgb);
            if (pixel > 255) {
                pixel = 255;
            } else if (pixel < 0) {
                pixel = 0;
            }
            histogram[pixel] ++;
        }
    }
    return histogram;
}

QImage CPImageUtil::createGrayImage(const uchar* data, int width, int height, int bitsperline) {
    QImage image = QImage(data, width, height, bitsperline, QImage::Format_Grayscale8);
    memcpy(image.bits(), data, sizeof(uchar)*image.width()*image.height());
    return image;
}
