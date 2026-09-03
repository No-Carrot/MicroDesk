#ifndef PINYININPUTMETHOD_H
#define PINYININPUTMETHOD_H
#include "abstractinputmethod.h"

class PinyinDecoderService;
class PinyinInputMethod : public GroupInputMethod
{

      Q_OBJECT
public:
    PinyinInputMethod();
    virtual bool handleKeyPressEvent(QKeyEvent *e);
    virtual int language() ;
    virtual void commit(const QString &text=QString());

private:
    void setCommit(const QString &text =QString(), bool bSymbol=false);
    PinyinDecoderService * service();
    QString  handleSymbolConversion (QKeyEvent *e);
    bool setChoose(const QString &chinese);
    void updateChoose();
    void init();
    QString toWordPingyin(const QString &pinyin);
    void resetPinyin();

private:
    QString m_chooseCandiatate; //最终提交中文.
    QString m_pinyinSring; //拼音字幕。
};

#endif // PINYININPUTMETHOD_H
