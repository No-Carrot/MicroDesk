#include "keyboardwidget.h"
#include "iminputcontext.h"
#include "implatforminputcontext.h"
#include "abstractinputmethod.h"
#include "inputmethodmanager.h"
#include <QListView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <QWindow>
#include <QStringListModel>
#include <QScreen>
#include <QtMath>
#include <QTimer>

#define  PageKey Qt::Key_unknown+1
#define  LanguageKey Qt::Key_unknown+2
#define  CloseKey     Qt::Key_unknown+3
#define  NotUsedKey    Qt::Key_unknown+6
#define  PAGEKEYCOUNT 39

class KeyboardWidgetPrivate{
public:
    QWidget *keyboardWidget=nullptr;
    QListView *listView=nullptr;
    QStringListModel model;

    QList<ButtonKey*> buttons;

    QList<int> pageKeys1;
    QList<int> pageKeys2;
    QList<int> pageKeys3;

    static int currentPage;
    bool bBigLetter =false;
    bool bLetterPage =true;

    QRect rect;
    bool bSupport =true;
    void updatPage(int nPage =-1);
    void updateState();
};

void KeyboardWidgetPrivate::updatPage(int nPage)
{
    QList<int> keys;;

    if(nPage == -1)
        nPage = currentPage;

    switch (nPage) {
    case 0:{
        keys = pageKeys1;
        currentPage =0;
    }break;
    case 1: {
        keys = pageKeys2;;
        currentPage =1;
    }break;
    case 2:{
        keys = pageKeys3;
        currentPage =2;
    }
        break;
    default:
        break;
    }

    AbstractInputMethod *pInstance = InputMethodManager::instance()->curentInputMethod();
    for(int i=0; i<keys.size() && i<buttons.size(); i++){
        int key = keys[i];
        QString text = pInstance->getText(key);
        if(AbstractInputMethod::isLetter(key)){
            bBigLetter ?  text =text.toUpper() :  text =text.toLower();
        }
        buttons[i]->set(text,  key);
    }
}

void KeyboardWidgetPrivate::updateState()
{
    switch (currentPage) {
    case 0:
        this->updatPage(0);
        break;
    case 1:
        this->updatPage(2);
        break;
    case 2:
        this->updatPage(1);
        break;
    default:
        break;
    }
}


class  Keyboard: public QWidget
{
public:
    Keyboard(QWidget *parent =nullptr):QWidget(parent){}

