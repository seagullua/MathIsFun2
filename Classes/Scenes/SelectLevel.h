#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H
#include "cocos2d-A.h"
#include "Logic/Collection.h"
#include <ADLib/Device/ADAds.h>
#include "SceneStyle.h"

class SelectLevel: public SceneStyle
{
public:
    SelectLevel(Collection* colection);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(Collection *colection);

    void onLevelSelect(CCObject* pSender);
private:
   std:: vector<ADMenuItem*> _levels_arr;
   void onBackClick();

   void doGoBack();
   void doOpenLevel();
   void hideEverything(cocos2d::CCCallFunc *callback);
   void buildLevelsTiles();
   cocos2d::CCSprite* _menu_name;
   MenuSpriteBatch* _levels_menu;
   SpritesLoader _lev_spl;
   Collection* _collection;
   Level* _level_to_open;
   ADAds::Banner* _bottom_banner;
public:
   static SelectLevel* create(Collection* colection)
   {
       SelectLevel *pRet = new SelectLevel(colection);
       if (pRet && pRet->init())
       {
           pRet->autorelease();
           return pRet;
       }
       else
       {
           delete pRet;
           pRet = NULL;
           return NULL;
       }
   }

};

#endif // SELECTLEVEL_H
