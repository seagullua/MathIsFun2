#include "SavesManager.h"
#include "Math/Solution.h"
#include "ADLib.h"

SavesManager SavesManager::instance;

SavesManager::SavesManager()
{
    //Add blocks to storage
    CONNECT(ADStorage::signalCreateBlocks, this,
                   &SavesManager::initStorageBlocks);

    //Check if first launch and add bonuses
    CONNECT(ADStorage::signalInitialDataLoaded, this,
                   &SavesManager::initDefaultValues);

//    auto on_currency_change = [this](int64_t currency){
//        this->signalRenewCurrency(currency);
//    };

//    _storage.addValueChangeAction<int64_t>(
//                BLOCK_CURRENCY, on_currency_change);
}

SavesManager* SavesManager::getInstance()
{
    return &instance;
}

void SavesManager::initStorageBlocks()
{
    ADStorage::createMapBlock<Solutions>(BLOCK_SOLUTIONS, KeyType::Int,ADStorage::mergeTakeFirst<Solutions>());

    ADStorage::createMapBlock<Level::LevelState>(BLOCK_LEVELS, KeyType::Int);
    ADStorage::createMapBlock<Collection::CollectionState>(BLOCK_COLLECTIONS, KeyType::Int);

    ADStorage::createValueBlock<int64_t>(BLOCK_HINTS);

    ADStorage::createValueBlock<int64_t>(BLOCK_MUSIC);
    ADStorage::createValueBlock<int64_t>(BLOCK_SOUND);
    ADStorage::createValueBlock<int64_t>(BLOCK_EXPERT_MODE);
    ADStorage::createValueBlock<int64_t>(BLOCK_FULL_VERSION);
    ADStorage::createValueBlock<int64_t>(BLOCK_SHOW_RATE_ME);

    ADStorage::createValueBlock<time_t>(BLOCK_HINT_TIME);

    //last time ads shown
    ADStorage::createValueBlock<time_t>(BLOCK_ADS_SHOW_TIME);
}

void SavesManager::initDefaultValues()
{
    //specially empty for level solutions

    //first time set no date - it will be automatically closed

    //set date for music, sound, expert mode, full version
    if(!ADStorage::hasValue(BLOCK_MUSIC))
        ADStorage::setValue<int64_t>(BLOCK_MUSIC,1);
    if(!ADStorage::hasValue(BLOCK_SOUND))
        ADStorage::setValue<int64_t>(BLOCK_SOUND,1);

    if(!ADStorage::hasValue(BLOCK_EXPERT_MODE))
        ADStorage::setValue<int64_t>(BLOCK_EXPERT_MODE,1);


    if(!ADStorage::hasValue(BLOCK_FULL_VERSION))
        ADStorage::setValue<int64_t>(BLOCK_FULL_VERSION,0);
    else
    {
        if(isFullVersion())
            ADAds::disableAds();
    }


    //add 10 hints
    if(!ADStorage::hasValue(BLOCK_HINTS))
        ADStorage::setValue<int64_t>(BLOCK_HINTS,7);

    if(!ADStorage::hasValue(BLOCK_SHOW_RATE_ME))
        ADStorage::setValue<int64_t>(BLOCK_SHOW_RATE_ME,0);

    //last time for hint add-s
    if(!ADStorage::hasValue(BLOCK_HINT_TIME))
        ADStorage::setValue<time_t>(BLOCK_HINT_TIME,0);

    //last time ads shown
    if(!ADStorage::hasValue(BLOCK_ADS_SHOW_TIME))
        ADStorage::setValue<time_t>(BLOCK_ADS_SHOW_TIME, 0);
}

//SLOTS
void SavesManager::levelSolutionChanged(Collection::CollectionID id_collection,
                                        Level::LevelID id_level,
                                        Solutions solutions)
{
    int64_t key = getLevelID(id_collection,id_level);
    ADStorage::setMapValue<Solutions>(BLOCK_SOLUTIONS, key, solutions);
}
const int64_t SavesManager::getHintCount()
{
    return ADStorage::getValue<int64_t>(BLOCK_HINTS, 0);
}
bool SavesManager::isFullVersion()
{
    int64_t version = ADStorage::getValue<int64_t>(BLOCK_FULL_VERSION, 0);
    if(version == 0)
        return false;
    return true;
}

bool SavesManager::isMusicOn()
{
    int64_t value = ADStorage::getValue<int64_t>(BLOCK_MUSIC, 0);
    if(value == 0)
        return false;
    return true;
}

bool SavesManager::isSoundOn()
{
    int64_t value = ADStorage::getValue<int64_t>(BLOCK_SOUND, 0);
    if(value == 0)
        return false;
    return true;
}

bool SavesManager::isExpertModeOn()
{
    int64_t value = ADStorage::getValue<int64_t>(BLOCK_EXPERT_MODE, 0);
    if(value == 0)
        return false;
    return true;
}
bool SavesManager::isAds()
{
    int64_t value = ADStorage::getValue<int64_t>(BLOCK_FULL_VERSION, 0);
    if(value == 0)
        return true;
    return false;
}
bool SavesManager::isShowedRateMe()
{
    int64_t value = ADStorage::getValue<int64_t>(BLOCK_SHOW_RATE_ME, 0);
    if(value == 0)
        return false;
    return true;
}

void SavesManager::setMusic(bool music_mode)
{
    int64_t music_mode_value = 0;
    if(music_mode)
        music_mode_value = 1;
    ADStorage::setValue<int64_t>(BLOCK_MUSIC,music_mode_value);
}

