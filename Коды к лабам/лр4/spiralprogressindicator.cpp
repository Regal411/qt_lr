#include "spiralprogressindicator.h"
#include <QFontMetrics>
#include <cmath>

SpiralProgressIndicator::SpiralProgressIndicator(QWidget *parent)
    : QWidget(parent),
      m_progressValue(0),
      m_maximumValue(100),
      m_minimumValue(0),
      m_targetValue(0),
      m_animationTimer(new QTimer(this))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_animationTimer, &QTimer::timeout, this, &SpiralProgressIndicator::updateAnimation);
}

void SpiralProgressIndicator::setProgressValue(int value)
{

    if (value < m_minimumValue)
        value = m_minimumValue;
    if (value > m_maximumValue)
        value = m_maximumValue;

    m_targetValue = value;

    if (m_progressValue != m_targetValue && !m_animationTimer->isActive()) {
        m_animationTimer->start(16);
    }
}

int SpiralProgressIndicator::progressValue() const
{
    return m_progressValue;
}

void SpiralProgressIndicator::setMaximumValue(int value)
{
    m_maximumValue = value;
    if (m_progressValue > m_maximumValue)
        setProgressValue(m_maximumValue);
    update();
}

int SpiralProgressIndicator::maximumValue() const
{
    return m_maximumValue;
}

void SpiralProgressIndicator::setMinimumValue(int value)
{
    m_minimumValue = value;
    if (m_progressValue < m_minimumValue)
        setProgressValue(m_minimumValue);
    update();
}

int SpiralProgressIndicator::minimumValue() const
{
    return m_minimumValue;
}

void SpiralProgressIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    int side = qMin(width(), height());
    QRectF outerRect(0, 0, side, side);
    outerRect.moveCenter(rect().center());


    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawEllipse(outerRect);


    double progress = static_cast<double>(m_progressValue - m_minimumValue) / (m_maximumValue - m_minimumValue);
    int maxLoops = 5;
    double maxRadius = side / 2.0;
    double minRadius = maxRadius * 0.3;

    QPainterPath spiralPath;
    spiralPath.moveTo(outerRect.center());
    for (int i = 0; i <= 360 * maxLoops * progress; ++i) {
        double angle = i * M_PI / 180.0;
        double t = i / (360.0 * maxLoops);
        double radius = minRadius + (maxRadius - minRadius) * t;
        double x = outerRect.center().x() + radius * cos(angle);
        double y = outerRect.center().y() + radius * sin(angle);
        spiralPath.lineTo(x, y);
    }

    painter.setPen(QPen(Qt::blue, 4));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(spiralPath);


    painter.setPen(Qt::white);
    QFont font = painter.font();
    int fontSize = static_cast<int>(side * 0.15);
    font.setPointSize(fontSize);
    painter.setFont(font);

    QString progressText = QString("%1%").arg(progress * 100, 0, 'f', 1);
    QRectF textRect = outerRect;
    textRect.adjust(10, 10, -10, -10);
    painter.drawText(textRect, Qt::AlignCenter, progressText);
}

void SpiralProgressIndicator::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y() / 120;
    setProgressValue(m_progressValue + delta);
}

void SpiralProgressIndicator::updateAnimation()
{

    if (m_progressValue < m_targetValue) {
        m_progressValue = qMin(m_progressValue + 1, m_targetValue);
    } else if (m_progressValue > m_targetValue) {
        m_progressValue = qMax(m_progressValue - 1, m_targetValue);
    }

    update();


    if (m_progressValue == m_targetValue) {
        m_animationTimer->stop();
    }
}
