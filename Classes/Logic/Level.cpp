#include "Level.h"
#include <cassert>
#include <algorithm>
#include "Math/Equation.h"
#include "Collection.h"
std::vector<StampsColors> fillStampsNeeded()
{
    std::vector<StampsColors> cache(11);
    cache[0] = StampsColors(0,0,0);
    cache[1] = StampsColors(0,0,1);
    cache[2] = StampsColors(0,1,1);
    cache[3] = StampsColors(1,1,1);
    cache[4] = StampsColors(1,1,2);
    cache[5] = StampsColors(1,2,2);
    cache[6] = StampsColors(2,2,2);
    cache[7] = StampsColors(2,2,3);
    cache[8] = StampsColors(2,3,3);
    cache[9] = StampsColors(3,3,3);
    cache[10] = StampsColors(3,3,4);
    return cache;
}
const StampsColors& StampsColors::forSolutions(const unsigned int& max)
{
    assert(max < _cache.size());
    return _cache[max];
}
std::vector<StampsColors> StampsColors::_cache = fillStampsNeeded();

Level::Level()
    : _state(Level::NoStamps), _level_id(0), _parent(0),
      _equation(0), _existing_solutions(0), _found_solutions(0)
{
}
const StampsColors Level::_avaliable_colors = StampsColors(6,5,3);
const Level::LevelState Level::getLevelState() const
{
    return _state;
}

Collection* Level::getLevelCollection() const
{
    return _parent;
}
const bool Level::hasSolution(const Solution& sol) const
{
    if(sol.getSubstitutions().size() != _equation->substitutionsNumber())
        return false;
    return std::find(_existing_solutions.begin(), _existing_solutions.end(),
                     sol) != _existing_solutions.end();
}
const bool Level::isSolutionFound(const Solution& sol) const
{
    return std::find(_found_solutions.begin(), _found_solutions.end(),
                     sol) != _found_solutions.end();
}
const Level::LevelID Level::getLevelID() const
{
    return _level_id;
}

const Equation& Level::getEquation() const
{
    assert(_equation);
    return *_equation;
}
const Solutions& Level::getFoundSolutions() const
{
    return _found_solutions;
}
const Solutions& Level::getSolutions() const
{
    return _existing_solutions;
}
const Stamps Level::getLevelStamps() const
{
    unsigned int found_solutions = getFoundSolutions().size();
    unsigned int all_solutions = getSolutions().size();
    StampsColors colors_for_found = StampsColors::forSolutions(all_solutions);
    unsigned int give_green_after = colors_for_found._blue;
    unsigned int give_red_after = give_green_after + colors_for_found._green;

    Stamps st;
    st.reserve(found_solutions);
    srand((getLevelID()+1)*(getLevelCollection()->getCollectionID()+1)*1000);
    for(unsigned int i=0; i<found_solutions; ++i)
    {
        bool is_new = false;
        if((i+1) == found_solutions)
            is_new = true;

        if(i >= give_red_after)
        {
            //Red stamp
            st.push_back(Stamp(Stamp::Red,
                               rand() % _avaliable_colors._red,
                               rand() % 180 - 90,
                               is_new));
        }
        else if(i >= give_green_after)
        {
            //Green stamp
            st.push_back(Stamp(Stamp::Green,
                               rand() % _avaliable_colors._green,
                               rand() % 180 - 90,
                               is_new));
        }
        else
        {
            //Blue stamp
            st.push_back(Stamp(Stamp::Blue,
                               rand() % _avaliable_colors._green,
                               rand() % 180 - 90,
                               is_new));
        }
    }

    return st;
}
#ifndef RW_BUILD_LEVELS
#include "SavesManager.h"
void Level::updateStampsStatus()
{
    StampsColors needed = StampsColors::forSolutions(_existing_solutions.size());
    unsigned int found_solutions = _found_solutions.size();
    if(found_solutions == 0)
        _state = Level::NoStamps;
    else
    {
        _state = Level::OneStamp;
        if(found_solutions > needed._blue)
            _state = Level::TwoStamps;
        if(found_solutions > (needed._blue + needed._green))
            _state = Level::ThreeStamps;
        if(found_solutions == _existing_solutions.size())
            _state = Level::Crown;
    }

    //update level state in saves
    SavesManager::getInstance()->updateLevelState(this->getLevelCollection()->getCollectionID(),
                                                  this->getLevelID(),
                                                  this->getLevelState());
}
#endif
Level::~Level()
{}


ADStatistics::Params levelToStatisticsParams(Level* level)
{
    std::stringstream ss;
    ss << level->getLevelCollection()->getCollectionID() << "-" << (level->getLevelID()+1);
    return ADStatistics::Params()
            .add("Collection ID", level->getLevelCollection()->getCollectionID())
            .add("Level ID", level->getLevelID()+1)
            .add("Level number", ss.str())
            .add("Found solutions", level->getFoundSolutions().size());
}