    void paintEvent(QPaintEvent *event){
        QWidget::paintEvent(event);
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};
int KeyboardWidgetPrivate::currentPage =0;

KeyboardWidget::KeyboardWidget(QObject *parent) :
    QObject(parent),
    m_d(new KeyboardWidgetPrivate)
{

    //page1
    int letterPage[PAGEKEYCOUNT]={Qt::Key_Q, Qt::Key_W, Qt::Key_E, Qt::Key_R, Qt::Key_T, Qt::Key_Y, Qt::Key_U, Qt::Key_I, Qt::Key_O, Qt::Key_P, Qt::Key_Backspace, //11
                                  Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_F, Qt::Key_G, Qt::Key_H, Qt::Key_J, Qt::Key_K, Qt::Key_L , Qt::Key_Return, //10
                                  Qt::Key_Shift, Qt::Key_Z, Qt::Key_X, Qt::Key_C, Qt::Key_V, Qt::Key_B, Qt::Key_N, Qt::Key_M, Qt::Key_Period, Qt::Key_Comma, Qt::Key_Shift, //11
                                  PageKey , NotUsedKey, NotUsedKey,  LanguageKey, NotUsedKey, NotUsedKey, CloseKey //7
                                 };
    //page2
    int numberPage[PAGEKEYCOUNT]={ Qt::Key_1,Qt::Key_2,Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6,Qt::Key_7,Qt::Key_8,Qt::Key_9, Qt::Key_0, Qt::Key_Backspace, //11
                                   Qt::Key_At, Qt::Key_NumberSign,  Qt::Key_Percent, Qt::Key_Ampersand, Qt::Key_Asterisk, Qt::Key_Minus, Qt::Key_Plus, Qt::Key_ParenLeft, Qt::Key_ParenRight, Qt::Key_Return,//10
                                   Qt::Key_Shift, Qt::Key_Exclam, Qt::Key_QuoteDbl, Qt::Key_Less, Qt::Key_Greater, Qt::Key_Apostrophe, Qt::Key_Colon, Qt::Key_Semicolon, Qt::Key_Period, Qt::Key_Comma,Qt::Key_Shift,//11
                                   PageKey , NotUsedKey, NotUsedKey,  LanguageKey, NotUsedKey, NotUsedKey, CloseKey //7
                                 };
    //page3
    int symbolPage[PAGEKEYCOUNT]={ Qt::Key_1,Qt::Key_2,Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6,Qt::Key_7,Qt::Key_8,Qt::Key_9, Qt::Key_0, Qt::Key_Backspace, //11
                                   Qt::Key_QuoteLeft, Qt::Key_AsciiTilde, Qt::Key_Dollar, Qt::Key_AsciiCircum, Qt::Key_Underscore,Qt::Key_Equal, Qt::Key_BraceLeft, Qt::Key_BraceRight, Qt::Key_BracketLeft, Qt::Key_Return,//10
                                   Qt::Key_Shift, Qt::Key_BracketRight, Qt::Key_Bar, Qt::Key_Backslash, Qt::Key_Slash, Qt::Key_Question ,Qt::Key_Period, Qt::Key_Comma,  NotUsedKey,NotUsedKey, Qt::Key_Shift,//11
                                   PageKey , NotUsedKey, NotUsedKey,  LanguageKey, NotUsedKey, NotUsedKey, CloseKey //7
                                 };

    m_d->keyboardWidget = new Keyboard;
    m_d->keyboardWidget->setObjectName("im_keyboardwidget");

    QVBoxLayout *vBox=new QVBoxLayout(m_d->keyboardWidget);
    m_d->listView = new QListView;
    m_d->listView->setObjectName("candidateView");
    connect(m_d->listView, &QListView::clicked, this, [=](const QModelIndex &index){
        QString text =index.data().toString();
        InputMethodManager::instance()->curentInputMethod()->commit(text);
    });
    m_d->listView->setInputMethodHints(Qt::ImhNone);;
    m_d->listView->setFocusPolicy(Qt::NoFocus);
    m_d->listView->setStyleSheet("border:none;");
    m_d->listView->setEditTriggers(QListView::NoEditTriggers);
    m_d->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_d->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_d->listView->setFlow(QListView::LeftToRight);
    m_d->listView->setContentsMargins(0,0,0,0);
    m_d->listView->setModel(&m_d->model);
    m_d->listView->setSpacing(6);

    m_d->listView->setMinimumHeight(20);
    m_d->listView->setMaximumHeight(35);

    QMargins marging = vBox->contentsMargins();
    vBox->setContentsMargins(marging.left(), 5, marging.right(), marging.bottom());
    vBox->addWidget(m_d->listView);
    vBox->setSpacing(0);
    QGridLayout *gLayout = new QGridLayout;
    if(qApp->primaryScreen() && qApp->primaryScreen()->size().width() < 1280)
          gLayout->setSpacing(6);
    else
        gLayout->setSpacing(10);
    vBox->addLayout(gLayout);

    int span =1;
    int count = 0;
    for(int i=0; i<PAGEKEYCOUNT; i++){
        ButtonKey *btn = new ButtonKey;
        btn->set("",letterPage[i] );
        m_d->buttons <<btn;
        int key = letterPage[i];

        if(key == Qt::Key_Backspace)
        {
            btn->setAutoRepeat(true);
        }

        switch (key) {
        case PageKey:  //切换页面.
            btn->setObjectName("key_page"); span = 1;
            connect(btn, &ButtonKey::clicked, this, [=](){
                m_d->currentPage == 0 ? m_d->currentPage =2: m_d->currentPage =0;
                m_d->updateState();
            });
            break;
        case Qt::Key_Shift: //大小写.
            btn->setObjectName("key_shift"); span = 1;
            connect(btn, &ButtonKey::clicked,  [=](){
                m_d->bBigLetter = !m_d->bBigLetter;
                m_d->updateState();
            });
            break;

        case LanguageKey:  //语言切换按键.
            btn->setObjectName("key_language"); span = 5;
            connect(btn, &ButtonKey::clicked, this, [=](){
                if(InputMethodManager::instance()->curentInputMethod()->language() == QLocale::Chinese){
                    IMInputContext::instance()->commit();
                }
                InputMethodManager::instance()->changeLanguage();
                m_d->updatPage();
            });
            break;

        case CloseKey: //关闭按键
        {
            btn->setObjectName("key_close"); span = 1;
            connect(btn, &ButtonKey::clicked, this, [=](){
                IMInputContext::instance()->commit();
                this->hide();
            });
        }
            break;

        case NotUsedKey: //不使用的按键，占位用途.
            btn->setObjectName("key_empty"); span = 1;
            btn->setEnabled(false);
            break;

        default:
            span =1; btn->setObjectName("key_common");
            connect(btn, &ButtonKey::pressed, this, [=](){
                QString text = btn->text();
                if(InputMethodManager::instance()->curentInputMethod()->language() == QLocale::Chinese){
                    text = text.toLower();
                }
                QKeyEvent e(QEvent::KeyPress, btn->key(), Qt::NoModifier, text);
                IMInputContext::instance()->eventFilter(nullptr, &e);
            });

            if(key == Qt::Key_Return){
                span = 2;
                btn->setObjectName("key_enter");
            }
            break;
        }

        gLayout->addWidget(btn, count/11, count%11, 1, span);
        count +=span;

        //保存页按键，用于切换处理.
        m_d->pageKeys1 << letterPage[i];
        m_d->pageKeys2 << numberPage[i];
        m_d->pageKeys3 << symbolPage[i];
    }

    m_d->updatPage(0);
    connect(IMInputContext::instance(), &IMInputContext::dataChange, this, [=](const QStringList &model){
        m_d->model.setStringList(model);
    });


    //platform set. (xcb/eglfs/linuxfb 无窗口管理器，使用相同策略)
    QString platform = qApp->platformName();
    if(platform == "xcb" || platform == "eglfs" || platform == "linuxfb"){
        // linuxfb/eglfs 平台：使用 Popup 标志确保键盘在最上层
        if(platform == "eglfs" || platform == "linuxfb") {
            m_d->keyboardWidget->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        } else {
            m_d->keyboardWidget->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus | Qt::FramelessWindowHint);
        }
        m_d->bSupport = true;

        if(platform == "eglfs" || platform == "linuxfb")
        {
            qApp->installEventFilter(this);
        }
    }else{
        m_d->keyboardWidget->setFocusPolicy(Qt::NoFocus);
        connect(qApp, &QApplication::focusChanged, [=](QWidget *old, QWidget *now){


            //remove parent-child .
           if(m_d->keyboardWidget && m_d->keyboardWidget->parentWidget()){
              m_d->keyboardWidget->parentWidget()->removeEventFilter(this);
               m_d->keyboardWidget->setParent(NULL);
            }

            //qDebug()<<"old = " << old <<" now="<<now <<"keyboardWidget=" << m_d->keyboardWidget;
            if(now && now != m_d->keyboardWidget && !now->isModal()){;
               QWidget *topWidget = now->topLevelWidget();
                if(topWidget && qApp->primaryScreen()){
                    QSize size = qApp->primaryScreen()->size();
                    m_d->bSupport=topWidget->isFullScreen()
                            || topWidget->isMaximized()
                            ||(size.width() <= topWidget->size().width() && size.height() <=topWidget->height() );
                    //qDebug()<<topWidget << m_d->bSupport;
                }else{
                    m_d->bSupport = false;
                }

                //set parent-child.
                if(m_d->bSupport && topWidget){
                    m_d->bSupport = true;
                    topWidget->installEventFilter(this);
                    m_d->keyboardWidget->setParent(topWidget);
                }
            }
        });
    }

