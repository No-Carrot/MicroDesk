#include "inputmethodmanager.h"
#include "pinyininputmethod.h"
#include "iminputcontext.h"
#include "implatforminputcontext.h"
#include <QLocale>
#include <QtDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QtDebug>

static  InputMethodManager *s_instance= nullptr;
InputMethodManager::InputMethodManager(QObject *parent) :
    QObject(parent),
    m_input(nullptr),
    m_settings(new QSettings("inputmethod.qt"))
{
    if(m_settings->contains("language")){
        int language = m_settings->value("language").toInt();
        this->create(language);
        if(!m_input){
            this->create(QLocale::English);
        }
    }
}

AbstractInputMethod *InputMethodManager::curentInputMethod()
{
    if(m_input == nullptr)
        create(QLocale::English);

    return m_input;
}

void InputMethodManager::setCurrentInputMethod(int lge)
{
    create(lge);
}

void InputMethodManager::changeLanguage()
{
    if(m_lges.isEmpty()){
        this->create(QLocale::English);
        return;
    }

    if(m_input->language() == QLocale::English)
    {
        this->create(QLocale::Chinese);
    }else{
        this->create(QLocale::English);
    }

    m_input->reset();
    m_settings->setValue("language", QVariant(m_input->language()));
}

InputMethodManager::~InputMethodManager()
{
    foreach (AbstractInputMethod *input, m_inputList) {
        delete input;
    }; m_inputList.clear();m_lges.clear();
}


void InputMethodManager::create(int lge)
{
    if(m_lges.contains(lge)){
        m_input = m_inputList.at(m_lges.indexOf(lge));
        return;
    }

    AbstractInputMethod * input=nullptr;
    switch (lge) {
    case QLocale::Chinese:
        input = new PinyinInputMethod;
        break;

    case QLocale::English:
        input = new DefaultInputMethod;
        break;

    default:
        return;
    }


    m_input = input;
    connect(m_input, &AbstractInputMethod::dataChange,   this, [=](const QStringList &data, const QString &pinyin){
        static IMInputContext *pInstance = IMInputContext::instance();
        pInstance->setInputPanelVisible(!pinyin.isEmpty());
        //qDebug()<< data << pinyin << pInstance->isInputPanelVisible();
        pInstance->isInputPanelVisible()? pInstance->platformCtx()->showInputPanel():pInstance->platformCtx()->hideInputPanel();
        emit pInstance->dataChange(data, pinyin);
    });

    m_inputList.push_back(input);
    m_lges.push_back(input->language());
}

InputMethodManager *InputMethodManager::instance()
{
    if(s_instance ==nullptr){
        s_instance = new InputMethodManager;
    }
    return s_instance;
}
