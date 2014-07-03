#include "RW.h"
#include <cassert>
#include "SavesManager.h"
#include <ADLib/Device/ADSoundManager.h>
#include "Math/Equation.h"
#include <sstream>

#ifndef RW_DEBUG
#include "cocos2d.h"
using namespace cocos2d;
#endif


#include <fstream>
#include <ctime>
static const uint32_t ADS_MARK = 0xAD001010;

RW* RW::_rw = 0;
#ifdef RW_BUILD_LEVELS
void RW::prepareForLevelBuild()
{
    _rw = new RW();
}
#endif
void RW::init()
{
    //Init can be called only once
    assert(_rw == 0);

    _rw = new RW();

#ifdef RW_TEST_INPUT
    std::ifstream input("../../Resources/levels.ad", std::ios::in | std::ios::binary);
    ADStreamIn is(input);
    readLevelsInformation(is);
#endif
}

Level* RW::getNextLevel(Level* l)
{
    unsigned int next_id = l->getLevelID()+1;
    Collection* col = l->getLevelCollection();
    if(next_id < col->getLevels().size())
    {
        Level* next_level = col->getLevels()[next_id];
        return next_level;
    }
    return 0;
}
Level* RW::skipLevel(Level* l)
{
    unlockNextLevel(l, true);
    return getNextLevel(l);
}

bool RW::unlockCollection(Collection* col)
{
    if(_rw)
    {
        if(col->_state != Collection::Locked)
            return false;
        else
        {
            if(RW::allStampsObtained() >= col->stampsToUnlock())
            {
                col->_state = Collection::Unlocked;

                //MANAGER unlock collection in saves
                SavesManager::getInstance()->unlockCollection(col->getCollectionID());
                return true;
            }
        }
    }
    return false;
}

void RW::unlockNextLevel(Level* l, bool flush)
{
    if(_rw)
    {
        Level* next = getNextLevel(l);
        if(next && next->_state == Level::Locked)
        {

            next->_state = Level::NoStamps;

            //MANAGER unlock in saves
            SavesManager::getInstance()->unlockLevel(next->getLevelCollection()->getCollectionID(),
                                                     next->getLevelID());
            if(flush)
                saveGame();
        }
    }
}
bool RW::isAdsDisabled()
{
    //MANAGER
    return (!SavesManager::getInstance()->isAds());
}


bool RW::isFullVersion()
{
    return (SavesManager::getInstance()->isFullVersion());
}

bool RW::registerSolution(Level* l, const Solution& s)
{
    if(_rw)
    {
        assert(l->hasSolution(s));

        if(l->isSolutionFound(s))
            return false;
        else
        {
            //Add solution
            l->_found_solutions.push_back(s);

            //MANAGER set to storage new found solutions
            SavesManager::getInstance()->levelSolutionChanged(l->getLevelCollection()->getCollectionID(),
                                                              l->getLevelID(),
                                                              l->getFoundSolutions());


            //Update stamps counts
            l->updateStampsStatus();
            l->getLevelCollection()->updateStampsAndCrownsCount();
            RW::updateStampsAndCrownsCount();

            //unlock next (we do not need to save to disk
            unlockNextLevel(l, false);

            //Write to disk that we found solution
            saveGame();
            return true;
        }
    }
    return false;
}



void RW::release()
{
    if(_rw)
    {
        //saveGame();
        //release all objects
    }
}

/**
 * @brief RW::loadGame
 * read collection saves
 */
void RW::loadGame()
{
    if(_rw)
    {

        //Read all collections
        for(auto& i : _rw->_collections)
        {
            Collection* a = i.second;
            readCollectionInfo(a);
            a->updateStampsAndCrownsCount();

        }
    }

}
bool RW::isExpertMode()
{
    if(_rw)
    {
        return _rw->_expert_mode;
    }
    return false;
}