    //use imkeyboard.style.
    QString fileName =qApp->applicationDirPath() +"/imkeyboard.style";
    QString styleSheet;
    QFile file(fileName);
    if(file.open(QFile::ReadOnly| QFile::Text)){
        styleSheet = file.readAll().trimmed();
        file.close();
    }

    //default style.
    if(styleSheet.isEmpty()){
        m_d->keyboardWidget->setStyleSheet("QListView{color: green; font: bold 18px; background:rgb(35,35,35);}"
                                           " QPushButton{background: rgb(55,55,55); color: white; font-size: 18px;} "
                                           "QPushButton:pressed{background: rgb(175,175,175); color: white; font-size: 18px;}"
                                           "QWidget#im_keyboardwidget{background:rgb(35,35,35)}");
    }else{
        m_d->keyboardWidget->setStyleSheet(styleSheet);
    }
    connect(qApp, &QApplication::aboutToQuit, this, [=](){
        m_d->keyboardWidget->close();
        delete m_d->keyboardWidget;
        m_d->keyboardWidget = nullptr;
    });

}

KeyboardWidget::~KeyboardWidget()
{
    if(m_d->keyboardWidget)
    {
        m_d->keyboardWidget->close();
        delete m_d->keyboardWidget;
        m_d->keyboardWidget = nullptr;
    }
    delete m_d;
}

