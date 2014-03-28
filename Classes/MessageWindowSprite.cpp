//
//  MessageWindowSprite.cpp
//
//  Created by @miurakatsu on 2014/01/06.
//
//

#include "MessageWindowSprite.h"

USING_NS_CC;

MessageWindowSprite* MessageWindowSprite::create(const char *pszFileName)
{
    MessageWindowSprite *pobSprite = new MessageWindowSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void MessageWindowSprite::onEnter()
{
    CCSprite::onEnter();
    CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    dispatcher->addTargetedDelegate(this, 0, true);
}

void MessageWindowSprite::onExit() {
    CCSprite::onExit();
    CCTouchDispatcher* dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    dispatcher->removeDelegate(this);
}

bool MessageWindowSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    // タッチ開始処理
    
    // タッチ位置を取得
    CCPoint point = pTouch->getLocation();
    // スプライトの大きさ
    CCSize size = this->getTexture()->getContentSize();
    // スプライトの範囲
    CCRect rect = CCRectMake(
                             this->getPositionX()-size.width/2,
                             this->getPositionY()-size.height/2,
                             size.width,
                             size.height);
    // タッチ位置がスプライト内ならイベント処理するのでtrueを返す
    return rect.containsPoint(point);
}

void MessageWindowSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    // タッチ終了処理
    
    //文字表示中か文字表示前か文字表示終わりかフラグ判定
    if(!display_finish){
        //Window内の表示をクリア
        this->stopAllActions();
        this->removeAllChildren();
        //
        this->startStringDisplay();
    }

}

void MessageWindowSprite::setLabelString(CCString LabelStr)
{
    std::string MessageStr = LabelStr.getCString();
    
    int     pos;
    unsigned char   lead;
    int     charSize = 0;
   
    CCString*   MessageCCString;
    CCArray*    MessageCCArrary = new CCArray;
    
    for (pos=0; pos < MessageStr.size() ; pos += charSize) {
        
        //UTF-8の文字サイズ判別
        lead = MessageStr[pos];
        if (lead < 0x80) {
            charSize = 1;
        } else if (lead < 0xE0) {
            charSize = 2;
        } else if (lead < 0xF0) {
            charSize = 3;
        } else {
            charSize = 4;
        }
       
        //UTF-8の文字を一文字づつ切り出してCCArrayに格納
        MessageCCString = ccs(MessageStr.substr(pos,charSize).c_str());
        MessageCCArrary->addObject(MessageCCString);
    
    }
    //メンバ変数にセット
    MessageStrCCArray = MessageCCArrary;
}

void MessageWindowSprite::startStringDisplay()
{
    int len;
    int message_count = MessageStrCCArray->count();
    
    int max_len;
    
    //1ページに何文字表示できるか
    int chars_per_page = chars_per_row * rows_per_page;

    //複数ページにまたがるか
    if( message_count - display_page * chars_per_page < chars_per_page ){
        max_len = message_count - display_page * chars_per_page;
        display_finish = true;
    }else{
        max_len = chars_per_page;
    }

    
    //文字表示処理
    for (len=display_page * chars_per_page; len < display_page * chars_per_page + max_len; len++) {
        
        CCDelayTime* pDelay = CCDelayTime::create( delay_interval * (len - display_page * chars_per_page) );
        
        CCCallFuncND* pFunc = CCCallFuncND::create(
                                            this,
                                            callfuncND_selector(MessageWindowSprite::delayLabelString),
                                            (void* ) len );
        
        CCSequence* pSeq = CCSequence::create(pDelay,pFunc,NULL);
        
        this->runAction(pSeq);
    }
    
    //ページ送り
    display_page++;
}

void MessageWindowSprite::startAllStringDisplay()
{
    this->setDelayInterval(0);
    this->startStringDisplay();
}

void MessageWindowSprite::delayLabelString(CCNode* sender,void* pStrLen)
{
    //何文字目か
    int pos = (int)pStrLen - (display_page-1) * chars_per_row * rows_per_page;

    //表示する文字
    CCString* Message =  (CCString*)MessageStrCCArray->objectAtIndex((int)pStrLen);
    
    //文字表示部
    if(cc_label_ttf_enable)
    {
        CCLabelTTF* pLabel = CCLabelTTF::create((char*)Message->getCString(), cc_label_font_name, cc_label_font_size);
        pLabel->setColor(cc_label_color);
        
        //X:開始位置 + (フォントサイズ * 行数 ),
        //Y:開始位置 - (フォントサイズ + 文字間隔 ) *　文字数)
        pLabel->setPosition(ccp(start_x_pos +  cc_label_font_size      * ( pos % chars_per_row  )  ,
                                start_y_pos - (cc_label_font_size + 1) * ( pos / chars_per_row)));
        
        this->addChild(pLabel);
        
    }else if(cc_label_bmf_enable)
    {
        CCLabelBMFont* pLabel = CCLabelBMFont::create((char*)Message->getCString(), cc_label_font_file_name);
        
        //X:開始位置 + (フォントサイズ * 行数 ),
        //Y:開始位置 - (フォントサイズ + 文字間隔 ) *　文字数)
        pLabel->setPosition(ccp(start_x_pos +  cc_label_font_size      * ( pos % chars_per_row  )  ,
                                start_y_pos - (cc_label_font_size + 1) * ( pos / chars_per_row)));
        
        this->addChild(pLabel);
    
    }
    
    
    //TODO rowSpriteを用意してそれに置いていけば行送りもできるはず
    
}


void MessageWindowSprite::setRowsPerPage(int rows)
{
    rows_per_page = rows;
}

void MessageWindowSprite::setCharsPerRow(int chars)
{
    chars_per_row = chars;
}

void MessageWindowSprite::setDelayInterval(float interval)
{
    delay_interval = interval;
}

void MessageWindowSprite::setCcLabelFontSize(int fontSize)
{
    cc_label_font_size = fontSize;
}