void SavesManager::setSound(bool sound_mode)
{
    int64_t mode_value = 0;
    if(sound_mode)
        mode_value = 1;
    ADStorage::setValue<int64_t>(BLOCK_SOUND,mode_value);
}

void SavesManager::setExpertMode(bool expert_mode)
{
    int64_t mode_value = 0;
    if(expert_mode)
        mode_value = 1;
    ADStorage::setValue<int64_t>(BLOCK_EXPERT_MODE,mode_value);
}

void SavesManager::setFullVersion(bool full_version)
{
    int64_t mode_value = 0;
    if(full_version)
        mode_value = 1;
    ADStorage::setValue<int64_t>(BLOCK_FULL_VERSION,mode_value);
}

void SavesManager::minusHint()
{
    int64_t hints = ADStorage::getValue<int64_t>(BLOCK_HINTS, 0);
    if(hints>0)
    {
        hints=hints-1;
        return ADStorage::setValue<int64_t>(BLOCK_HINTS, hints);
    }
}

void SavesManager::addHint(int num)
{
    int64_t hints = ADStorage::getValue<int64_t>(BLOCK_HINTS, 0);
    if(hints>0)
    {
        hints+=num;
        return ADStorage::setValue<int64_t>(BLOCK_HINTS, hints);
    }
}

void SavesManager::setHint(int64_t new_hint)
{
    ADStorage::setValue<int64_t>(BLOCK_HINTS, new_hint);
}

void SavesManager::setTimeForHints(time_t new_time)
{
    ADStorage::setValue<time_t>(BLOCK_HINT_TIME, new_time);
}

void SavesManager::showedRateMe()
{
    ADStorage::setValue<int64_t>(BLOCK_SHOW_RATE_ME, 1);
}

void SavesManager::unlockCollection(Collection::CollectionID id)
{
    ADStorage::setMapValue<Collection::CollectionState>(BLOCK_COLLECTIONS, id, Collection::Unlocked);
}

void SavesManager::unlockLevel(Collection::CollectionID c_id,
                               Level::LevelID l_id)
{
    int64_t id = getLevelID(c_id,l_id);
    ADStorage::setMapValue<Level::LevelState>(BLOCK_LEVELS, id, Level::NoStamps);
}

void SavesManager::updateLevelState(Collection::CollectionID c_id,
                                    Level::LevelID l_id,
                                    Level::LevelState new_state)
{
    int64_t id = getLevelID(c_id,l_id);
    ADStorage::setMapValue<Level::LevelState>(BLOCK_LEVELS, id, new_state);
}

void SavesManager::updateCollectionState(Collection::CollectionID c_id,
                      Collection::CollectionState state)
{
    ADStorage::setMapValue<Collection::CollectionState>(BLOCK_COLLECTIONS, c_id, state);
}

bool SavesManager::hasSolutions(Collection::CollectionID c_id,
                  Level::LevelID l_id)
{
    int64_t key = getLevelID(c_id,l_id);
    if(ADStorage::hasMapValue(BLOCK_SOLUTIONS,key))
        return true;
    return false;
}

bool SavesManager::hasCollectionStateValue(Collection::CollectionID id)
{
    if(ADStorage::hasMapValue(BLOCK_COLLECTIONS,id))
        return true;
    return false;
}

bool SavesManager::hasLevelStateValue(Collection::CollectionID c_id,
                        Level::LevelID l_id)
{
    int64_t key = getLevelID(c_id,l_id);
    if(ADStorage::hasMapValue(BLOCK_LEVELS,key))
        return true;
    return false;
}

//get-ers
Collection::CollectionState SavesManager::getCollectionState(
        Collection::CollectionID id)
{
    return ADStorage::getMapValue<Collection::CollectionState>(BLOCK_COLLECTIONS,
                                                               id,
                                                               Collection::Locked);
}

Level::LevelState SavesManager::getLevelState(
        Collection::CollectionID c_id,
        Level::LevelID l_id)
{
    int64_t id = getLevelID(c_id,l_id);
    return ADStorage::getMapValue<Level::LevelState>(BLOCK_LEVELS,
                                                     id,
                                                     Level::Locked);

}

Solutions SavesManager::getSolutions(
        Collection::CollectionID c_id,
        Level::LevelID l_id)
{
    int64_t id = getLevelID(c_id,l_id);
    std::vector<Solution> default_solutions;
    return ADStorage::getMapValue<Solutions>(BLOCK_SOLUTIONS,
                                             id,
                                             default_solutions);

}

time_t SavesManager::getHintTime()
{
    return ADStorage::getValue<time_t>(BLOCK_HINT_TIME, 0);
}

void SavesManager::setADSTimeShowen(time_t last_time)
{
    ADStorage::setValue<time_t>(BLOCK_ADS_SHOW_TIME, last_time);
}

time_t SavesManager::getLastADSTimeShown()
{
    return ADStorage::getValue<time_t>(BLOCK_ADS_SHOW_TIME, 0);
}

void SavesManager::deleteSaves()
{
    ADStorage::cleanMapBlock(BLOCK_SOLUTIONS,KeyType::Int);
    ADStorage::cleanMapBlock(BLOCK_LEVELS,KeyType::Int);
}

//private functions
int64_t SavesManager::getLevelID(
        Collection::CollectionID id_collection,
                   Level::LevelID id_level)
{
    return (id_collection*1000)+id_level;
}