void RW::setExpertMode(bool expert)
{
    if(_rw)
    {
        _rw->_expert_mode = expert;
        //MANAGER
        SavesManager::getInstance()->setExpertMode(expert);
    }
}
void RW::buyFullVersion()
{
    SavesManager::getInstance()->setFullVersion(true);

    //set collections open
    if(_rw)
    {
        for(auto& i : _rw->_collections)
        {
            Collection* a = i.second;
            a->_state = Collection::Unlocked;

            //MANAGER
            SavesManager::getInstance()->unlockCollection(a->getCollectionID());

        }
    }
}

/**
 * @brief RW::readCollectionInfo
 * @param Collection collection
 *  read one collection saves
 */
void RW::readCollectionInfo(Collection* a)
{
    if(_rw)
    {
        //get status from MANGER(saves)
        bool has_status = SavesManager::getInstance()->hasCollectionStateValue(a->getCollectionID());
        if(has_status)
            a->_state = SavesManager::getInstance()->getCollectionState(a->getCollectionID());

        for(unsigned int i=0; i<a->_levels.size();++i)
        {
            Level* l = a->_levels[i];
            bool has_level_status = SavesManager::getInstance()->hasLevelStateValue(a->getCollectionID(),l->getLevelID());
            if(has_level_status)
                l->_state = (SavesManager::getInstance()->getLevelState(a->getCollectionID(),l->getLevelID()));


            //read found solutions
            bool has_solutions = SavesManager::getInstance()->hasSolutions(a->getCollectionID(),l->getLevelID());
            if(has_solutions)
            {
                l->_found_solutions = SavesManager::getInstance()->getSolutions(a->getCollectionID(),l->getLevelID());
            }

            //Get new stamp status
            //TODO: I comment it, because it gives NoStamps status at the begin
            //l->updateStampsStatus();

        }
    }
}

//save new collection info
void RW::flushCollectionInfo(Collection* a/*, ADStreamOut& os*/)
{
    if(_rw)
    {
        //MANAGER: save info
        SavesManager::getInstance()->unlockCollection(a->getCollectionID());

        for(unsigned int i=0; i<a->_levels.size(); ++i)
        {
            Level* l = a->_levels[i];

            if(l->getLevelState() != Level::Locked)
            {
                SavesManager::getInstance()->updateLevelState(a->getCollectionID(),
                                                              l->getLevelID(),
                                                              l->getLevelState());

                SavesManager::getInstance()->levelSolutionChanged(a->getCollectionID(),
                                                                  l->getLevelID(),
                                                                  l->getFoundSolutions());

            }
        }
    }

}

//save all collection info
void RW::saveGame()
{
    if(_rw)
    {
        flushSettings();

        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;

            if(a->getCollectionState() == Collection::Unlocked)
            {

                flushCollectionInfo(a);
            }
        }

    }
}

//delete saves
void RW::deletePersistentInfo()
{
    if(_rw)
    {
        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            if(a->_state == Collection::Unlocked)
            {
                a->_state = Collection::Locked;
                SavesManager::getInstance()->updateCollectionState(a->getCollectionID(),
                                                                   Collection::Locked);
            }
        }
        readSavedData();
        saveGame();
    }
}

/**
 * @brief RW::readSavedData
 *firstly fill auto data
 *then read from saves(ADStorage)
 */
void RW::readSavedData()
{
    if(_rw)
    {
#ifndef RW_DEBUG
        readSettings();

        readHints();
        //unsigned int stamps_max = 0;
        //Fill by the default info
        for(CollectionsArr::iterator it=_rw->_collections.begin();
            it!=_rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            //stamps_max += a->stampsMax();

            //open first two collections
            if(a->getCollectionID()==100 || a->getCollectionID()==200)
                a->_state = Collection::Unlocked;
            else
                a->_state = Collection::Locked;


            a->_stamps_obtained = 0;
            a->_crowns_obtained = 0;
            a->_max_stamps = 0;
            for(unsigned int j=0; j<a->_levels.size(); ++j)
            {
                Level* l = a->_levels[j];
                if(l->getLevelID() == 0)
                    l->_state = Level::NoStamps;
                else
                    l->_state = Level::Locked;
                l->_found_solutions.resize(0);
                a->_max_stamps += l->getSolutions().size();
            }
        }
        //_rw->_stamps_max = stamps_max;

        //Read saved info
        loadGame();



        updateStampsAndCrownsCount();

#endif
    }
}

