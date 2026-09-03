#include "pinyininputmethod.h"
#include <QLocale>
#include "pinyindecoderservice_p.h"
#include <QKeyEvent>
#include <QGuiApplication>
#include <QDebug>
#include <qmath.h>
#include <QTimer>

PinyinInputMethod::PinyinInputMethod()
{
    this->init();
}

bool PinyinInputMethod::handleKeyPressEvent(QKeyEvent *e)
{
    //qDebug()<<"e1="<<e;

    //处理特殊字符
    if(!isLetter(e->key()) && m_pinyinSring.isEmpty()){
        m_chooseCandiatate =this->handleSymbolConversion(e);
        if(!m_chooseCandiatate.isEmpty()){ //处理特殊中文特殊字符.
            this->setCommit(m_chooseCandiatate, true);
            return true;
        }else{
            if(e->key() == Qt::Key_Backspace)
                this->sendKeyEvent(e);
            if(e->key() == Qt::Key_Return)
                this->sendKeyEvent(e);
            return false;
        }
    }

    if(!m_pinyinSring.isEmpty() && service()){
        if(e->key() == Qt::Key_Backspace && m_pinyinSring.size() > 0){ //删除字符串.
            service()->deleteSearch(m_pinyinSring.size()-1, true, true);
            m_pinyinSring = m_pinyinSring.left(m_pinyinSring.size()-1);

            service()->search(m_pinyinSring);
            if(!m_pinyinSring.isEmpty()){
                if(service()->candidateAt(0).count() == m_chooseCandiatate.count()){
                    resetPinyin();
                }
            }
        }
    }

    if(isLetter(e->key())){
        m_pinyinSring +=e->text();
    }

    this->updateChoose();


    return true;
}

int PinyinInputMethod::language()
{
    return QLocale::Chinese;
}

void PinyinInputMethod::commit(const QString &text)
{
    this->setCommit(text);
}

bool PinyinInputMethod::setChoose(const QString &chinese)
{
    bool bCommit = chinese.isEmpty();
    if(!service()) return bCommit;
    
    int candidates = service()->search(m_pinyinSring);
    int index = service()->fetchCandidates(0, candidates, 0).indexOf(chinese);
    if(index != -1){
        service()->chooceCandidate(index);
        if(0 == index){
            m_chooseCandiatate.isEmpty() ?  m_chooseCandiatate = chinese:  m_chooseCandiatate = chinese.right(chinese.size()- m_chooseCandiatate.size());
            this->commitText(m_chooseCandiatate);
            bCommit = true;
        }else{
            QString result;
            QString text = m_chooseCandiatate;
            m_chooseCandiatate += chinese;
            bCommit = m_chooseCandiatate == service()->candidateAt(0);
            if(bCommit && !text.isEmpty()){
                result = m_chooseCandiatate.right(m_chooseCandiatate.size() - text.size());
            }else{
                result = chinese;
            }
            this->commitText(result);
        }
    }

    return bCommit;
}

void PinyinInputMethod::updateChoose()
{
    if(!service()) {
        emit dataChange(QStringList(), QString());
        return;
    }
    
    int candidates = service()->search(m_pinyinSring);
    m_pinyinSring = service()->pinyinString(false);
    m_pinyinSring = m_pinyinSring.trimmed();

    //同步显示所有单词
    QStringList showPageWrods = service()->fetchCandidates(0, candidates, 0);
    QString wordPinyinString = toWordPingyin(m_pinyinSring);

    int count = m_chooseCandiatate.count();

    int index = 0;
    for(int i=0, k=0; i < wordPinyinString.size(); i++)
    {
        if(wordPinyinString.at(i) == '\''){
            if(++k == count){
                index = i;
                break;
            }
        }
    }

    setPreeditText(wordPinyinString.mid(index, wordPinyinString.size()-(index+1)));
    emit dataChange(showPageWrods, wordPinyinString);
}


void PinyinInputMethod::setCommit(const QString &text, bool bSymbol)
{
    bool bCommit =bSymbol;
    if(bSymbol)
        commitText(text);
    else
        bCommit = setChoose(text);

    if(bCommit){
//        this->setPreeditText("");
//        this->init();
//        service()->resetSearch();
//        emit dataChange(QStringList(), QString());

        resetPinyin();
    }else{
        updateChoose();
    }

}