void KeyboardWidget::show()
{
    QString platform = qApp->platformName();
    qDebug() << "[Keyboard] show() called, platform:" << platform 
             << "bSupport:" << m_d->bSupport
             << "focusWindow:" << qApp->focusWindow()
             << "focusObject:" << qApp->focusObject();
    
    if(platform == "wayland" && m_d->keyboardWidget->parentWidget() == nullptr){
        qDebug() << "[Keyboard] Wayland without parent, skip";
        return;
    }

    // QTableView/QTableWidget 不自动弹键盘
    if(platform != "xcb" && qApp->focusObject() && qApp->focusObject()->inherits("QAbstractItemView")){
        qDebug() << "[Keyboard] QAbstractItemView detected, hiding";
        this->hide();
        return;
    }

    bool isEmbedded = (platform == "linuxfb" || platform == "eglfs");
    bool canShow = m_d->bSupport && (qApp->focusWindow() == nullptr || !qApp->focusWindow()->isModal());
    
    qDebug() << "[Keyboard] canShow:" << canShow << "isEmbedded:" << isEmbedded;
    
    if(canShow || isEmbedded){
        // 在 linuxfb/eglfs 平台上，将键盘设为当前活动窗口的子窗口
        if (isEmbedded) {
            QWidget *activeWin = qApp->activeWindow();
            if (activeWin && activeWin != m_d->keyboardWidget) {
                // 如果之前有父窗口，先移除
                if (m_d->keyboardWidget->parentWidget() && 
                    m_d->keyboardWidget->parentWidget() != activeWin) {
                    m_d->keyboardWidget->setParent(nullptr);
                }
                // 设置为当前活动窗口的子窗口
                if (m_d->keyboardWidget->parentWidget() != activeWin) {
                    m_d->keyboardWidget->setParent(activeWin);
                    m_d->keyboardWidget->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
                    qDebug() << "[Keyboard] Set parent to:" << activeWin;
                }
            }
        }
        
        this->resize();
        m_d->keyboardWidget->show();
        
        // 在 linuxfb/eglfs 平台上，需要手动提升窗口到最前面
        if (isEmbedded) {
            m_d->keyboardWidget->raise();           // 提升到最前面
            m_d->keyboardWidget->repaint();         // 强制刷新显示
        }
        
        qDebug() << "[Keyboard] Keyboard shown, geometry:" << m_d->keyboardWidget->geometry() 
                 << "parent:" << m_d->keyboardWidget->parentWidget();
    } else {
        qDebug() << "[Keyboard] Cannot show keyboard";
    }
}

void KeyboardWidget::hide()
{
    m_d->keyboardWidget->hide();
}

bool KeyboardWidget::isVisible()
{
    return m_d->keyboardWidget && m_d->keyboardWidget->isVisible();
}

