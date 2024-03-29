#include "SceneStyle.h"
#include <ADLib/Device/ADDeviceEvents.h>
using namespace cocos2d;
SceneStyle::SceneStyle()
    : _pop_up_manager(this)
{
    CONNECT(ADDeviceEvents::signalOnBackClicked, this, &SceneStyle::simulateBackClick);
}

bool SceneStyle::init()
{
    bool res = CCLayer::init();
    this->setKeypadEnabled(true);

    return res;
}
void SceneStyle::simulateBackClick()
{
    if(!_pop_up_manager.backAction())
    {
        onBackClick();
    }
}

void SceneStyle::keyBackClicked()
{
    simulateBackClick();
}