#include <ctime>
#include "GameInfo.h"
void RW::readHints()
{
    bool emit_dayly_hints = false;
    time_t curtime = time(0);
    time_t last_add_time = SavesManager::getInstance()->getHintTime();


        if(curtime > last_add_time)
        {
            const time_t day = 3600 * 24;
            if(curtime > last_add_time + day)
            {
                emit_dayly_hints = true;
            }
            else
            {
                time_t curt = curtime;
                time_t lastt = last_add_time;
                struct tm * cur_info = localtime ( &curt );
                unsigned int cur_day = cur_info->tm_mday;

                struct tm * last_info = localtime ( &lastt );
                unsigned int last_day = last_info->tm_mday;

                if(cur_day != last_day)
                    emit_dayly_hints = true;

            }
        }
        if(emit_dayly_hints)
        {
            SavesManager::getInstance()->setTimeForHints(curtime);
            SavesManager::getInstance()->addHint(GameInfo::HINT_ADD);

            _rw->_hints_count = SavesManager::getInstance()->getHintCount();
        }

}

void RW::readSettings()
{
#ifndef RW_DEBUG
    if(_rw)
    {
        bool expert_mode = SavesManager::getInstance()->isExpertModeOn();
        bool music_on = SavesManager::getInstance()->isMusicOn();
        bool sounds_on = SavesManager::getInstance()->isSoundOn();


//            is >> unlock_all >> buy_all >> hints;
//            _rw->_unlock_all_purchased = (unlock_all == 1);
//            _rw->_buy_all_purchased = (buy_all == 1);

            _rw->_hints_count = SavesManager::getInstance()->getHintCount();

            _rw->_ads_disabled = !SavesManager::getInstance()->isAds();

        RW::setExpertMode(expert_mode);
        if(music_on)
            ADSoundManager::turnOnMusic();
        else
            ADSoundManager::turnOffMusic();

        if(sounds_on)
            ADSoundManager::turnOnSound();
        else
            ADSoundManager::turnOffSound();
    }
#endif
}

//save settings
void RW::flushSettings()
{
#ifndef RW_DEBUG
    if(_rw)
    {
        bool expert_mode = RW::isExpertMode();
        bool music_on = ADSoundManager::isMusicTurnedOn();
        bool sounds_on = ADSoundManager::isSoundTurnedOn();

        SavesManager::getInstance()->setExpertMode(expert_mode);
        SavesManager::getInstance()->setMusic(music_on);
        SavesManager::getInstance()->setSound(sounds_on);
        SavesManager::getInstance()->setFullVersion(!_rw->_ads_disabled);
        SavesManager::getInstance()->setHint(_rw->_hints_count);
    }
#endif
}

const CollectionsArr &RW::getCollections()
{
    assert(_rw);
    return _rw->_collections;
}

unsigned int RW::allStampsObtained()
{
    assert(_rw);
    return _rw->_stamps_obtained;
}

unsigned int RW::allCrownsObtained()
{
    assert(_rw);
    return _rw->_crowns_obtained;
}

ADStreamOut& operator<<(ADStreamOut& os, const cocos2d::ccColor3B& color)
{
    uint16_t r = color.r;
    uint16_t g = color.g;
    uint16_t b = color.b;

    os << r << g << b;
    return os;
}

ADStreamIn& operator>>(ADStreamIn& is, cocos2d::ccColor3B& color)
{
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;
    is >> r >> g >> b;
    typedef unsigned char Type;
    color = ccc3((Type)r,(Type)g,(Type)b);
    return is;
}

/**
 * @brief RW::writeLevelsInformation
 * @param os
 * use during AD file creation
 */
