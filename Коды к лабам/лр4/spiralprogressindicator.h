#ifndef SPIRALPROGRESSINDICATOR_H
#define SPIRALPROGRESSINDICATOR_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QTimer>
#include <QPainterPath>

class SpiralProgressIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit SpiralProgressIndicator(QWidget *parent = nullptr);

    void setProgressValue(int value);
    int progressValue() const;

    void setMaximumValue(int value);
    int maximumValue() const;

    void setMinimumValue(int value);
    int minimumValue() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void updateAnimation();

private:
    int m_progressValue;
    int m_maximumValue;
    int m_minimumValue;
    int m_targetValue;
    QTimer *m_animationTimer;
};

#endif // SPIRALPROGRESSINDICATOR_H