void KeyboardWidget::resize()
{
    qDebug() << "[Keyboard] resize() called";
    
    QRect keyboardRect;
    QScreen *screen = qApp->primaryScreen();
    if(!screen){
        // 无屏幕时使用默认尺寸（800x480 底部区域）
        qDebug() << "[Keyboard] No screen, using default 800x200 at bottom";
        keyboardRect = QRect(0, 280, 800, 200);
        m_d->keyboardWidget->setGeometry(keyboardRect);
        return;
    }
    
    qDebug() << "[Keyboard] Screen geometry:" << screen->geometry() 
             << "available:" << screen->availableGeometry();

    // 获取可用区域
    QWidget *parentWidget = m_d->keyboardWidget->parentWidget();
    if(parentWidget != nullptr){
        // 作为子窗口时，使用父窗口的尺寸，坐标相对于父窗口
        m_d->rect = QRect(0, 0, parentWidget->width(), parentWidget->height());
        qDebug() << "[Keyboard] Using parent widget size:" << m_d->rect;
    }else if(QWidget *active = qApp->activeWindow()){
        m_d->rect = active->geometry();
    }else{
        m_d->rect = screen->availableGeometry();
    }
    
    qDebug() << "[Keyboard] Working rect:" << m_d->rect;

    // 键盘宽度：屏幕宽度的 90%（原来是 75%）
    int width = qMax(100, m_d->rect.width() * 9 / 10);
    // 键盘高度：宽度的 40%（原来是 33%）
    int height = qMax(100, qCeil(width * 2 / 5.0));

    // 竖屏或小屏（如 800x480）时调整
    if(m_d->rect.width() < m_d->rect.height()){
        width = m_d->rect.width();
        height = qMax(100, (int)(width * 2 / 5.0));
    }
    
    // 对于 800x480 小屏，设置更大的键盘
    if(m_d->rect.height() <= 480){
        width = m_d->rect.width();            // 全宽 800px
        height = 240;                         // 固定 240px 高
    }
    
    qDebug() << "[Keyboard] Calculated size:" << width << "x" << height;

    // 嵌入式平台（linuxfb/eglfs）：键盘固定在底部居中
    QString platform = qApp->platformName();
    bool isEmbedded = (platform == "linuxfb" || platform == "eglfs");
    
    if (isEmbedded) {
        // 嵌入式平台：始终固定在底部居中
        keyboardRect.setRect((m_d->rect.width() - width) / 2, m_d->rect.height() - height, width, height);
        qDebug() << "[Keyboard] Embedded platform, fixed at bottom";
    } else {
        IMPlatformInputContext *platCtx = IMInputContext::instance()->platformCtx();
        QWindow *focusWin = qApp->focusWindow();
        
        qDebug() << "[Keyboard] platCtx:" << platCtx << "focusWin:" << focusWin;

        if(platCtx && focusWin){
            QTransform t = QGuiApplication::inputMethod()->inputItemTransform();
            QRect cusRext = platCtx->inputMethodQuery(Qt::ImCursorRectangle).toRect();
            QRect gCursorFocusWindowRect = t.mapRect(cusRext);
            QPoint gWindowPos = focusWin->mapToGlobal(QPoint(0, 0));

            int gCurorX = gWindowPos.x() + gCursorFocusWindowRect.x() + gCursorFocusWindowRect.width();
            int gCurorY = gWindowPos.y() + gCursorFocusWindowRect.y() + gCursorFocusWindowRect.height();
            QPoint gloabePos(gCurorX, gCurorY);

            if(!qApp->property("bottom").toBool()){
                int maxX = m_d->rect.width() - width;
                int maxY = m_d->rect.height() - height;
                if(gloabePos.x() > maxX)
                    gloabePos.setX(maxX);
                if(gloabePos.x() < 0)
                    gloabePos.setX(0);
                if(gloabePos.y() > maxY)
                    gloabePos.setY(gCurorY - height - cusRext.height());
                if(gloabePos.y() < 0)
                    gloabePos.setY(0);

                keyboardRect.setRect(gloabePos.x(), gloabePos.y(), width, height);
            }else{
                keyboardRect.setRect((m_d->rect.width() - width) / 2, m_d->rect.height() - height, width, height);
            }
        }else{
            // 无焦点窗口时：键盘固定在底部
            keyboardRect.setRect((m_d->rect.width() - width) / 2, m_d->rect.height() - height, width, height);
            qDebug() << "[Keyboard] No focus window, using bottom position";
        }
    }
    
    qDebug() << "[Keyboard] Final keyboard rect:" << keyboardRect;

    if(keyboardRect.width() > 0 && keyboardRect.height() > 0){
        m_d->keyboardWidget->setGeometry(keyboardRect);
    }
}

bool KeyboardWidget::eventFilter(QObject *watched, QEvent *event)
{
    QString platform = qApp->platformName();
    if(platform == "eglfs" || platform == "linuxfb"){
        if(event->type() == QEvent::FocusIn){
            QFocusEvent *inEvent = dynamic_cast<QFocusEvent*>(event);
            m_d->bSupport = inEvent && inEvent->reason() == Qt::MouseFocusReason;
        }else if(event->type() == QEvent::MouseButtonPress){
            m_d->bSupport = true;
        }
    }
    return QObject::eventFilter(watched, event);
}


ButtonKey::ButtonKey( QWidget *parent):
    QPushButton(parent),
    m_text("text"),
    m_key(-1)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(20, 20);
}

void ButtonKey::set(const QString &text, int key){
    m_text = text;
    m_key = key;
    m_showText = m_text;
    switch (m_key) {
    case Qt::Key_Return:
        m_showText = "Enter";
        break;

    case Qt::Key_Backspace:
        m_showText = "Del";
        break;

    case Qt::Key_Shift:
        switch (KeyboardWidgetPrivate::currentPage) {
        case 0:
            m_showText = "Caps";
            break;
        case 1:
            m_showText = "1/2";
            break;
        case 2:
            m_showText = "2/2";
            break;
        default:
            break;
        }
        break;

    case Qt::Key_Ampersand:
        m_showText ="&&";
        break;

    case PageKey:
        m_showText = "&&123";
        break;

    case LanguageKey:{
        AbstractInputMethod *im = InputMethodManager::instance()->curentInputMethod();
        if(im){
            if(im->language() == QLocale::English)
                m_showText = "English";
            else if(im->language() == QLocale::Chinese)
                m_showText = "中文";
        }else{
            m_showText = "中文";
        }
        break;
    }

    case CloseKey:
        m_showText = "x";
        break;
    default:
        break;
    }
    this->setText(m_showText);
}

int ButtonKey::key()
{
    return m_key;
}

QString ButtonKey::text()
{
    return m_text;
}

