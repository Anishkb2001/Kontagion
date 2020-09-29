#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;
class Socrates;
class Actor;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Socrates *player() const;
    bool Levelcheck() const;
    void objectRemove();
    void updateStats();
    void addobjects();
    void addVector( Actor* a);
    Actor *overlap_checker(Actor* a);
    Actor *food_finder(Actor *a);
    Actor *overlap_Food(Actor *a);
    bool blocker2(double aX,double aY);
    int getbact_no();
    void change_bact(int x);
    void change_kill();
    bool checkBounds(double x, double y);
    bool checkOuter_bounds(double x,double y);
private:
    int bacts_killed;
    int bact_number;
    bool level_complete;
    Socrates *s_player;
    vector<Actor*> v;
};

#endif // STUDENTWORLD_H_