void RW::writeLevelsInformation(ADStreamOut& os)
{
    if(_rw)
    {
        uint32_t coll_size = static_cast<uint32_t>(_rw->_collections.size());
        os << _rw->_levels_mark << coll_size;

        for(CollectionsArr::iterator it=_rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            uint16_t free = a->_collection_free ? 1 : 0;
            uint16_t difficulty = a->_collection_difficulty;
            os << uint32_t(a->_collection_id) << free << difficulty << a->_collection_color << a->_stamps_to_unlock;

            uint32_t level_number = static_cast<uint32_t>(a->_levels.size());
            os << level_number;

            for(unsigned int lev_id=0; lev_id<level_number; ++lev_id)
            {
                Level* l = a->_levels[lev_id];
                //std::string test =l->_equation->getUntouchedLhsString();
                os << l->_equation->getUntouchedLhsString() << l->_equation->getUntouchedRhsString();
                os << l->_existing_solutions;
            }
        }
    }
}

/**
 * @brief RW::readLevelsInformation
 * @param stream - file strem
 *  read levels from file
 *  it is static, will not be changed
 * (without saves)
 */
void RW::readLevelsInformation(ADStreamIn& is)
{
    if(_rw)
    {

        unsigned short int control_mark = 0;
        is >> control_mark;
        if(control_mark != _levels_mark)
            is.setError();
        else
        {
            unsigned int collection_size = 0;
            is >> collection_size;


            //_rw->_collections.resize(0);
            //_rw->_collections.reserve(collection_size);

            for(unsigned int col_id=0;
                col_id < collection_size && is.isOK();
                ++col_id)
            {
                Collection* a = new Collection;


                uint32_t id = 0;
                is >> id;
                a->_collection_id = id;
                _rw->_collections[id] = a;

                uint16_t free = 0;
                is >> free;
                a->_collection_free = (free == 1);

                if(a->_collection_free)
                    a->_state = Collection::Locked;
                else
                    a->_state = Collection::InShop;

                uint16_t difficulty = Collection::Easy;
                is >> difficulty;

                a->_collection_difficulty = Collection::Difficulty(difficulty);

                is >> a->_collection_color >> a->_stamps_to_unlock;



                unsigned int levels_number = 0;
                is >> levels_number;

                a->_levels.resize(0);
                a->_levels.reserve(levels_number);

                for(unsigned int lev_id=0; lev_id < levels_number; ++lev_id)
                {
                    Level* l = new Level;
                    a->_levels.push_back(l);
                    l->_parent = a;
                    l->_level_id = lev_id;

                    std::string lhs="";
                    std::string rhs="";
                    is >> lhs >> rhs >> l->_existing_solutions;

                    l->_equation = new Equation(lhs,rhs);
                }

            }

        }

    }
}
void RW::updateStampsAndCrownsCount()
{
    if(_rw)
    {
        unsigned int stamps = 0;
        unsigned int crowns = 0;

        for(CollectionsArr::iterator it=_rw->_collections.begin();
            it!=_rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            stamps += a->stampsObtained();
            crowns += a->crownsObtained();
        }
        _rw->_crowns_obtained = crowns;
        _rw->_stamps_obtained = stamps;
    }
}
int64_t RW::getHintCount()
{
   int64_t res =  SavesManager::getInstance()->getHintCount();
    _rw->_hints_count =res;
   return _rw->_hints_count;
}
void RW::addHints(unsigned int to_add)
{
    _rw->_hints_count += to_add;
    SavesManager::getInstance()->addHint(to_add);

    flushSettings();
}

//buy full version
void RW::unlockAllCollectionsPurchased()
{
    if(_rw)
    {
        for(CollectionsArr::iterator it = _rw->_collections.begin();
            it != _rw->_collections.end(); ++it)
        {
            Collection* a = it->second;
            if(a->getCollectionState() == Collection::Locked)
            {
                a->_state = Collection::Unlocked;
            }
        }
        //_rw->_unlock_all_purchased = true;

        saveGame();

    }
}
void RW::disableAds()
{
    _rw->_ads_disabled = true;
    flushSettings();
}

void RW::useOneHint()
{
    assert(_rw->_hints_count > 0);
    if(_rw->_hints_count > 0)
    {
        _rw->_hints_count--;
        SavesManager::getInstance()->minusHint();
        flushSettings();
    }
}

RW::RW()
    : _expert_mode(SavesManager::getInstance()->isExpertModeOn()),
      _hints_count(0),
      _ads_disabled(!SavesManager::getInstance()->isAds())
{
}
