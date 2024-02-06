#include "imagepdfwriter.h"


ImagePDFWriter::ImagePDFWriter(const QString &filename, const QPageSize &page_size) :
    pdf_writer_( filename )
{
    pdf_writer_.setPageSize(page_size);
}

void ImagePDFWriter::AddImage(const QPixmap &image_data)
{
    auto writer = QPdfWriter{"output.pdf"};
    writer.setPageSize(QPageSize(QPageSize::A4));
    auto painter = QPainter(&pdf_writer_);
    painter.drawPixmap(0, 0, image_data);
    writer.newPage();
    painter.end();

}

void ImagePDFWriter::AddImages(const QVector<QPixmap> &images_data)
{
    for (const auto &kImage : images_data)
    {
        AddImage(kImage);
    }
}
