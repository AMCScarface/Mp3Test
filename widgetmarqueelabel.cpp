/**
    This class is resolving the marquee effect issue needed for the metadata-label
*/
#include "WidgetMarqueeLabel.h"
#include <QPainter>

/**
 * @brief WidgetMarqueeLabel::WidgetMarqueeLabel
 * Constructor of hte WidgetMarqueeLabel class
 * @param parent
 */
WidgetMarqueeLabel::WidgetMarqueeLabel(QWidget *parent)
{
    px = 0;
    py = 15;
    //Set the speed to 2;
    speed = 2;
    direction = RightToLeft;
    connect(&timer, SIGNAL(timeout()), this, SLOT(refreshLabel()));
    //Wait 100 ms to update the label
    timer.start(100);
}

/**
 * @brief WidgetMarqueeLabel::refreshLabel
 * Repaint the label every 100 ms
 */
void WidgetMarqueeLabel::refreshLabel()
{
    //Refresh:
    repaint();
}

/**
 * @brief WidgetMarqueeLabel::~WidgetMarqueeLabel
 * Destructor of the WidgetMarqueeLabel class
 */
WidgetMarqueeLabel::~WidgetMarqueeLabel()
{}

/**
 * @brief WidgetMarqueeLabel::show
 * Show the label
 */
void WidgetMarqueeLabel::show()
{
    QLabel::show();
}

/**
 * @brief WidgetMarqueeLabel::setAlignment
 * Set the allignment of the Text in the label
 * @param al
 * Type of allignment
 */
void WidgetMarqueeLabel::setAlignment(Qt::Alignment al)
{
    m_align = al;
    updateCoordinates();
    QLabel::setAlignment(al);
}

/**
 * @brief WidgetMarqueeLabel::paintEvent
 * Resolve the marquee effect through this event-method
 * @param evt
 */
void WidgetMarqueeLabel::paintEvent(QPaintEvent *evt)
{
    QPainter p(this);
    if(direction==RightToLeft)
    {
        px -= speed;
        if(px <= (-textLength))
            px = width();
    }
    else
    {
        px += speed;
        if(px >= width())
            px = - textLength;
    }
    p.drawText(px, py + fontPointSize, text());
    p.translate(px,0);
}

/**
 * @brief WidgetMarqueeLabel::resizeEvent
 * Update the label
 * @param evt
 * Resizevent
 */
void WidgetMarqueeLabel::resizeEvent(QResizeEvent *evt)
{
    updateCoordinates();
    QLabel::resizeEvent(evt);
}

/**
 * @brief WidgetMarqueeLabel::updateCoordinates
 * Update coordinates inside the label to realize marquee effect
 */
void WidgetMarqueeLabel::updateCoordinates()
{
    switch(m_align)
    {
        case Qt::AlignTop:
            py = 10;
            break;
        case Qt::AlignBottom:
            py = height()-10;
            break;
        case Qt::AlignVCenter:
            py = height()/2;
            break;
    }
    fontPointSize = font().pointSize()/2;
    textLength = fontMetrics().width(text());
}

/**
 * @brief WidgetMarqueeLabel::setSpeed
 * Set the flow speed of the text
 * @param s
 * speed of the flow
 */
void WidgetMarqueeLabel::setSpeed(int s)
{
    speed = s;
}

/**
 * @brief WidgetMarqueeLabel::getSpeed
 * Get the flow speed of the text
 * @return
 * the speed of the current flow
 */
int WidgetMarqueeLabel::getSpeed()
{
    return speed;
}

/**
 * @brief WidgetMarqueeLabel::setDirection
 * Set the direction of the text that flows inside the label
 * @param d
 * direction of the text
 */
void WidgetMarqueeLabel::setDirection(int d)
{
    direction = d;
    if (direction==RightToLeft)
        px = width() - textLength;
    else
        px = 0;
    refreshLabel();
}
