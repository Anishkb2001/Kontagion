#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>
using namespace std;
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor:public GraphObject{
public:
 Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s);
    virtual void doSomething()=0;
    bool status() const;
    void alterstatus();
    virtual bool overlap()=0;
    virtual bool blockable()=0;
    virtual ~Actor();
    StudentWorld* stu();
    string actorType_ret() const;
    void set_type(string act);
    virtual bool isDamageable()=0;
    virtual bool flame_damage()=0;
    virtual bool spray_damage()=0;
private:
    string actorType;
    bool alive;
    StudentWorld *m_stu;
};

class Socrates:public Actor{
public:
    Socrates(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* sw);
    virtual void doSomething();
    virtual ~Socrates();
    int get_spray() const;
    int get_flame() const;
    int get_hit() const;
    void change_spray(int x);
    void change_flame(int x);
    void change_hit(int x);
    void addFungus();
    void addGoodie();
    virtual bool blockable();
    virtual bool overlap(){
        return true;
    }
    virtual bool isDamageable();
    virtual bool flame_damage();
    virtual bool spray_damage();
private:
    int spray_charges;
    int flame_charges;
    int hit_points;
    int positional_angle;
};

class Dirt:public Actor{
public:
    Dirt(int imageID,double startX, double startY, int dir, double size,int depth, StudentWorld *D);
    virtual void doSomething();
    virtual bool isDamageable();
    virtual ~Dirt();
    virtual bool overlap(){
        return true;
    }
    virtual bool blockable();
    virtual bool flame_damage();
    virtual bool spray_damage();
private:
};

class Food:public Actor{
public:
    Food(int imageID,double startX, double startY, int dir, double size,int depth, StudentWorld *D);
    virtual void doSomething();
    virtual bool isDamageable();
    virtual ~Food();
    virtual bool overlap(){
        return true;
    }
    virtual bool blockable();
    virtual bool flame_damage();
    virtual bool spray_damage();

    
};

class Projectiles:public Actor{
public:
    Projectiles(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld *P);
    virtual void doSomething();
    void move_projectile();
    int getDistance() const;
    virtual bool isDamageable();
    virtual bool overlap();
    virtual bool blockable();
    virtual void move_projectile_different()=0;
    virtual bool flame_damage();
    virtual bool spray_damage();
private:
    void setDistance(int x);
    int distance;

};


class Flame: public Projectiles{
public:
    Flame(int imageID,double startX, double startY, int dir, double size,int depth, StudentWorld *D);
    virtual ~Flame();
    virtual void move_projectile_different();    
};

class Spray: public Projectiles{
public:
    Spray(int imageID,double startX, double startY, int dir, double size,int depth, StudentWorld *D);
    virtual void move_projectile_different();
    virtual ~Spray();
};

class Goodies:public Actor{
public:
    Goodies(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual void doSomething();
    virtual ~Goodies();
    virtual bool isDamageable();
    virtual bool overlap();
    virtual void doDifferent()=0;
    virtual bool blockable();
    virtual bool flame_damage();
    virtual bool spray_damage();
private:
    int g_life_ret() const;
    int max_life_ret() const;
    void inc_glife();
    int g_life;
    int max_life;
};

class FT_goodie:public Goodies{
public:
    FT_goodie(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual void doDifferent();
    virtual ~FT_goodie();

};

class EL_goodie:public Goodies{
public:
    EL_goodie(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~EL_goodie();
    virtual void doDifferent();
};

class RH_goodie:public Goodies{
public:
    RH_goodie(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~RH_goodie();
    virtual void doDifferent();
};

class Fungus:public Goodies{
public:
    Fungus(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~Fungus();
    virtual void doDifferent();
};

class Pit:public Actor{
public:
    Pit(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~Pit();
    virtual void doSomething();
    virtual bool blockable();
    virtual  bool isDamageable();
    virtual bool overlap(){
        return true;
    }
    virtual bool flame_damage();
    virtual bool spray_damage();
private:
    int get_reg() const;
    int get_agg() const;
    int get_coli() const;
    void dec_reg();
    void dec_agg();
    void dec_coli();
    int number;
    int reg_sal;
    int agg_sal;
    int e_coli;
    };

class Bacteria:public Actor{
public:
    Bacteria(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~Bacteria();
    virtual bool blockable();
    virtual bool isDamageable();
    int get_hit_points();
    void set_hit_points(int x);
    int get_moveDistance();
    void set_moveDistance(int x);
    virtual bool overlap(){
        return true;
    }
    bool overlap_player();
    bool overlap_food();
    int get_foodCount();
    void set_foodCount(int x);
    bool checkCoordinates(double x,double y);
    virtual bool flame_damage();
    virtual bool spray_damage();
    virtual void playsound()=0;
    virtual void doSomething();
    virtual void doDifferent()=0;
    virtual void create(double newx, double newy)=0;
    void bact_create();
    void food_create();
private:

    int hit_points;
    int food_count;
    int movement_planDistance;
    
};
class Salmonella:public Bacteria{
public:
    Salmonella(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~Salmonella();
    virtual void playsound();
    virtual void doDifferent();
    bool contemplateMove(int angle,double x, double y);
    virtual bool doDifferently()=0;
};


class Reg_Sal:public Salmonella{
public:
    Reg_Sal(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~Reg_Sal();
    virtual bool doDifferently();
    virtual void create(double newx, double newy);
};

class Agg_Sal:public Salmonella{
public:
    Agg_Sal(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~Agg_Sal();
    virtual bool doDifferently();
    virtual void create(double newx, double newy);
private:
        bool checkSocrates();
};

class E_Coli:public Bacteria{
public:
    E_Coli(int imageID,double startX,double startY,int dir, double size,int depth, StudentWorld *G);
    virtual ~E_Coli();
    virtual void playsound();
    virtual void doDifferent();
    virtual void create(double newx, double newy);
private:
    bool contemplateMove(int angle,double x, double y);
    bool checkSocrates();
};

#endif // ACTOR_H_
