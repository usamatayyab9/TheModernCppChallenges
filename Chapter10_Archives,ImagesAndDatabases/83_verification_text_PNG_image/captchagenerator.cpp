#include "captchagenerator.h"

CaptchaGenerator::CaptchaGenerator()
{

}

/**
 * @brief Generates a captcha image which contains a random text, random lines
 * and random gradient.
 * 
 * @return QImage - Captch image generated
 */
QImage CaptchaGenerator::operator()()
{
    auto captcha_image  = QImage(kCaptchaImageWidth, kCaptchaImageHeight, QImage::Format_RGB32);
    captcha_image.fill(Qt::darkGray);
    auto painter        = QPainter{ &captcha_image };

    /*!Gradient must be drawn first otherwise it will cover text and lines*/
    DrawGradient(captcha_image.width(), captcha_image.height(), captcha_image.rect(), painter);
    DrawRandomText(captcha_image.width(), captcha_image.height(), painter);
    DrawRandomLines(captcha_image.width(), captcha_image.height(),painter);

    return captcha_image;
}

QString CaptchaGenerator::GetRandomString(const int &length)
{
    const auto kAllowedCharacters = QString{ "abcdefghijklmnopqrstuvxywzABCDEFGHIJKLMNOPQRSTUVXYWZ0123456789" };
    auto seed_data                = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(device_));
    auto seq = std::seed_seq(std::begin(seed_data), std::end(seed_data));
    generator_.seed(seq);
    auto dist = std::uniform_int_distribution{ 0, kAllowedCharacters.size() - 1 };

    auto random_text = QString(length, QChar('\0'));
    for (auto &ch : random_text)
    {
        ch = kAllowedCharacters[dist(generator_)];
    }
    return random_text;
}

void CaptchaGenerator::DrawRandomText(const int &image_width, const int &image_height, QPainter &painter)
{
    const auto kText    = GetRandomString(kRandomTextSize);
    auto x_coord        = 20; // Starting position. i.e. position on x-aixs where first letter will drawn
    auto angle_dist     = std::uniform_int_distribution{ kMinAngleForText, kMaxAngleForText };
    painter.setFont(QFont{ kFontType, kFontSize });
    for (const auto &kLetter : kText)
    {
        painter.save();
        painter.translate(x_coord, image_height / 2);
        painter.rotate(angle_dist(generator_));
        painter.drawText(0, 0, QString{ kLetter });
        painter.restore();

        x_coord += image_width / kText.size();
    }
    qDebug() << "Text: " << kText;
}

void CaptchaGenerator::DrawRandomLines(const int &image_width, const int &image_height, QPainter &painter)
{
    auto x_coord_dist    = std::uniform_int_distribution{ 0, image_width  };
    auto y_coord_dist    = std::uniform_int_distribution{ 0, image_height };
    auto line_color_dist = std::uniform_int_distribution{ 0, 255 };
    auto pen             = QPen{};
    pen.setWidth(2);
    for (auto ii = 0; ii < kRandomLinesCount; ++ii)
    {
        const auto kPoint1 = QPoint{ x_coord_dist(generator_), y_coord_dist(generator_) };
        const auto kPoint2 = QPoint{ x_coord_dist(generator_), y_coord_dist(generator_) };
        //Generate randome RGB values
        pen.setColor(QColor{ line_color_dist(generator_), line_color_dist(generator_), line_color_dist(generator_) });
        painter.setPen(pen);
        painter.drawLine(kPoint1, kPoint2);
    }
}

void CaptchaGenerator::DrawGradient(const int &image_width, const int &image_height, const QRect &image_rect, QPainter &painter)
{
    auto linear_gradient = QLinearGradient{ 0, 0, static_cast<qreal>(image_width), static_cast<qreal>(image_height) };
    linear_gradient.setColorAt(0.0, Qt::magenta);
    linear_gradient.setColorAt(1.0, Qt::cyan);
    painter.fillRect(image_rect, linear_gradient);
}
