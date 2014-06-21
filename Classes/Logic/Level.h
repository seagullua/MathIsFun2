#ifndef LEVEL_H
#define LEVEL_H

class RW;
class Collection;
class Equation;
#include "Math/Solution.h"
#include "Stamp.h"

struct StampsColors
{
    StampsColors(unsigned int blue=0, unsigned int green=0, unsigned int red=0)
        : _blue(blue), _green(green), _red(red)
    {

    }

    unsigned int _blue;
    unsigned int _green;
    unsigned int _red;

    static const StampsColors& forSolutions(const unsigned int& max);

private:

    static std::vector<StampsColors> _cache;
};

typedef std::vector<Stamp> Stamps;
class Level
{
public:
    typedef unsigned int LevelID;
    /**
     * @brief The state of the level.
     */
    enum LevelState
    {
        //Level is locked
        Locked=0,
        //Level is unlocked but has no stamps
        NoStamps=1,
        //Level has one stamp
        OneStamp=2,
        //Level has two stamps
        TwoStamps=3,
        //Level has three stamps
        ThreeStamps=4,
        //Level has crown
        Crown=5
    };

    /**
     * @brief Get the status of current level
     * (locked, unlocked, how many stamps
     * @return
     */
    const LevelState getLevelState() const;

    /**
     * @brief Get the pointer to the level's collection
     * @return
     */
    Collection* getLevelCollection() const;

    /**
     * @brief Get the level's number in collection (starts from 0)
     * For example Level 5 will have LevelID == 4
     * @return
     */
    const LevelID getLevelID() const;

    /**
     * @brief Returns the equation
     * @return
     */
    const Equation& getEquation() const;

    /**
     * @brief Returns the array of found solutions by the user
     * @return
     */
    const Solutions& getFoundSolutions() const;

    /**
     * @brief Returns all possible solutions which this levele has
     * @return
     */
    const Solutions& getSolutions() const;

    const bool hasSolution(const Solution& sol) const;
    const bool isSolutionFound(const Solution& sol) const;

    const Stamps getLevelStamps() const;
    /*const unsigned int getLevelMaxStamps() const
    {
        //TODO: fix it
        return 7;
    }*/

    ~Level();
private:
    friend class RW;
#ifdef RW_BUILD_LEVELS
public:
#endif
    Level();
    Level(const Level&);
    Level& operator=(const Level&);

    LevelState _state;
    LevelID _level_id;
    Collection* _parent;

    Equation* _equation;
    Solutions _existing_solutions;
    Solutions _found_solutions;

    //Stamps _level_stamps;
    static const StampsColors _avaliable_colors;
    void updateStampsStatus();
};


#include <ADLib/Device/ADStatistics.h>
ADStatistics::Params levelToStatisticsParams(Level* level);

#endif // LEVEL_H
