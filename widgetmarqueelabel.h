/**
  This class represents the marquee-effect that is implemented in the metadata-jlabel
*/
#ifndef _WIDGETMARQUEELABEL_H_
#define _WIDGETMARQUEELABEL_H_

#include <QLabel>
#include <QTimer>

class WidgetMarqueeLabel : public QLabel
{
    Q_OBJECT

    //Member Functions
    public:
        enum Direction{LeftToRight,RightToLeft};
        WidgetMarqueeLabel(QWidget *parent = 0);
        ~WidgetMarqueeLabel();
        void show();
        void setAlignment(Qt::Alignment);
        int getSpeed();

    //Public Member Slots
    public slots:
        void setSpeed(int s);
        void setDirection(int d);

    //Member Functions
    protected:
        void paintEvent(QPaintEvent *evt);
        void resizeEvent(QResizeEvent *evt);
        void updateCoordinates();

    //Data Members
    private:
        int px;
        int py;
        QTimer timer;
        Qt::Alignment m_align;
        int speed;
        int direction;
        int fontPointSize;
        int textLength;

    //Private Member Slots
    private slots:
        void refreshLabel();
};
#endif /*_WIDGETMARQUEELABEL_H_*/
