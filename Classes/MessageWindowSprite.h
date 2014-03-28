//
//  MessageWindowSprite.h
//  mmk
//
//  Created by n01006 on 2014/01/06.
//
//

#ifndef mmk_MessageWindowSprite_h
#define mmk_MessageWindowSprite_h

#include "TapSprite.h"

#include "unicode/utf.h"
#include "unicode/ustring.h"

USING_NS_CC;

class MessageWindowSprite : public CCSprite,public CCTargetedTouchDelegate
{
public:
    static MessageWindowSprite* create(const char *pszFileName);
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    //virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    //virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    

    virtual void setLabelString(CCString LabelStr);
    virtual void startStringDisplay();
    virtual void startAllStringDisplay();
    virtual void delayLabelString(CCNode* sender,void* pLabelStr);
    
    
    virtual void setRowsPerPage(int rows);
    virtual void setCharsPerRow(int chars);
    virtual void setDelayInterval(float interval);
    
    virtual void setCcLabelFontSize(int fontSize);
    
    
    CCArray* MessageStrCCArray;
    
    int display_page = 0;
    int display_rows = 0;
   
    
    //1行の文字数
    int     chars_per_row = 18;
    //1ページの行数
    int     rows_per_page = 2;
   
    //文字表示間隔(s)
    float   delay_interval = 0.1;
    
    //開始位置
    int     start_x_pos = 24;
    int     start_y_pos = 100;
    //フォントサイズ
    int     cc_label_font_size = 24;
    //CCLabelTTFを使う(優先)
    bool    cc_label_ttf_enable = true;
    //CCLabelBMFont
    bool    cc_label_bmf_enable = false;
    //フォント名
    char    *cc_label_font_name = "TanukiMagic";
    //文字色
    ccColor3B cc_label_color = ccc3(0,0,0);
    //フォントファイル名
    char    *cc_label_font_file_name = "sample_font.fnt";
    
    
    
    bool    display_finish = false;
    
};


#endif
