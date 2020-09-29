#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    bact_number=10;
    bacts_killed=0;
}

int StudentWorld::init()
{   int L=getLevel();
    level_complete=false;
    bacts_killed=0;
    bact_number=10*L;
    s_player = new Socrates(IID_PLAYER, 0, 128, 0, 1, 0, this);
    int x=max(180-20*L,20);
    for(int i=0;i<x;i++){
        x:
        int p1=randInt(8, 248 );
        int p2=randInt(8, 248);
        if(checkBounds(p1,p2))
            v.push_back(new Dirt(IID_DIRT,p1,p2,0,1,1,this));
        else
            goto x;
    }
    int y=min(5*L,25);
    for(int i=0;i<y;i++){
        z:
        int p1=randInt(8, 248 );
        int p2=randInt(8, 248);
        if(checkBounds(p1,p2))
            v.push_back(new Food(IID_FOOD,p1,p2,90,1,1,this));
        else
            goto z;
    }
    for(int i=0;i<L;i++){
        p:
        int p1=randInt(8, 248 );
        int p2=randInt(8, 248);
        if(checkBounds(p1,p2))
            v.push_back(new Pit(IID_PIT,p1,p2,0,1,1,this));
        else
            goto p;
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    s_player->doSomething();
    level_complete=false;;
    for(int i=0;i<v.size();i++){
        if(v[i]->status()){
            v[i]->doSomething();
            if(!(s_player->status())){
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(bacts_killed>=bact_number){
                    level_complete=true;
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    objectRemove();
    addobjects();
    updateStats();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete s_player;
    s_player=NULL;
    vector<Actor*>::iterator it=v.begin();
    for(;it!=v.end();){
        delete(*it);
        it=v.erase(it);
    }
    v.clear();
}

Socrates* StudentWorld::player() const
{
    return s_player;
}

bool StudentWorld::checkOuter_bounds(double x,double y){
    int z=sqrt(pow((x-VIEW_RADIUS),2)+pow((y-VIEW_RADIUS),2));
    if(z==VIEW_RADIUS)
       return true;
    return false;
}

bool StudentWorld::checkBounds(double x, double y){
    if(sqrt(pow((x-VIEW_RADIUS),2)+pow((y-VIEW_RADIUS),2))<=120)
       return true;
    return false;
}


bool StudentWorld::Levelcheck() const{
    return level_complete;
}

void StudentWorld::objectRemove(){
    vector<Actor*>::iterator it=v.begin();
    for(;it!=v.end();){
       if((*it)->status())
           it++;
       else{
            delete *it;
            it = v.erase(it);
       }
    }
}
void StudentWorld::updateStats(){
    ostringstream oss;
    oss<<setw(5)<<"Score: "<<getScore()<<"  ";
    oss<<setw(2)<<"Level: "<<getLevel()<<"  ";
    oss<<setw(2)<<"Lives: "<<getLives()<<"  ";
    oss<<setw(2)<<"Health: "<<s_player->get_hit()<<"  ";
    oss<<setw(2)<<"Sprays: "<<s_player->get_spray()<<"  ";
    oss<<setw(2)<<"Flames: "<<s_player->get_flame()<<"  ";
    string text=oss.str();
    setGameStatText(text);
}

void StudentWorld::addobjects(){
    s_player->addFungus();
    s_player->addGoodie();
}


Actor* StudentWorld::overlap_checker(Actor *a){
vector<Actor*>::iterator it=v.begin();
 double aX = a->getX();
 double aY = a->getY();
 for (;it!=v.end();it++){

     double bX = (*it)->getX();
     double bY = (*it)->getY();
     double x = aX - bX;
     double y = aY - bY;
     double dist = sqrt(pow(x, 2) + pow(y, 2));

     if ((dist <= SPRITE_RADIUS*2) && (*it)->isDamageable() && (*it)->status())
            return (*it);
    }
 return nullptr;
}

Actor* StudentWorld::overlap_Food(Actor *a){
vector<Actor*>::iterator it=v.begin();
 double aX = a->getX();
 double aY = a->getY();
 for (;it!=v.end();it++){
     double bX = (*it)->getX();
     double bY = (*it)->getY();
     double x = aX - bX;
     double y = aY - bY;
     double dist = sqrt(pow(x, 2) + pow(y, 2));
     if ((dist <= SPRITE_RADIUS*2)&& (*it)->actorType_ret()=="Food"){
            return (*it);
     }
}
 return nullptr;
}


int StudentWorld::getbact_no(){
    return bact_number;
}

void StudentWorld::change_kill(){
    bacts_killed++;
}

void StudentWorld::change_bact(int x){
    bact_number=x;
}

bool StudentWorld::blocker2(double aX,double aY){
    vector<Actor*>::iterator it=v.begin();
   
    for (;it!=v.end();it++){
        double bX = (*it)->getX();
        double bY = (*it)->getY();
        double x = aX - bX;
        double y = aY - bY;
        double dist = sqrt(pow(x, 2) + pow(y, 2));
             if (dist<=SPRITE_WIDTH/2 && (*it)->blockable())
                 return true;
    }
    return false;
 
}

Actor* StudentWorld::food_finder(Actor *a){
    vector<Actor*>::iterator it=v.begin();
    double aX = a->getX();
    double aY = a->getY();
    double dist=10000;
    Actor *nearFood=nullptr;
    for (;it!=v.end();it++){
        double bX = (*it)->getX();
        double bY = (*it)->getY();
        double x = aX - bX;
        double y = aY - bY;
        double tempdist = sqrt(pow(x, 2) + pow(y, 2));
        if(tempdist<dist && (*it)->actorType_ret()=="Food"){
            dist=tempdist;
            nearFood=(*it);
        }
    }
if(dist<128 && nearFood->actorType_ret()=="Food"){
        return nearFood;
    }
    return nullptr;
}

void StudentWorld::addVector(Actor *a){
    v.push_back(a);
}
StudentWorld::~StudentWorld(){
    this->cleanUp();
}
