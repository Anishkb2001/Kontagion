#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s)
:GraphObject(imageID, startX, startY, dir, depth, size){
    alive=true;
    m_stu=s;
    actorType="Actor";
}

StudentWorld* Actor::stu(){

    return m_stu;
}

bool Actor::status() const{
    return alive;
}

void Actor::alterstatus(){
alive=false;
}

void Actor::set_type(string act){
    actorType=act;
}

string Actor::actorType_ret() const{
    return actorType;
}

Socrates::Socrates(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Actor(imageID, startX, startY, dir, size, depth, s){
    hit_points=100;
    flame_charges=5;
    spray_charges=20;
    positional_angle=180;
    set_type("Soc");

}

bool Socrates::flame_damage(){
    return isDamageable();
}

bool Socrates::spray_damage(){
    return isDamageable();
}

void Socrates::doSomething(){
    int ch;
    if(!status())
        return;
    
    if(stu()->getKey(ch)){
        switch(ch){
            case KEY_PRESS_LEFT:{
                    positional_angle+=5;
                    moveTo((VIEW_RADIUS*cos((positional_angle*3.14)/180)+VIEW_RADIUS),((VIEW_RADIUS*sin((positional_angle*3.14)/180)+VIEW_RADIUS)));
                    setDirection((180+positional_angle)%360);
                break;
            }
            case KEY_PRESS_RIGHT:{
                    positional_angle-=5;
                moveTo((VIEW_RADIUS*cos((positional_angle*3.14)/180)+VIEW_RADIUS),((VIEW_RADIUS*sin((positional_angle*3.14)/180)+VIEW_RADIUS)));
                    setDirection((180+positional_angle)%360);
                break;
            }
            case KEY_PRESS_ENTER:{
                if(get_flame()>0){
                    for(int i=0;i<16;i++){
                        Flame *f=new Flame(IID_FLAME,getX()+(SPRITE_WIDTH*cos((i*22*3.14)/180)), getY()+(SPRITE_WIDTH*sin((i*22*3.14)/180)),i*22, 1, 1, stu());
                        stu()->addVector(f);
                    }
                    change_flame(get_flame()-1);
                    stu()->playSound(SOUND_PLAYER_FIRE);
                }
                else{
                    return;
                }
                break;
            }
            case KEY_PRESS_SPACE:{
                if(get_spray()>0){
                    Spray *sp=new Spray(IID_SPRAY,getX(),getY(),getDirection(),1,1,stu());
                    stu()->addVector(sp);
                    change_spray(get_spray()-1);
                    stu()->playSound(SOUND_PLAYER_SPRAY);
                }else{
                    return;
                }
                break;
            }
        }
    }
    else{
        if(get_spray()<20){
            change_spray(get_spray()+1);
        }
    }
    return;
}

bool Socrates::isDamageable(){
    return false;
}

bool Socrates::blockable(){
    return false;
}

void Socrates::addGoodie(){
    int chance=max(510-(stu()->getLevel()*10),250);
    int rand_chance=randInt(0, chance);
    if(rand_chance!=0){
        return;
    }
    int x=randInt(0,9);
    y:
    int angle=randInt(0, 355);
    int p1=randInt(0, 256);
    int p2=randInt(0, 256);
    if(!(stu()->checkOuter_bounds(p1, p2))){
        goto y;
    }
    if(x==0){
        EL_goodie* e_goodie=new EL_goodie(IID_EXTRA_LIFE_GOODIE,p1,p2,angle,1,1,stu());
        stu()->addVector(e_goodie);
    }
    if(x>=1 && x<=3){
       FT_goodie *f_goodie= new FT_goodie(IID_FLAME_THROWER_GOODIE,p1,p2,angle,1,1,stu());
        stu()->addVector(f_goodie);
    }
    if(x>3){
       RH_goodie *r_goodie= new RH_goodie(IID_RESTORE_HEALTH_GOODIE,p1,p2,angle,1,1,stu());
        stu()->addVector(r_goodie);
    }
}

void Socrates::addFungus(){
    int chance1=max(510-(stu()->getLevel()*10),200);
    int rand_chance1=randInt(0, chance1);
    if(rand_chance1!=0)
        return;
    z:
    int angle1=randInt(0, 355);
    int p3=randInt(0, 256);
    int p4=randInt(0, 256);
    if(!(stu()->checkOuter_bounds(p3, p4))){
        goto z;
    }
    Fungus *fung=new Fungus(IID_FUNGUS,p3,p4,angle1,1,1,stu());
    stu()->addVector(fung);

}

void Socrates::change_spray(int x){
    spray_charges=x;
}

void Socrates::change_hit(int x){
    if(x<=0){
        alterstatus();
        stu()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    if(x>100){
        hit_points=100;
        return;
    }
    hit_points=x;
}

void Socrates::change_flame(int x){
    if(x>20){
        flame_charges=20;
    }
    flame_charges=x;
}

int Socrates::get_hit() const{
    return hit_points;
}

int Socrates::get_flame() const{
    return flame_charges;
}

int Socrates::get_spray() const{
    return spray_charges;
}

Dirt::Dirt(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Actor(imageID, startX, startY, dir, size, depth, s){
    set_type("DIRT");
}

bool Dirt::flame_damage(){
    alterstatus();
    return isDamageable();
}

bool Dirt::spray_damage(){
    alterstatus();
    return isDamageable();
}

void Dirt::doSomething(){
    return;
}

bool Dirt::blockable(){
    return true;
}

bool Dirt::isDamageable(){
    return true;
}

Food::Food(int imageID,double startX, double startY, int dir, double size,int depth, StudentWorld *f):Actor(imageID, startX, startY, dir, size, depth, f){
    set_type("Food");
}

bool Food::flame_damage(){
    return isDamageable();
}

bool Food::spray_damage(){
    return isDamageable();
}

bool Food::blockable(){
    return false;
}


void Food::doSomething(){
    return;
}

bool Food::isDamageable(){
    return false;
}

Projectiles::Projectiles(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Actor(imageID, startX, startY, dir, size, depth, s){
   distance=0;
    set_type("Projectile");
}

bool Projectiles::blockable(){
    return false;
}

bool Projectiles::flame_damage(){
    return isDamageable();
}

bool Projectiles::spray_damage(){
    return isDamageable();
}


bool Projectiles::overlap(){
    Actor* b=stu()->overlap_checker(this);
    if(b!=nullptr){
       if(actorType_ret()=="Flame")
           b->flame_damage();
       else
           b->spray_damage();
    alterstatus();
    return true;
    }
    return false;
}

int Projectiles::getDistance() const{
    return distance;
}

void Projectiles::setDistance(int x){
    distance+=x;
}

bool Projectiles::isDamageable(){
    return false;
}

void Projectiles::doSomething(){
    if(!status())
        return;
    if(overlap()){
        return;
     }
     setDistance(SPRITE_WIDTH);
     move_projectile();
     return;
}

void Projectiles::move_projectile(){
    move_projectile_different();
    double x=getX();
    double y=getY();
    getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);
    moveTo(x, y);
}

Flame::Flame(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Projectiles(imageID, startX, startY, dir, size, depth, s){
    set_type("Flame");
}

void Flame::move_projectile_different(){
    if(getDistance()==32){
        alterstatus();
        return;
    }
        
}

Spray::Spray(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Projectiles(imageID, startX, startY, dir, size, depth, s){
    set_type("Spray");
}

void Spray::move_projectile_different(){
    if(getDistance()==112){
        alterstatus();
        return;
    }

}

Goodies::Goodies(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Actor(imageID, startX, startY, dir, size, depth, s){
        g_life=0;
        max_life=max(rand()%(300-10*(stu()->getLevel())),50);
        set_type("Goodie");
}

bool Goodies::flame_damage(){
    alterstatus();
    return isDamageable();
}

bool Goodies::spray_damage(){
    alterstatus();
    return isDamageable();
}

bool Goodies::blockable(){
    return false;
}


bool Goodies::isDamageable(){
    return true;
}

Fungus::Fungus(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Goodies(imageID, startX, startY, dir, size, depth, s){
    set_type("Fungus");
}

RH_goodie::RH_goodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Goodies(imageID, startX, startY, dir, size, depth, s){
    set_type("RH_goodie");
}

EL_goodie::EL_goodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Goodies(imageID, startX, startY, dir, size, depth, s){
    set_type("EL_goodie");
}

FT_goodie::FT_goodie(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Goodies(imageID, startX, startY, dir, size, depth, s){
    set_type("FT_goodie");
}

void Goodies::doSomething(){
    if(!status())
        return;
    if(overlap()){
            if(actorType_ret()=="Fungus")
                stu()->playSound(SOUND_PLAYER_HURT);
            else{
                stu()->playSound(SOUND_GOT_GOODIE);
            }
            doDifferent();
            alterstatus();
    }
    if(g_life_ret()==max_life_ret()){
        this->alterstatus();
     }
    else{
       this->inc_glife();
    }
    return;
}

bool Goodies::overlap(){
    double aX = getX();
    double aY = getY();
    double sX=stu()->player()->getX();
    double sY=stu()->player()->getY();
    double x=aX - sX;
    double y=aY - sY;
    double dist = sqrt(pow(x, 2) + pow(y, 2));
    if(dist<=SPRITE_WIDTH)
        return true;
    return false;
 
}

int Goodies::g_life_ret() const{
    return g_life;
}

int Goodies::max_life_ret() const{
    return max_life;
}

void Goodies::inc_glife(){
    g_life++;
}

void RH_goodie::doDifferent(){
    stu()->player()->change_hit(stu()->player()->get_hit()+100);
    stu()->increaseScore(250);
    return;
}

void EL_goodie::doDifferent(){
    stu()->incLives();
    stu()->increaseScore(300);
    return;
}
void FT_goodie::doDifferent(){
    stu()->player()->change_flame(stu()->player()->get_flame()  +5);
    stu()->increaseScore(300);
    return;
}

void Fungus::doDifferent(){
    stu()->player()->change_hit(stu()->player()->get_hit()-20);
    stu()->increaseScore(-50);
    return;
}

Pit::Pit(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Actor(imageID, startX, startY, dir, size, depth, s){
    reg_sal = 5;
    agg_sal = 3;
    e_coli = 2;
    set_type("Pit");
    number=10;
}

bool Pit::flame_damage(){
    return isDamageable();
}

bool Pit::spray_damage(){
    return isDamageable();
}

int Pit::get_agg() const{
    return agg_sal;
}

int Pit::get_reg() const{
    return reg_sal;
}

int Pit::get_coli() const{
    return e_coli;
}

void Pit::dec_agg(){
    agg_sal--;
}

void Pit::dec_reg(){
    reg_sal--;
}

void Pit::dec_coli(){
    e_coli--;
}

bool Pit::blockable(){
    return false;
}

bool Pit::isDamageable(){
    return false;
}

void Pit::doSomething(){
   if(!status())
       return;
    if(number==0){
        alterstatus();
        return;
    }
    int x=randInt(0, 49);
   if(x==0){
       int x = randInt(0, 2);
           if(x==0 && e_coli>0){
               E_Coli* coli=new E_Coli(IID_ECOLI,getX(),getY(),0,1,1,stu());
               stu()->addVector(coli);
               dec_coli();
               stu()->playSound(SOUND_BACTERIUM_BORN);
               number--;
           }
           else if(x==1 && reg_sal>0){
                Reg_Sal* Rsal=new Reg_Sal(IID_SALMONELLA,getX(),getY(),90,1,1,stu());
                stu()->addVector(Rsal);
               dec_reg();
               stu()->playSound(SOUND_BACTERIUM_BORN);
               number--;
           }
           else if(x==2 && agg_sal>0   ){
                Agg_Sal* Asal=new Agg_Sal(IID_SALMONELLA,getX(),getY(),90,1,1,stu());
                stu()->addVector(Asal);
               dec_agg();
               stu()->playSound(SOUND_BACTERIUM_BORN);
               number--;
               }
               
           }
    }

Bacteria::Bacteria(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Actor(imageID, startX, startY, dir, size, depth, s){
    movement_planDistance=0;
    set_type("Bacteria");
    food_count=0;
}


int Bacteria::get_foodCount(){
    return food_count;
}

void Bacteria::set_foodCount(int x){
    food_count=x;
}

int Bacteria::get_hit_points(){
    return hit_points;
}

void Bacteria::set_hit_points(int x){
    hit_points=x;
}

int Bacteria::get_moveDistance(){
    return movement_planDistance;
}

void Bacteria::set_moveDistance(int x){
    movement_planDistance=x;
}

bool Bacteria::blockable(){
    return false;
}


bool Bacteria::isDamageable(){
    return true;
}

bool Bacteria::flame_damage(){
    if(!status()){
        return false;
    }
    set_hit_points(get_hit_points()-5);
    if(get_hit_points()<=0){
        stu()->increaseScore(100);
        playsound();
        stu()->change_kill();
        alterstatus();
        food_create();
        return isDamageable();
    }
    playsound();
return isDamageable();
    
}

bool Bacteria::spray_damage(){
    if(!status()){
        return false;
    }
    set_hit_points(get_hit_points()-2);
    if(get_hit_points()<=0){
        stu()->increaseScore(100);
        stu()->change_kill();
        playsound();
        alterstatus();
        food_create();
        return isDamageable();
    }
    playsound();
return isDamageable();
}

void Bacteria::food_create(){
   int x= randInt(0, 1);
    if(x==1){
        Food* f= new Food(IID_FOOD,getX(),getY(),0,1,1,stu());
        stu()->addVector(f);
    }
}

bool Bacteria::checkCoordinates(double x, double y){
    int z=sqrt(pow((x-VIEW_RADIUS),2)+pow((y-VIEW_RADIUS),2));
    if(z>VIEW_RADIUS || stu()->blocker2(x, y) )
        return false;
    return true;
}

void Bacteria::bact_create(){
    double newx= getX();
    double newy= getY();
    if(newx<(VIEW_WIDTH)/2)
        newx+=SPRITE_RADIUS;
    else if(newx>VIEW_WIDTH/2)
        newx-=SPRITE_RADIUS;
    
    if(newy<(VIEW_HEIGHT)/2)
        newy+=SPRITE_RADIUS;
    else if(newy>VIEW_HEIGHT/2)
        newy-=SPRITE_RADIUS;
    create(newx, newy);
    stu()->change_bact(stu()->getbact_no()+1);
    stu()->playSound(SOUND_BACTERIUM_BORN);
    set_foodCount(0);
}

bool Bacteria::overlap_player(){
    double aX = getX();
    double aY = getY();
    double sX=stu()->player()->getX();
    double sY=stu()->player()->getY();
    double x=aX - sX;
    double y=aY - sY;
    double dist = sqrt(pow(x, 2) + pow(y, 2));
    if(dist<=SPRITE_RADIUS*2)
        return true;
    return false;
}

bool Bacteria::overlap_food(){
    Actor* b=stu()->overlap_Food(this);
    if(b!=nullptr){
        b->alterstatus();
        return true;
    }
    return false;
}

void Bacteria::doSomething(){
    if(!status()){
        return;
    }
    doDifferent();
}

Salmonella::Salmonella(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Bacteria(imageID, startX, startY, dir, size, depth, s){
    set_moveDistance(0);
    set_type("Salmonella");
}
    
Reg_Sal::Reg_Sal(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Salmonella(imageID, startX, startY, dir, size, depth, s){
    set_moveDistance(0);
    set_type("Reg_Sal");
    set_hit_points(4);
}


Agg_Sal::Agg_Sal(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Salmonella(imageID, startX, startY, dir, size, depth, s){
    set_moveDistance(0);
    set_type("Agg_Sal");
    set_hit_points(10);
}

E_Coli::E_Coli(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s):Bacteria(imageID, startX, startY, dir, size, depth, s){
    set_moveDistance(0);
    set_type("E_Coli");
    set_hit_points(5);
}

bool Salmonella::contemplateMove(int angle, double x, double y){
    double p3=getX();
    double p4=getY();
    double a=x-p3;
    double b=y-p4;
    double dist = sqrt(pow(a, 2) + pow(b, 2));
    for(int i=0;i<dist;i++){
         getPositionInThisDirection(angle, 3, p3, p4);
         if(!checkCoordinates(p3, p4)){
             return false;
         }
    }
     return true;
}

void Salmonella::playsound(){
    if(get_hit_points()<=0){
        stu()->playSound(SOUND_SALMONELLA_DIE);
    }
    else{
        stu()->playSound(SOUND_SALMONELLA_HURT);
        }
}

void Salmonella::doDifferent(){
      if(!doDifferently())
          return;
      if(get_moveDistance()>0){
          double x=getX();
          double y=getY();
          set_moveDistance(get_moveDistance()-1);
          getPositionInThisDirection(getDirection(), 3, x, y);
          if(checkCoordinates(x, y)){
              moveTo(x,y);
          }
          else{
              setDirection(randInt(0, 359));
              set_moveDistance(10);
          }
          return;
      }
    
     else{
          Actor* f=stu()->food_finder(this);
          if(f==nullptr){
              int x=randInt(0, 359);
              setDirection(x);
              set_moveDistance(10);
          }
          else{
              double p1=f->getX();
              double p2=f->getY();
              int result=atan2(p2-getY(),p1-getX())*180/3.14;
              if(contemplateMove(result, p1, p2)){
                  setDirection(result);
                  set_moveDistance(10);
                  return;
              }
              else{
                  setDirection(randInt(0, 359));
                  set_moveDistance(10);
                  return;
              }
          }
      }
}

bool Reg_Sal::doDifferently(){
   if(overlap_player()){
       stu()->player()->change_hit(stu()->player()->get_hit()-1);
    }
   else if(get_foodCount()==3){
       bact_create();
        }
   else{
        if(overlap_food()){
            set_foodCount(get_foodCount()+1);
            }
        }
    return true;
}

void Reg_Sal::create(double newx, double newy){
    Reg_Sal* Rsal=new Reg_Sal(IID_SALMONELLA,newx,newy,90,1,1,stu());
    stu()->addVector(Rsal);

}

void Agg_Sal::create(double newx, double newy){
    Agg_Sal* Agg_sal=new Agg_Sal(IID_SALMONELLA,newx,newy,90,1,1,stu());
    stu()->addVector(Agg_sal);
}

void E_Coli::create(double newx, double newy){
    E_Coli* ecol=new E_Coli(IID_ECOLI,newx,newy,90,1,1,stu());
    stu()->addVector(ecol);
}

bool Agg_Sal::doDifferently(){
    if(overlap_player()){
        stu()->player()->change_hit(stu()->player()->get_hit()-2);
        return false;
    }
    if(checkSocrates()){
        double p1=stu()->player()->getX();
        double p2=stu()->player()->getY();
        int result=atan2(p2-getY(),p1-getX())*180/3.14;
        double p3=getX();
        double p4=getY();
        if(contemplateMove(result, p3, p4)){
            getPositionInThisDirection(result, 2, p3, p4);
            setDirection(result);
            moveTo(p3,p4);
        }
        if(overlap_player()){
            stu()->player()->change_hit(stu()->player()->get_hit()-2);
            return false;
         }
        else if(get_foodCount()==3){
            bact_create();
             }
        else{
             if(overlap_food()){
                 set_foodCount(get_foodCount()+1);
                 }
             }
        return false;
    }
     if(overlap_player()){
        stu()->player()->change_hit(stu()->player()->get_hit()-2);
         return true;;
     }
     else if(get_foodCount()==3){
        bact_create();
         }
    else{
         if(overlap_food()){
             set_foodCount(get_foodCount()+1);
             }
         }
    return true;
    
}

bool Agg_Sal::checkSocrates(){
    double aX = getX();
    double aY = getY();
    double sX=stu()->player()->getX();
    double sY=stu()->player()->getY();
    double x=sX - aX;
    double y=sY - aY;
    double dist = sqrt(pow(x, 2) + pow(y, 2));
    if(dist<=72)
        return true;
    return false;
}

void E_Coli::doDifferent(){
     if(overlap_player()){
         stu()->player()->change_hit(stu()->player()->get_hit()-4);
      }
     else if(get_foodCount()==3){
         bact_create();
          }
    else{
          if(overlap_food()){
              set_foodCount(get_foodCount()+1);
              }
          }
        if(checkSocrates()){
              double p1=stu()->player()->getX();
              double p2=stu()->player()->getY();
              int result=atan2(p2-getY(),p1-getX())*180/3.14;
            for(int i=0;i<10;i++){
                double p3=getX();
                double p4=getY();
                if(contemplateMove(result, p3, p4)){
                    getPositionInThisDirection(result, 2, p3, p4);
                    setDirection(result);
                    moveTo(p3,p4);
                   return;
                 }
               else{
                  setDirection(result+10);
                }
            }
          }
    return;
      
}

bool E_Coli::checkSocrates(){
    double aX = getX();
    double aY = getY();
    double sX=stu()->player()->getX();
    double sY=stu()->player()->getY();
    double x=sX - aX;
    double y=sY - aY;
    double dist = sqrt(pow(x, 2) + pow(y, 2));
    if(dist<=256)
        return true;
    return false;
}

bool E_Coli::contemplateMove(int angle,double x, double y){
    getPositionInThisDirection(angle, 2, x, y);
    if(checkCoordinates(x, y)){
        return true;
    }
    return false;
}

void E_Coli::playsound(){
    if(get_hit_points()<=0){
        stu()->playSound(SOUND_SALMONELLA_DIE);    }
    else{
        stu()->playSound(SOUND_SALMONELLA_HURT);
        }
}


//Destructors
Salmonella::~Salmonella(){
    
}

Bacteria::~Bacteria(){
    
}

Reg_Sal::~Reg_Sal(){
    
}

Agg_Sal::~Agg_Sal(){
    
}

E_Coli::~E_Coli(){
    
}

Pit::~Pit(){
    
}

    
Food::~Food(){
    
}

Spray::~Spray(){
    
}

Flame::~Flame(){
    
}

Fungus::~Fungus(){
    
}

Goodies::~Goodies(){
    
}

RH_goodie::~RH_goodie(){
    
}

EL_goodie::~EL_goodie(){
    
}

FT_goodie::~FT_goodie(){
    
}

Actor::~Actor(){
}

Socrates::~Socrates(){
    
}

Dirt::~Dirt(){
    
}





