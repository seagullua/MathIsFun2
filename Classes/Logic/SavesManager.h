#ifndef SAVESMANAGER_H
#define SAVESMANAGER_H

#include "cocos2d.h"
#include <ADLib/ADSignals.h>
#include <ADLib/Storage.h>
#include "Collection.h"
#include "Level.h"

class SavesManager: public HasSlots
{
    static const int BLOCK_SOLUTIONS = 10001;

    static const int BLOCK_LEVELS = 20001;
    static const int BLOCK_COLLECTIONS = 30001;

    static const int BLOCK_HINTS = 40001;

    static const int BLOCK_MUSIC = 50001;
    static const int BLOCK_SOUND = 60001;
    static const int BLOCK_EXPERT_MODE = 70001;
    static const int BLOCK_FULL_VERSION = 80001;

public:
    static SavesManager* getInstance();

    void levelSolutionChanged(Collection::CollectionID id_collection,
                                            Level::LevelID id_level,
                                            Solutions solutions);
    const int64_t getHintCount();


    bool isFullVersion();
    bool isMusicOn();
    bool isSoundOn();
    bool isExpertModeOn();
    bool isAds();

    void setMusic(bool music_mode);
    void setSound(bool sound_mode);
    void setExpertMode(bool expert_mode);
    void setFullVersion(bool full_version);
    void minusHint();
    void addHint(int num=1);

    void unlockCollection(Collection::CollectionID id);
    void unlockLevel(Collection::CollectionID c_id,
                     Level::LevelID l_id);

    void updateLevelState(Collection::CollectionID c_id,
                          Level::LevelID,
                          Level::LevelState);
    void updateCollectionState(Collection::CollectionID c_id,
                          Collection::CollectionState);

    //bool
    bool hasSolutions(Collection::CollectionID c_id,
                      Level::LevelID l_id);
    bool hasCollectionStateValue(Collection::CollectionID id);
    bool hasLevelStateValue(Collection::CollectionID c_id,
                            Level::LevelID l_id);

    //get-ers
    Collection::CollectionState getCollectionState(Collection::CollectionID id);
    Level::LevelState getLevelState(Collection::CollectionID c_id,
                                    Level::LevelID l_id);
    Solutions getSolutions(Collection::CollectionID c_id,
                           Level::LevelID l_id);

private:

    void initStorageBlocks();
    void initDefaultValues();

    int64_t getLevelID(Collection::CollectionID id_collection,
                       Level::LevelID id_level);

    ADStorageListener _storage;
    static SavesManager instance;

    SavesManager();
    SavesManager(SavesManager const&);
    void operator=(SavesManager const&);

    //data
    bool _music_on;

    //id of level+collection and the collection data
    //std::map<int64_t, Level> _levels;
};

#endif // SAVESMANAGER_H

