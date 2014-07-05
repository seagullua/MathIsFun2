#include "Loading.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "Logic/Language.h"
#include "GameInfo.h"
using namespace cocos2d;

Loading::Loading() : _sprites_to_load(0), _loaded_sprites(0)
{
}
CCScene* Loading::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    Loading *layer = Loading::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void Loading::drawEverything(float)
{
    //Get the size of the screen we can see
    CCSize visibleSize = ADScreen::getVisibleSize();
    //Get the screen start of cordinates
    CCPoint origin = ADScreen::getOrigin();
    float scaled = ADScreen::getScaleFactor();

    //Place background
    CCSprite* pSprite = CCSprite::create("background/sheet-background.jpg");
    CCPoint sp_origin = pSprite->boundingBox().origin;
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x,
                             origin.y + visibleSize.height + sp_origin.y));
    this->addChild(pSprite);

    float x_middle_of_sheet = (visibleSize.width-133/scaled)/2 + origin.x;
    CCPoint logo_target_position = ccp(x_middle_of_sheet,
                                       visibleSize.height/2);


    SpritesLoader spl = GraphicsManager::getLoaderFor(this,
                                                      "loading_screen.plist",
                                                      "loading_screen.png");
    _logo = spl->loadSprite("loading_logo.png");
    _logo->setPosition(logo_target_position);


}

void Loading::loadSprite(const char* name)
{
    _sprites_to_load++;

    CCTextureCache::sharedTextureCache()->
            addImageAsync(name, this, callfuncO_selector(Loading::loadingCallBack));

}
void Loading::hideLogo()
{
    _logo->runAction(CCSequence::create(
                         CCFadeTo::create(0.2f, 0),
                         CCCallFunc::create(this, callfunc_selector(Loading::openMenu)),
                         NULL));
}

void Loading::openMenu()
{
    CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
}

void Loading::loadingCallBack(CCObject *)
{
    _loaded_sprites++;

    if(_loaded_sprites == _sprites_to_load)
    {
        const float splash_screen_time = 1;

        this->runAction(CCSequence::create(
                            CCDelayTime::create(splash_screen_time),
                            CCCallFunc::create(this, callfunc_selector(Loading::hideLogo)),
                            NULL));


    }
}

