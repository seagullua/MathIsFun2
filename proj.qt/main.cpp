#include "main.h"
#include "../Classes/AppDelegate.h"
#include <Testing/ADDeviceEmulator.h>
#include <ADLib/Device/ADAds.h>

USING_NS_CC;

void openMainScene(TestInfo info);
void openAllCollectionScene(TestInfo info);
void openOneCollectionScene(TestInfo info,
                              int collection_number);
void openOneLevelScene(TestInfo info,
                       int collection_number,
                       int level_number);

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
    emulator->setDevice(Device::IPhone4GS);
    emulator->setLanguage("uk");
    emulator->setOrientation(Orientation::Landscape);


    //add screen tests
    emulator->addTestCase(&prepare);

    emulator->addTestCase(&openMainScene);
    emulator->addTestCase(&openAllCollectionScene);
    emulator->addTestCase([](TestInfo info){
        openOneCollectionScene(info,100);
    });
    emulator->addTestCase([](TestInfo info){
        openOneLevelScene(info,100,7);
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
void openAllCollectionScene(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
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
                       int level_number)
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

    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(level));
    ADDeviceEmulator::runLater(2.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
