//-----本文件所声明用于引擎的其它类型-----
#pragma once
#include "head.h"
#include "NewType.h"

#define SynchronousStart(fun) {QTimer t;\
                    QEventLoop q;\
                    t.setSingleShot(true);\
                    connect(lfevent,SIGNAL( fun ()),&q,SLOT(quit()));
#define SynchronousFinish() t.start();q.exec();}

class ParametersStru;

class Widget;
class Item;
class SC : public QObject//渐变使用的工具类
{
    Q_OBJECT
public:
    SC(float CurrentModulus,float TargetModulus,int times)
        :CurrentModulus(CurrentModulus),TargetModulus(TargetModulus),times(times){}//这个构造方法处理单渐变系数
    SC(float CurrentModulus,float CurrentModulus2,float TargetModulus,float TargetModulus2,
       int times)
        :CurrentModulus(CurrentModulus),CurrentModulus2(CurrentModulus2),
        TargetModulus(TargetModulus),TargetModulus2(TargetModulus2),times(times){}//这个构造方法处理双渐变系数
    SC(float CurrentModulus,float CurrentModulus2,float CurrentModulus3,float TargetModulus,
       float TargetModulus2,float TargetModulus3,int times)
        :CurrentModulus(CurrentModulus),
        CurrentModulus2(CurrentModulus2),CurrentModulus3(CurrentModulus3),TargetModulus(TargetModulus),
        TargetModulus2(TargetModulus2),TargetModulus3(TargetModulus3),times(times){}//这个构造方法处理三渐变系数

    QGraphicsItem* gr;
    QGraphicsBlurEffect *Effect;
    QGraphicsColorizeEffect *co;
    QTransform *tf;
    QGraphicsPixmapItem *pi;
    QVector<QPixmap> pixmap;
    QString signfun;
    ParametersStru *par;
    int over;//结束标志
    void start(int choose);
    bool cycle;//连续播图是否循环播放
    Item* num;//动画的图元指针
    int choose;//start()函数的参数

protected:
    float CurrentModulus;//当前系数
    float CurrentModulus2;//当前系数2（处理多系数变化,下同）
    float CurrentModulus3;//当前系数3
    float TargetModulus;//目标系数1
    float TargetModulus2;//目标系数2
    float TargetModulus3;//目标系数3
    int times;//变化所需【总】时间,单位：毫秒
    QTimer *timer;//计时器
    float temp;//临时变量，在某些特殊情况下用于各个函数之间通信使用
    float temp1;//临时变量，在某些特殊情况下用于各个函数之间通信使用
    float temp2;//临时变量，在某些特殊情况下用于各个函数之间通信使用
    float temp3;//临时变量，在某些特殊情况下用于各个函数之间通信使用

    //旋转                           choose==1
    //缩放                           choose==2
    //相对移动一个图元                 chose==3
    //将一个图元变模糊                 choose==4
    //设置一个图元的透明度              choose==5
    //在一个图元基础上进行着色           choose==6
    void changepixmap();//Gif改变图元  choose==7
    //剪切一个图元                      choose==8
    QVector<QPixmap>::iterator iter;

private slots:
    void SlowChange();
};


class MyItem : public QGraphicsPixmapItem//图元类
{
public:
    MyItem(QGraphicsItem *parent = 0):QGraphicsPixmapItem::QGraphicsPixmapItem(parent){}
    MyItem(const QPixmap &pixmap, QGraphicsItem *parent = 0)
        :QGraphicsPixmapItem::QGraphicsPixmapItem(pixmap,parent){}
    QPixmap up;
    QString fun;
    QPixmap down;
    Widget *s;
    ParametersStru *par;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //允许在此添加更多类型的事件，或许你需要添加一些公有成员变量来记录其所对应的槽函数
};


class VideoPlayer : public QWidget //视频类
{
    Q_OBJECT
public:
    VideoPlayer(QString Path,int Volume, int x, int y,int width,int heigh,bool cycle,QString signfun,QGraphicsScene *scene,QWidget *parent = 0);
    ~VideoPlayer();
    void start();
    QMediaPlayer *mediaPlayer;
    QString signfun;

private:
    QGraphicsVideoItem *videoItem;
    bool cycle;
    QString Path;

private slots:
    void playover(QMediaPlayer::State state);
};


class GraphicsView : public QGraphicsView//视图类
{
public:
    GraphicsView(QWidget *parent):QGraphicsView(parent){}
    void Scale(float sx, float sy);
    void Rotate(float set);

protected:
    void wheelEvent(QWheelEvent *e)
    {
        //不对滚轮事件做任何处理，防止滚动view（如果忽略还会向下层机制传递消息）
    }
    void keyPressEvent(QKeyEvent *e)
    {
        switch(e->key())
        {
        case Qt::Key_Left:{}
        case Qt::Key_Right:{}
        case Qt::Key_Down:{}
        case Qt::Key_Up:{}
        default:
        {e->ignore();}//忽略键盘消息，防止未定义按键滚动view
        }
    }
};


class EasyThread : public QThread//线程类
{
public:
    EasyThread(QString fun,ParametersStru *par=nullptr):
        fun(fun),par(par){}
    void run();
    QString fun;
    ParametersStru *par;
};


enum AnimationType{Rotation,Scale,Move,BlurRadius,Opacity,Color,Picture,Shear};


class Item
{
public:
    Item(MyItem* pixmapitem=nullptr,QGraphicsItem *graphicsitem=nullptr);
    Item(QPixmap *pixmap);

    QGraphicsItem *ItemPointer;
    MyItem *PixmapItemPoniter;
    QGraphicsBlurEffect *Blur;
    QGraphicsColorizeEffect *Color;
    float ShearX;
    float ShearY;
    SC* scPointer[8];
};


class ParametersStru : public QObject
{
    Q_OBJECT
public:
    QVector<int> intVar;
    QVector<float> floatVar;
    QVector<QString> QStringVar;
    QVector<bool> boolVar;
    QVector<VideoPlayer*> VideoPlayerVar;
    QVector<GraphicsView*> GraphicsViewVar;
    QVector<EasyThread*> EasyThreadVar;
    QVector<AnimationType> AnimationTypeVar;
    QVector<MyItem*> MyPixmapVar;

    QVector<QMediaPlayer*> QMediaPlayerVar;
    QVector<QGraphicsScene*> QGraphicsSceneVar;
    QVector<QScriptValue> QScriptValueVar;
    QVector<Qt::Key> QtKeyVar;
};

struct InputEvent
{
    Qt::Key key;
    float MouseX;
    float MouseY;
    float fMouseX;
    float fMouseY;
    ParametersStru *par;
    QString slotfun;
};

void RunFun(QString fun, ParametersStru *par=nullptr, Qt::ConnectionType CT=Qt::QueuedConnection);
