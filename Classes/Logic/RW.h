#ifndef RW_H
#define RW_H
#include "Collection.h"
#include "Level.h"
#include <map>
typedef std::map<Collection::CollectionID, Collection*> CollectionsArr;
class Loading;
class RW
{
public:
    static const CollectionsArr& getCollections();
    static unsigned int allStampsObtained(); 
    static unsigned int allCrownsObtained();


    /**
     * @brief Returns 0 if the given Level is the last in collection
     * @param Level l
     * @return
     */
    static Level* getNextLevel(Level* l);
    static Level* skipLevel(Level* l);

    //Returns true if this is new solution and false otherwise
    static bool registerSolution(Level* l, const Solution&);
    static bool unlockCollection(Collection* col);

    //delete saves
    static void deletePersistentInfo();


    static bool isExpertMode();
    static bool isAdsDisabled();
    static bool isFullVersion();


    static void setExpertMode(bool expert);
    static void buyFullVersion();

    //save settings
    static void flushSettings();

    static int64_t getHintCount();
    static void useOneHint();
    static void addHints(unsigned int to_add);


    static void saveGame();
    static void unlockAllCollectionsPurchased();
    static void buyAllCollectionsPurchased();
    static void buyCollection(const Collection::CollectionID id);
    static void disableAds();


private:
    friend class AppDelegate;
	friend class Loading;

    static void init();
    static void release();

    //read saves
    static void readSavedData();
    static void readSettings();

    //write and read levels
    static void writeLevelsInformation(ADStreamOut&);
    static void readLevelsInformation(ADStreamIn&);


    static RW* _rw;
    RW();
    bool _expert_mode;
    bool _unlock_all_purchased;
    bool _buy_all_purchased;
    bool _ads_disabled;
    unsigned int _stamps_obtained;
    unsigned int _crowns_obtained;
    CollectionsArr _collections;
    int64_t _hints_count;
    static const uint16_t _levels_mark = 0xAABD;


    static void fillCollectionWithLevels(Collection*);
    //static void parseSolutions(Level* l, const std::string& solutions);
//#ifdef RW_BUILD_LEVELS
//    static void prepareForLevelBuild();
//#endif

private:
    static void updateStampsAndCrownsCount();
    static void saveLevelSolutions(Level* l);
    static void unlockNextLevel(Level* l, bool flush=true);

    //save one collection info
    static void flushCollectionInfo(Collection *a);
    static void readCollectionInfo(Collection* a);
    static void loadGame();

};

#endif // RW_H
