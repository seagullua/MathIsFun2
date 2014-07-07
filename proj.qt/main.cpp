#include "main.h"
#include "../Classes/AppDelegate.h"
#include <Testing/ADDeviceEmulator.h>
#include <ADLib/Device/ADAds.h>

USING_NS_CC;

void openMainScene(TestInfo info);
void openAllCollectionScene(TestInfo info,
                            bool first_collections);
void openOneCollectionScene(TestInfo info,
                              int collection_number);
void openOneLevelScene(TestInfo info,
                       int collection_number,
                       int level_number,
                       int free_space_id);

void prepare(TestInfo info);


int CALLBACK WinMain(
        _In_  HINSTANCE,
        _In_  HINSTANCE,
        _In_  LPSTR,
        _In_  int
        )
{
    //Create AppDelegate
    AppDelegate app;

    //Configure emulator
    ADDeviceEmulator* emulator = ADDeviceEmulator::getInstance();


    DeviceConfig custom(Device::Custom);
    custom.setDeviceName("Custom");
    custom.setScreenDencity(1.0);
    custom.setScreenWidth(1920);
    custom.setScreenHeight(1200);
    emulator->setCustomDevice(custom);


    emulator->setFitTheScreen(true);
    emulator->setDevice(Device::Galaxy_Tab2_7);
    emulator->setLanguage("uk");
    emulator->setOrientation(Orientation::Landscape);


    //add screen tests
    emulator->addTestCase(&prepare);

    emulator->addTestCase(&openMainScene);
    emulator->addTestCase([](TestInfo info){
        openAllCollectionScene(info,true);
    });
    emulator->addTestCase([](TestInfo info){
        openAllCollectionScene(info,false);
    });
    emulator->addTestCase([](TestInfo info){
        openOneCollectionScene(info,100);
    });
    emulator->addTestCase([](TestInfo info){
        openOneCollectionScene(info,300);
    });
    emulator->addTestCase([](TestInfo info){
        openOneCollectionScene(info,500);
    });
    emulator->addTestCase([](TestInfo info){
        openOneLevelScene(info,100,7,0);
    });
    emulator->addTestCase([](TestInfo info){
        openOneLevelScene(info,600,0,0);
    });
    emulator->addTestCase([](TestInfo info){
        openOneLevelScene(info,500,1,1);
    });
    emulator->addTestCase([](TestInfo info){
        openOneLevelScene(info,3000,3,0);
    });

    return emulator->run();
}





#include "Scenes/SelectCollection.h"
#include "Scenes/SelectLevel.h"
#include "Logic/RW.h"
#include "Scenes/Loading.h"
#include "Scenes/MainMenu.h"
#include "Scenes/LevelScene.h"

void prepare(TestInfo info)
{
    //Loading::initRW();
    ADDeviceEmulator::runLater(1.5f, [info](){
        info.finish();
    });
}
void openMainScene(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
    ADDeviceEmulator::runLater(2.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
void openAllCollectionScene(TestInfo info,
                            bool first_collections)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SelectCollection *layer = SelectCollection::create();

    BackgroundHolder::backgroundSwitchTo(scene,0,false);
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);

    //move scroll zone
    if(!first_collections)
        layer->moveCollectionScrollZone();

    ADDeviceEmulator::runLater(2.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}

void openOneCollectionScene(TestInfo info,
                          int collection_number)
{
    Collection* collection = RW::getCollection(collection_number);

    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(collection));
    ADDeviceEmulator::runLater(2.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}

void openOneLevelScene(TestInfo info,
                       int collection_number,
                       int level_number,
                       int free_space_id)
{
    Collection* collection = RW::getCollection(collection_number);

    //get level
    std::vector<Level*> levels = collection->getLevels();
    Level* level;
    for(unsigned int i=0; i<levels.size() ;++i)
    {
        if(levels[i]->getLevelID() == level_number)
        {
            level = levels[i];
            break;
        }
    }

    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelScene *layer = LevelScene::create(level);

    BackgroundHolder::backgroundSwitchTo(scene,0,false);
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);
    layer->clickEquationSpace(free_space_id);

    ADDeviceEmulator::runLater(3.5f, [info](){


        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