#include "Logic/RW.h"
#include "Logic/Tutorial.h"
#include "AppDelegate.h"
#include <ADLib/Device/ADInApp.h>
void Loading::initRW()
{
    //load storage
    ADStorage::loadInitialData();

    //TODO: write cleanup
    RW::init();

    std::string fullPath = "";
    if(GameInfo::GAME_VERSION == GameVersion::VersionPlus)
        fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("levels.ad");
    else if(GameInfo::GAME_VERSION == GameVersion::VerionMultiply)
        fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("levels_multiplication.ad");

    unsigned long size = 0;
    unsigned char* file_cont = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &size);

    assert(size > 0); //TODO: put message that file can't be readed
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    ss.write((char*)file_cont, size);

    ADStreamIn stream(ss);

    //read levels
    RW::readLevelsInformation(stream);

    //read saves
    RW::readSavedData();

    Tutorial::getInstance()->init();

    /**
         * The initialization of StoreController. This is the first and ONLY time it's initialized.
         **/


    ADAds::prepareInterstitial();

    ADInApp::loadStore(ADInApp::Mode::Production);


}
void Loading::addAll()
{


//    addSpriteToLoad("common.png");
//    addPlistToLoad("common.plist");

//    addSpriteToLoad("loading_screen.png");
//    addPlistToLoad("loading_screen.plist");




//    addSpriteToLoad("font/mathisfun.png");
//    addSpriteToLoad("font/mathisfun_digits.png");
//    addPlistToLoad("font/mathisfun_digits.plist");

//    addSpriteToLoad("level/input_space.png");
//    addPlistToLoad("level/input_space.plist");

//    addSpriteToLoad("level/keyboard.png");
//    addPlistToLoad("level/keyboard.plist");

//    addSpriteToLoad("level/level_end.png");
//    addPlistToLoad("level/level_end.plist");

//    addSpriteToLoad("level/level_end_menu.png");
//    addPlistToLoad("level/level_end_menu.plist");

//    /*addSpriteToLoad(Language::localizeFileName("level/found_solutions.png").c_str());
//    addPlistToLoad(Language::localizeFileName("level/found_solutions.plist").c_str());
//    */
    addSpriteToLoad("level/stamps.png");
    addPlistToLoad("level/stamps.plist");

//    addSpriteToLoad("level_scene/ellements.png");
//    addPlistToLoad("level_scene/ellements.plist");

//    addSpriteToLoad("main_menu/images.png");
//    addPlistToLoad("main_menu/images.plist");

//    addSpriteToLoad("main_menu/menu.png");
//    addPlistToLoad("main_menu/menu.plist");

//    addSpriteToLoad("sheet_menu.png");
//    addPlistToLoad("sheet_menu.plist");


//    addSpriteToLoad("select_collection/new_collection.png");
//    addPlistToLoad("select_collection/new_collection.plist");

//    //addSpriteToLoad(Language::localizeFileName("select_collection/images.png").c_str());
//    //addPlistToLoad(Language::localizeFileName("select_collection/images.plist").c_str());

//    addSpriteToLoad("select_level/marks.png");
//    addPlistToLoad("select_level/marks.plist");

//    addSpriteToLoad(Language::localizeFileName("select_level/menu_name.png").c_str());
//    addPlistToLoad(Language::localizeFileName("select_level/menu_name.plist").c_str());

//    //    addSpriteToLoad(Language::localizeFileName("settings/developers_menu_name.png").c_str());
//    //    addPlistToLoad(Language::localizeFileName("settings/developers_menu_name.plist").c_str());

//    addSpriteToLoad("settings/settings_developers_ad.png");
//    addPlistToLoad("settings/settings_developers_ad.plist");

//    addSpriteToLoad(Language::localizeFileName("settings/settings_menu.png").c_str());
//    addPlistToLoad(Language::localizeFileName("settings/settings_menu.plist").c_str());

//    //addSpriteToLoad(Language::localizeFileName("settings/settings_name.png").c_str());
//    //addPlistToLoad(Language::localizeFileName("settings/settings_name.plist").c_str());

//    //	addSpriteToLoad("stop_level/sheet.png");
//    //	addPlistToLoad("stop_level/sheet.plist");

//    addSpriteToLoad(Language::localizeFileName("stop_level/stop_level_menu.png").c_str());
//    addPlistToLoad(Language::localizeFileName("stop_level/stop_level_menu.plist").c_str());


//    addSoundToLoad("music/choose.wav");
//    addMusicToLoad(GameInfo::BACKGROUND_MUSIC.c_str());

//    const CollectionsArr& arr = RW::getCollections();
//    for(CollectionsArr::const_iterator it = arr.begin(); it != arr.end(); ++it)
//    {
//        Collection* a = it->second;
//        std::stringstream ss;
//        std::stringstream ss_f;
//        ss << "medium/select_collection/coll_" << a->getCollectionID() << ".png";
//        ss_f << "medium/select_collection/clabel_" << a->getCollectionID() << ".png";
//        addSpriteToLoad(ss.str().c_str());
//        addSpriteToLoad(Language::localizeFileName(ss_f.str()).c_str());
//    }
//    addSpriteToLoad("medium/select_collection/coll_lock.png");
//    addSpriteToLoad(Language::localizeFileName("medium/select_collection/clabel_locked.png").c_str());
}
void Loading::addSpriteToLoad(const char* sp)
{
    _q_sprites_to_load.push(sp);
}
void Loading::addMusicToLoad(const char* s)
{
    _q_music_to_load.push(s);
}
void Loading::addSoundToLoad(const char* s)
{
    _q_sound_to_load.push(s);
}
void Loading::addPlistToLoad(const char* s)
{
    _q_plists_to_load.push(s);
}
void Loading::loadAll()
{
    CCTextureCache* cache = CCTextureCache::sharedTextureCache();
    while(!_q_sprites_to_load.empty())
    {
        std::string str = _q_sprites_to_load.front();
        cache->addImage(str.c_str());
        _q_sprites_to_load.pop();
    }

    CocosDenshion::SimpleAudioEngine* engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    while(!_q_music_to_load.empty())
    {
        std::string str = _q_music_to_load.front();
        engine->preloadBackgroundMusic(str.c_str());
        _q_music_to_load.pop();
    }

    while(!_q_sound_to_load.empty())
    {
        std::string str = _q_sound_to_load.front();
        engine->preloadEffect(str.c_str());
        _q_sound_to_load.pop();
    }

    CCSpriteFrameCache* plist_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    while(!_q_plists_to_load.empty())
    {
        std::string str = _q_plists_to_load.front();
        plist_cache->addSpriteFramesWithFile(str.c_str());
        _q_plists_to_load.pop();
    }
}
void Loading::onLoadFinished()
{
    const float splash_screen_time = float(SPLASH_SCREEN_TIME);


    this->runAction(CCSequence::create(
                        CCDelayTime::create(splash_screen_time),
                        CCCallFunc::create(this, callfunc_selector(Loading::hideLogo)),
                        NULL));

}


void Loading::loadAllAsync(float)
{
    addAll();
    loadAll();
    onLoadFinished();
}


bool Loading::init()
{
    if (!SceneStyle::init())
    {
        return false;
    }


    drawEverything(0);
    initRW();
    this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::loadAllAsync), this, 0.1, 0, 0, false);

    return true;
}
