//-----本文件是对于widget.h和NewType.h中定义的新类型的成员函数的实现部分-----
#include "widget.h"

//重载图元类
void myPixmap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(fun==NULL || event->button()!=Qt::LeftButton)//检测图元是否是按钮、按下的是否是左键
    {return;}
    this->setPixmap(down);
}

void myPixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(fun==NULL)
    {return;}

    int x=this->x();
    int width=this->pixmap().width();
    int y=this->y();
    int height=this->pixmap().height();

    int xmin,ymin,xmax,ymax;
    xmin=x;
    xmax=x+width;

    if(x<0)
    {xmin=0;}
    if(x+width>WindowsWidth)
    {xmax=WindowsWidth;}

    ymin=y;
    ymax=y+height;

    if(y<0)
        ymin=0;
    if(y+height>WindowsHeigh)
        ymax=WindowsHeigh;

    if(xmax<s->mapFromGlobal(QCursor::pos()).x() || xmin>s->mapFromGlobal(QCursor::pos()).x())
    {
        this->setPixmap(up);
        return;
    }

    if(ymax<s->mapFromGlobal(QCursor::pos()).y() || ymin>s->mapFromGlobal(QCursor::pos()).y())
    {
        this->setPixmap(up);
        return;
    }

     this->setPixmap(up);
     QByteArray ba = fun.toLatin1();
     const char *function = ba.data();
     QMetaObject::invokeMethod(thob,function);
}

//视频类
VideoPlayer::VideoPlayer(QString Path,int Volume,int x,int y,int width,int heigh,bool cycle,QString signfun,QGraphicsScene *scene,QWidget *parent)
    : QWidget(parent)
{
    mediaPlayer=new QMediaPlayer(0,QMediaPlayer::VideoSurface);
    mediaPlayer->setVolume(Volume);
    videoItem=new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(width,heigh));
    scene->addItem(videoItem);
    mediaPlayer->setVideoOutput(videoItem);
    videoItem->setPos(x,y);//设置视频位置，默认为窗口左上角
    this->cycle=cycle;
    this->Path=Path;
    this->signfun=signfun;
}

void VideoPlayer::start()
{
mediaPlayer->setMedia(QUrl::fromLocalFile(Path));
mediaPlayer->play();
QObject::connect(mediaPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(playover(QMediaPlayer::State)));
}

void VideoPlayer::playover(QMediaPlayer::State state)
{
    if(state!=QMediaPlayer::StoppedState)
    {return;}
    if(!cycle)
    {
        if(signfun!=NULL)
        {
         QByteArray ba = signfun.toLatin1();
         const char *function = ba.data();
         QMetaObject::invokeMethod(lfevent,function);
        }
    delete this;
    }
    else
    {mediaPlayer->play();}
}

VideoPlayer::~VideoPlayer()
{
    delete mediaPlayer;
    delete videoItem;
}

//SC类
void SC::changepixmap()
{
    if(iter==pixmap.end())
    {
        if(cycle)
        {iter=pixmap.begin();}//若需循环，就重播
        else//若不循环，发信号退出一切
        {
            QList<QPair<int,SC *> > scPointer;
            QMutableListIterator<QPair<int,SC *> > p(scPointer);
            p.toFront();
            while(p.hasNext())
            {
                if(p.next().first==num)
                {
                    p.value().second->over=1;
                    break;
                }
            }
            if(signfun!=NULL)
            {
             QByteArray ba = signfun.toLatin1();
             const char *function = ba.data();
             QMetaObject::invokeMethod(lfevent,function);
            }
            return;
        }
    }
    //若未播完，正常换图
    pi->setPixmap(*iter);
    ++iter;
}

void SC::start(int choose)
{
    this->choose=choose;
    timer=new QTimer(this);
    over=0;
    switch(choose)
    {
    case 1:
    {
        temp=0;
        temp1=2*TargetModulus/times;
        timer->start(2);
        break;
    }
    case 2:
    {
        temp=0;
        temp1=2*(TargetModulus-CurrentModulus)/times;
        timer->start(2);
        break;
    }
    case 3:
    {
        temp=0;
        temp1=2*TargetModulus/times;
        temp2=2*TargetModulus2/times;
        timer->start(2);
        break;
    }
    case 4:
    {
        temp=0;
        temp1=2*TargetModulus/times;
        timer->start(3);
        break;
    }
    case 5:
    {
        temp=0;
        temp1=2*(TargetModulus-CurrentModulus)/times;
        timer->start(2);
        break;
    }
    case 6:
    {
        temp=0;
        temp1=2*(TargetModulus-CurrentModulus)/times;
        temp2=2*(TargetModulus2-CurrentModulus2)/times;
        temp3=2*(TargetModulus3-CurrentModulus3)/times;
        timer->start(2);
        break;
    }

    case 7:
    {
        iter=pixmap.begin();//将装满连帧图元的迭代器调到头元素
        temp1=times/pixmap.size();//计算出播放每一帧需要的时间
        timer->start(int(temp1));//根据此时间开始播放
    }

    }
    connect(timer,SIGNAL(timeout()),this,SLOT(SlowChange()));
}

void SC::SlowChange()
{
    if(!over)
    {
        switch(choose)
        {
        case 1:
        {
            temp+=2;
            gr->setRotation(CurrentModulus+=temp1);
            gr->rotation();
            if(temp==times || temp==times+1 || temp==times-1)
                over=1;
            break;
        }
        case 2:
        {
            temp+=2;
            gr->setScale(CurrentModulus+=temp1);
            gr->scale();
            if(temp==times || temp==times+1 || temp==times-1)
                over=1;
           break;
        }
        case 3:
        {
            temp+=2;
            gr->moveBy(temp1,temp2);
            if(temp==times || temp==times+1 || temp==times-1)
                over=1;
            break;
        }
        case 4:
        {
            temp+=3;
            Effect->setBlurRadius(CurrentModulus+=temp1);
            gr->setGraphicsEffect(Effect);
            if(temp==times || temp==times+1 || temp==times-1 || temp==times+2 || temp==times-2)
                over=1;
            break;
        }
        case 5:
        {
            temp+=2;
            //SetOpacityItem(CurrentModulus+=temp1);
            gr->setOpacity(CurrentModulus+=temp1);
            if(temp==times || temp==times+1 || temp==times-1)
                over=1;
            break;
        }
        case 6:
        {
            temp+=2;
            co->setColor(QColor(CurrentModulus+=temp1,CurrentModulus2+=temp2,CurrentModulus3+=temp3));
            gr->setGraphicsEffect(co);
            if(temp==times || temp==times+1 || temp==times-1)
                over=1;
            break;
        }
        case 7:
        {
            changepixmap();
            break;
        }
        }
    }
    if(over)
    {
        if(signfun!=NULL)
        {
         QByteArray ba = signfun.toLatin1();
         const char *function = ba.data();
         QMetaObject::invokeMethod(lfevent,function);
        }
        p.toFront();
        while(p.hasNext())
        {
            if(p.next().second==this)
            {
                p.remove();
                break;
            }
        }
        delete timer;
        delete this;
    }
}

//视图类
void graphicsview::Scale(float sx,float sy)
{scale(sx,sy);}

void graphicsview::Rotate(float set)
{rotate(set);}

//线程类
void easythread::run()
{
    QByteArray ba = fun.toLatin1();
    const char *function = ba.data();
    QMetaObject::invokeMethod(thob,function,Qt::DirectConnection);
}
