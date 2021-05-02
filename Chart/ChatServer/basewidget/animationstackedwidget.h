#ifndef MUSICANIMATIONSTACKEDWIDGET_H
#define MUSICANIMATIONSTACKEDWIDGET_H

#include <QPainter>
#include <QStackedWidget>
#include <QPropertyAnimation>

class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    enum AnimationType
    {
        TopToBottom,    ///Animation from top to bottom
        BottomToTop,    ///Animation from bottom to top
        LeftToRight,    ///Animation from left to right
        RightToLeft     ///Animation from right to left
    };

    explicit AnimationStackedWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~AnimationStackedWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void start(int index);
    /*!
     * Start to anmiation by index.
     */
    void setIndex(int previous, int current);
    /*!
     * Set previoud and current index.
     */
    void setLength(int length, AnimationType type);
    /*!
     * Set animation start and end pos distance and animation type.
     */

    void setDuration(int duration);
    /*!
     * Set animation duration.
     */
    int getDuration() const;
    /*!
     * Get animation duration.
     */

private Q_SLOTS:
    void valueChanged(const QVariant &value);
    /*!
     * Animation value changed.
     */
    void animationFinished();
    /*!
     * Animation finished.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */
    void renderPreviousWidget(QPainter &painter, QTransform &transform);
    /*!
     * Render previous widget.
     */
    void renderCurrentWidget(QPainter &painter, QTransform &transform);
    /*!
     * Render current widget.
     */

    bool m_isAnimating;
    float m_currentValue;
    int m_currentIndex, m_previousIndex;
    AnimationType m_type;
    QPropertyAnimation *m_animation;

};

#endif // MUSICANIMATIONSTACKEDWIDGET_H
