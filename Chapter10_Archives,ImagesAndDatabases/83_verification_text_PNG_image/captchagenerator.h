#ifndef CAPTCHAGENERATOR_H
#define CAPTCHAGENERATOR_H

#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QtMath>
#include <QLinearGradient>

#include <algorithm>
#include <random>
#include <functional>
#include <array>

inline constexpr auto kRandomTextSize       = 6;
inline constexpr auto kRandomLinesCount     = 12;
inline constexpr auto kCaptchaImageWidth    = 300;
inline constexpr auto kCaptchaImageHeight   = 100;
inline constexpr auto kMinAngleForText      = -45;
inline constexpr auto kMaxAngleForText      = 45;
inline constexpr auto kFontType             = "Arial";
inline constexpr auto kFontSize             = 20;

class CaptchaGenerator
{
public:
    CaptchaGenerator();
    QImage operator()();
private:
    QString GetRandomString(const int &length);
    void DrawRandomText(const int &image_width, const int &image_height, QPainter &painter);
    void DrawRandomLines(const int &image_width, const int &image_height, QPainter &painter);
    void DrawGradient(const int &image_width, const int &image_height, const QRect &image_rect, QPainter &painter);

    std::random_device device_{};
    std::mt19937 generator_{ device_() };
};

#endif // CAPTCHAGENERATOR_H
