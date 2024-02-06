#ifndef IMAGEPDFWRITER_H
#define IMAGEPDFWRITER_H

#include <QString>
#include <QPdfWriter>
#include <QPainter>
#include <QRect>
#include <QString>
#include <QGuiApplication>
#include <QPen>
#include <QPageSize>
#include <QPixmap>
#include <QVector>

#include <algorithm>

class ImagePDFWriter
{
public:
    ImagePDFWriter(const QString &filename, const QPageSize &page_size);
    void AddImage(const QPixmap &image_data);
    void AddImages(const QVector<QPixmap> &images_data);
private:
    QPdfWriter pdf_writer_;
    QPainter painter_;
};

#endif // IMAGEPDFWRITER_H