PinyinDecoderService *PinyinInputMethod::service()
{
    static PinyinDecoderService *s = PinyinDecoderService::getInstance();
    if(!s){
        qWarning() << "PinyinDecoderService is null! Check if dict_pinyin.dat exists.";
    }
    return s;
}

struct KeyData{
    QString chinese;
    Qt::Key k;
    QString getText(){
        QString ret = chinese;
        if(chinese == "‘")
            chinese ="’";
        else if(chinese =="’")
            chinese ="‘";
        else if(chinese == "“")
            chinese ="”";
        else if(chinese == "”")
            chinese ="“";
        return ret;
    }
};

#define PINYIN_SYNMBOL (33+10)
QString PinyinInputMethod::handleSymbolConversion(QKeyEvent *e)
{
    static struct KeyData chinesKeySymbol[PINYIN_SYNMBOL]={
    {"～", Qt::Key_AsciiTilde}, { "！", Qt::Key_Exclam},{"@", Qt::Key_At},{"#", Qt::Key_NumberSign}, //0-3
    {"￥", Qt::Key_Dollar},{"%", Qt::Key_Percent},{"……", Qt::Key_AsciiCircum},{"&", Qt::Key_Ampersand}, //4-7
    {"*", Qt::Key_Asterisk},{"（", Qt::Key_ParenLeft},{"）", Qt::Key_ParenRight},{"——", Qt::Key_Underscore},//8-11
    {"+", Qt::Key_Plus},{"】", Qt::Key_BraceRight},{"【", Qt::Key_BraceLeft},{"“", Qt::Key_QuoteDbl},//12-15
    {"|", Qt::Key_Bar},{"？", Qt::Key_Question},{"》", Qt::Key_Greater},{"《", Qt::Key_Less},//16-19
    {"-", Qt::Key_Minus},{"=", Qt::Key_Equal},{"『", Qt::Key_BracketLeft},{"』", Qt::Key_BracketRight},//20-23
    {"、", Qt::Key_Backslash},{"‘", Qt::Key_Apostrophe},{"；", Qt::Key_Semicolon},{"/", Qt::Key_Slash},//24-27
    {"。", Qt::Key_Period},{"，", Qt::Key_Comma},{" ", Qt::Key_Space},{"·", Qt::Key_QuoteLeft},//28-32
    {"：" ,Qt::Key_Colon},
    {"0" ,Qt::Key_0},{"1" ,Qt::Key_1},{"2" ,Qt::Key_2},{"3" ,Qt::Key_3},{"4" ,Qt::Key_4},{"5" ,Qt::Key_5},{"6" ,Qt::Key_6},{"7" ,Qt::Key_7},{"8" ,Qt::Key_8},{"9" ,Qt::Key_9}
};

    for (int i=0; i<PINYIN_SYNMBOL; i++) {
        if(e->key() == chinesKeySymbol[i].k){
            return chinesKeySymbol[i].getText();
        }
    }
    return "";
}


void PinyinInputMethod::init()
{
    m_pinyinSring.clear();
    m_chooseCandiatate.clear();
}

QString PinyinInputMethod::toWordPingyin(const QString &pinyin)
{
    QString wordPinyinString;
    if(!service()) return pinyin;
    
    auto getWord = [=](int start, int end){
        QString ret;
        if(start >= 0 && start < pinyin.size() && end > start){
            ret = pinyin.mid(start, end - start);
        }
        return ret;
    };

    //显示拼音字幕 zhongguotiananmen -> zhong'guo'tian'an'men
    QVector<int> spellStartPosList = service()->spellingStartPositions();

    int start = 0;    //起始下标
    int maxCount = 0; //单词从开始到现在的字符数量。
    for(int index = 1; index < spellStartPosList.size() - 1; index++){
        start = spellStartPosList[index];
        maxCount = spellStartPosList[index + 1];
        wordPinyinString += getWord(start, maxCount) + "'";
    }

    //添加无效的拼音字符串.
    if(maxCount < pinyin.size())
        wordPinyinString += pinyin.right(pinyin.size() - maxCount);

    return wordPinyinString;
}

void PinyinInputMethod::resetPinyin()
{
    this->setPreeditText("");
    this->init();
    if(service())
        service()->resetSearch();
    emit dataChange(QStringList(), QString());
}




