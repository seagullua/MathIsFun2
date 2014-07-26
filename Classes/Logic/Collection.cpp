#include "Collection.h"


Collection::Collection()
    : _stamps_obtained(0), _stamps_max(0),
      _crowns_obtained(0), _stamps_to_unlock(0), _collection_free(false),
      _collection_difficulty(Easy)
{
}

Collection::~Collection()
{}
const LevelsArr& Collection::getLevels() const
{
    return _levels;
}
const unsigned int Collection::stampsObtained() const
{
    return _stamps_obtained;
}
const unsigned int Collection::stampsMax() const
{
    return _max_stamps;
}
const unsigned int Collection::crownsObtained() const
{
    return _crowns_obtained;
}
const unsigned int Collection::stampsToUnlock() const
{
    return _stamps_to_unlock;
}
const cocos2d::ccColor3B& Collection::getColor() const
{
    return _collection_color;
}

const Collection::Difficulty Collection::getDifficulty() const
{
    return _collection_difficulty;
}

const Collection::CollectionID Collection::getCollectionID() const
{
    return _collection_id;
}
const Collection::CollectionState Collection::getCollectionState() const
{
    return _state;
}
#include "Logic/RW.h"

#ifndef RW_BUILD_LEVELS
#include "SavesManager.h"
bool Collection::isReadyToBeUnlocked() const
{
    if(_state == Locked)
    {
//        unsigned int overall_stamp = RW::allStampsObtained();
//        if(overall_stamp >= _stamps_to_unlock)
//            return true;
        if(SavesManager::getInstance()->isLevelsUnlocked())
            return true;
    }
    return false;
}
#endif

void Collection::updateStampsAndCrownsCount()
{
    unsigned int stamps = 0;
    unsigned int crowns = 0;

    for(unsigned int i=0; i<_levels.size(); ++i)
    {
        Level* level = _levels[i];
        Level::LevelState st = level->getLevelState();
        if(st == Level::Crown)
        {
            crowns += 1;
        }
        stamps += level->getFoundSolutions().size();
    }
    _crowns_obtained = crowns;
    _stamps_obtained = stamps;
}
