#include <simplecpp>
#include<iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;
//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_
#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>
using namespace simplecpp;

class MovingObject: public Sprite {//predefined
  vector < Sprite * > parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused = true) {
    vx = argvx;
    vy = argvy;
    ax = argax;
    ay = argay;
    paused = argpaused;
  }
  public:
    MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused = true): Sprite() {
      initMO(argvx, argvy, argax, argay, argpaused);
    }
  MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta): Sprite() {
    double angle_rad = angle_deg * PI / 180.0;
    double argvx = speed * cos(angle_rad);
    double argvy = -speed * sin(angle_rad);
    initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) {
    vx = argvx;
  }
  void set_vy(double argvy) {
    vy = argvy;
  }
  void set_ax(double argax) {
    ax = argax;
  }
  void set_ay(double argay) {
    ay = argay;
  }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() {
    paused = true;
  }
  void unpause() {
    paused = false;
  }
  bool isPaused() {
    return paused;
  }

  void addPart(Sprite * p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject * m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if (paused) {
    return;
  }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for (size_t i = 0; i < parts.size(); i++) {
    parts[i] -> move(vx * t, vy * t);
  }
  vx += ax * t;
  vy += ay * t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0] -> getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0] -> getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for (size_t i = 0; i < parts.size(); i++) {
    parts[i] -> moveTo(argx, argy);
  }
  double angle_rad = angle_deg * PI / 180.0;
  double argvx = speed * cos(angle_rad);
  double argvy = -speed * sin(angle_rad);
  vx = argvx;
  vy = argvy;
  ax = argax;
  ay = argay;
  paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject * m) {
  double xpos = m -> getXPos();
  double ypos = m -> getYPos();
  for (size_t i = 0; i < parts.size(); i++) {
    parts[i] -> moveTo(xpos, ypos);
  }
  initMO(m -> vx, m -> vy, m -> ax, m -> ay, m -> paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__

class Coin: public MovingObject {//predefined
  public: double coin_start_x;//variables associated with the coin
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

  public: Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta): MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();

  }

  void initCoin();
  void hideCoin();
  void resetCoin();
  void resetCoin1();

}; // End class Coin

//predefined
#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)


#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5


class Lasso: public MovingObject {//predefined
  double lasso_start_x;//varaibles associated with the lasso
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  public:
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin * the_coin;//stores the address of the coin if the lasso catches the coin
  int num_coins;//stores the number of coins collected
  int level;//stores the level on which the player is
  int lives;// stores the no. of lives remaining in level 3

  void initLasso();

  public:
    void hidefigures();
  Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta): MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin * coin);
  int getNumCoins() {
    return num_coins;
  }
  int getlevel() {
    return level;
  }
  int getlives() {
    return lives;
  }

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {//initializes the coin
  coin_start_x = (PLAY_X_START + WINDOW_X) / 2;//intial x-cordinate of coin
  coin_start_y = PLAY_Y_HEIGHT;//intial y-cordinate of coin
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);//graphics of the coin
  addPart( & coin_circle);

}


void Coin::hideCoin() {//function to hide the coin
  coin_circle.hide();
}

void Coin::resetCoin() {// function to reset the vertically coin to the original co-ordinates and original parameters of the variables associated with the coin
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}
void Coin::resetCoin1() {// function to reset the coin moving in a parabola with angle 60 degree with x ground
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = 60;//assigning the anglw with the ground to be 60 degree
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x - 100, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}


//lasso.cpp

void Lasso::draw_lasso_band() {//defining the features of the band of the lasso
  double len = (release_speed / MAX_RELEASE_SPEED) * LASSO_BAND_LENGTH;
  double arad = release_angle_deg * PI / 180.0;
  double xlen = len * cos(arad);
  double ylen = len * sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x - xlen), (lasso_start_y + ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {//initializing th lasso and adding graphics to it
  lasso_start_x = (PLAY_X_START + LASSO_X_OFFSET);//x co-ordinate
  lasso_start_y = (PLAY_Y_HEIGHT - LASSO_Y_HEIGHT);//y co-ordinate

  //below are the graphics associated to the shape, structure and the color of the lasso and it's loop
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE / 2);
  lasso_loop.setColor(COLOR("green"));
  lasso_loop.setFill(true);
  addPart( & lasso_circle);
  addPart( & lasso_loop);
  lasso_looped = false;
  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("white"));

  lasso_band.setColor(COLOR("black"));
  draw_lasso_band();

  //initializing the variables the_coin, num_coins,level and lives with appropriate values
  the_coin = NULL;
  num_coins = 0;
  level = 1;
  lives=3;



} // End Lasso::initLasso()

void Lasso::hidefigures() {//function to hide the lasso after the game
  lasso_band.hide();
  lasso_loop.hide();
  lasso_line.hide();

}

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE / 2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if (the_coin != NULL) {//checking if a coin has been found (the_coin stores the address of the found coin)
    num_coins++;//adding the total if a coin is founf

    if (level== 1)
      the_coin -> resetCoin();//resetting the trajectory of vertical moving coin
    else
      the_coin -> resetCoin1();//resetting the trajectory of coin moving in a parabola

    //defining levels based on the no. of coins
    if (num_coins >= 2)
      level = 2;
    if (num_coins >= 4)
      level = 3;
    if (num_coins >= 7)
      level = 4;

  }
  else
  {
     if(level==3)
     {lives--;//deducting the no. of lives if the lasso misses
     }
  }
  the_coin = NULL;//(fixed the bug)emptying the variable once the coin is accounted for

}

void Lasso::loopit() {
  if (lasso_looped) {
    return;
  } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {//function to chamge the angle of the lasso
  release_angle_deg += angle_deg;
  if (release_angle_deg < MIN_RELEASE_ANGLE_DEG) {
    release_angle_deg = MIN_RELEASE_ANGLE_DEG;
  }
  if (release_angle_deg > MAX_RELEASE_ANGLE_DEG) {
    release_angle_deg = MAX_RELEASE_ANGLE_DEG;
  }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {//function to change the release speed of the lasso
  release_speed += speed;
  if (release_speed < MIN_RELEASE_SPEED) {
    release_speed = MIN_RELEASE_SPEED;
  }
  if (release_speed > MAX_RELEASE_SPEED) {
    release_speed = MAX_RELEASE_SPEED;
  }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if (getYPos() > PLAY_Y_HEIGHT) {
    yank();
  }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin * coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr -> getXPos();
  double coin_y = coinPtr -> getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff * xdiff) + (ydiff * ydiff));//distance between the centre of the coin and lasso's loop

  if (distance <= LASSO_RADIUS) {//checking if the lasso's loop can catch the coin or not
    the_coin = coinPtr;//stores the address of the found coin
    the_coin -> getAttachedTo(this);

  }
} // End Lasso::check_for_coin()

main_program {

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);

  //graphics of the main screen
  Rectangle full(WINDOW_X / 2, WINDOW_Y / 2, WINDOW_X, WINDOW_Y);
  full.setColor(COLOR(0, 255, 255));//hex code for cyan
  full.setFill(true);
  Text t(370, 100, "THE LASSO GAME");

  //graphics of the home screen
  //graphics of the start button
  Line under(0,130,WINDOW_X,130);

  int startbox_x1 = 200, startbox_x2 = 300, startbox_y1 = 300, startbox_y2 = 350;
  Rectangle r1((startbox_x1 + startbox_x2) / 2, (startbox_y1 + startbox_y2) / 2, 100, 50);
  Text start((startbox_x1 + startbox_x2) / 2, (startbox_y1 + startbox_y2) / 2, "START");

  r1.setColor(COLOR("white"));
  r1.setFill(true);
  Rectangle r11((startbox_x1 + startbox_x2) / 2, (startbox_y1 + startbox_y2) / 2, 100, 50);

  //graphics of the instructions button
  int intructionsbox_x1 = 430, intructionsbox_x2 = 550, intructionsbox_y1 = 300, intructionsbox_y2 = 350;
  Rectangle r2((intructionsbox_x1 + intructionsbox_x2) / 2, (intructionsbox_y1 + intructionsbox_y2) / 2, 110, 50);
  Text intructions((intructionsbox_x1 + intructionsbox_x2) / 2, (intructionsbox_y1 + intructionsbox_y2) / 2, "INSTRUCTIONS");

  r2.setColor(COLOR("white"));
  r2.setFill(true);
  Rectangle r21((intructionsbox_x1 + intructionsbox_x2) / 2, (intructionsbox_y1 + intructionsbox_y2) / 2, 110, 50);

  //graphics of the exit/close button
  int closebox_x1 = 630, closebox_x2 = 680, closebox_y1 = 85, closebox_y2 = 115;
  Rectangle r3((closebox_x1 + closebox_x2) / 2, (closebox_y1 + closebox_y2) / 2, 50, 15);
  Text close((closebox_x1 + closebox_x2) / 2, (closebox_y1 + closebox_y2) / 2, "EXIT");

  r3.setColor(COLOR("red"));
  r3.setFill(true);
  Rectangle r31((closebox_x1 + closebox_x2) / 2, (closebox_y1 + closebox_y2) / 2, 50, 15);

  //graphics of the practice button
  int practicebox_x1 = 315, practicebox_x2 = 415, practicebox_y1 = 390, practicebox_y2 = 440;
  Rectangle r4((practicebox_x1 + practicebox_x2) / 2, (practicebox_y1 + practicebox_y2) / 2, 100, 50);
  Text practice((practicebox_x1 + practicebox_x2) / 2, (practicebox_y1 + practicebox_y2) / 2, "PRACTICE");

  r4.setColor(COLOR("white"));
  r4.setFill(true);
  Rectangle r41((practicebox_x1 + practicebox_x2) / 2, (practicebox_y1 + practicebox_y2) / 2, 100, 50);

  // designing boundaries of the home screen
  Rectangle border1(25, WINDOW_Y / 2, 50, WINDOW_Y);
  Rectangle border2(WINDOW_X - 25, WINDOW_Y / 2, 50, WINDOW_Y);
  Rectangle border3(WINDOW_X / 2, 25, WINDOW_X, 50);
  Rectangle border4(WINDOW_X / 2, WINDOW_Y - 25, WINDOW_X, 50);

  border1.setColor(COLOR("blue"));
  border2.setColor(COLOR("blue"));
  border3.setColor(COLOR("blue"));
  border4.setColor(COLOR("blue"));

  border1.setFill(true);
  border2.setFill(true);
  border3.setFill(true);
  border4.setFill(true);

  Line line1(0, 50, WINDOW_X, 50);
  Line line2(0, WINDOW_Y - 50, WINDOW_X, WINDOW_Y - 50);
  Line line3(50, 0, 50, WINDOW_Y);
  Line line4(WINDOW_X - 50, 0, WINDOW_X - 50, WINDOW_Y);

  while (true) {
    t.show();
    r1.show();
    r11.show();
    r2.show();
    r21.show();
    r4.show();
    r41.show();
    practice.show();
    start.show();
    intructions.show();
    r3.show();
    r31.show();
    close.show();
    border2.show();
    line4.show();
    under.show();

    int click1 = getClick();
    if (click1 / 65536 > startbox_x1 && click1 / 65536 < startbox_x2 && click1 % 65536 < startbox_y2 && click1 % 65536 > startbox_y1) {
    //if the player presses the start button
      r1.setColor(COLOR("black"));
      r1.setFill(true);
      wait(0.1);
      r1.setColor(COLOR("white"));

      //hiding all the unneccessary graphic elements
      r1.hide();
      r11.hide();
      r2.hide();
      r21.hide();
      r3.hide();
      r31.hide();
      r4.hide();
      r41.hide();
      practice.hide();
      close.hide();
      start.hide();
      intructions.hide();
      border2.hide();
      line4.hide();
      under.hide();

      int stepCount = 0;
      float stepTime = STEP_TIME;
      float runTime = -1; // sec; -ve means infinite
      float currTime = 0;

      // Draw lasso at start position
      double release_speed = INIT_RELEASE_SPEED; // m/s
      double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
      double lasso_ax = 0;
      double lasso_ay = LASSO_G;
      bool paused = true;
      bool rtheta = true;
      Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

      //graphics of the game screen
      Rectangle b1(WINDOW_X / 2, PLAY_Y_HEIGHT + 64, WINDOW_X + 50, PLAY_Y_HEIGHT / 4);
      b1.setColor(COLOR(165, 42, 42));//hex code for brown
      b1.setFill(true);
      Rectangle b2(PLAY_X_START / 2, WINDOW_Y / 2, PLAY_X_START, WINDOW_Y);
      b2.setColor(COLOR("black"));
      b2.setFill(true);

      Line l(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
      l.setColor(COLOR("black"));

      //for displaying the command inputted and the no. of coins at present
      string msg("Cmd: _");
      Text charPressed(PLAY_X_START + 50, PLAY_Y_HEIGHT + 20, msg);
      char coinScoreStr[256];
      sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
      Text coinScore(PLAY_X_START + 50, PLAY_Y_HEIGHT + 50, coinScoreStr);

      paused = true;
      rtheta = true;
      double coin_speed = COIN_SPEED;
      double coin_angle_deg = COIN_ANGLE_DEG;
      double coin_ax = 0;
      double coin_ay = COIN_G;
      Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

      // After every COIN_GAP sec, make the coin jump
      double last_coin_jump_end = 0;
      //showing the no. of lives left using red circles
      Text lives(605,130,"LIVES :");
      Circle c1(640,130,5);
      Circle c2(655,130,5);
      Circle c3(670,130,5);
      c1.setColor(COLOR("red"));
      c2.setColor(COLOR("red"));
      c3.setColor(COLOR("red"));
      c1.setFill(true);
      c2.setFill(true);
      c3.setFill(true);
      c1.hide();
      c2.hide();
      c3.hide();
      lives.hide();

      //displaying the level
      Text level1(655, 100, "LEVEL-1");
      level1.hide();
      Text level2(655, 100, "LEVEL-2");
      level2.hide();
      Text level3(655, 100, "LEVEL-3");
      level3.hide();


      int flag = 0, first = 0;//flag as an indicator when 'h' key is pressed;first as a counter
      for (;;) {

        int lev = lasso.getlevel();//to call the level at which the player is
        //displaying graphics i.e level no.
        if (lev == 1) {

          level1.show();

        } else if (lev == 2) {

          level1.hide();
          level2.show();

        } else if (lev == 3) {
          lives.show();
          level2.hide();
          level3.show();
          int life=lasso.getlives();//calling and storing the no. of lives left
          //displaying the no. of lives left on the screen
          if(life==3)
          {
           c1.show();
           c2.show();
           c3.show();
          }
          else if(life==2)
          {c1.show();
           c2.show();
           c3.hide();
          }
          else if(life==1)
          {c1.show();
           c2.hide();
           c3.hide();
          }
          else
          {
           //message displayed when the player loses
           Text over(350,350,"GAME OVER");
           Text over1(350,375,"BETTER LUCK NEXT TIME");
           wait(1);
           flag=1;//going back to home screen
          }


        } else {
           //message when the player wins
           level3.hide();
           Text congrats(350, 350, "CONGRATULTAIONS!!!");
           wait(1.5);
           flag=1;//going back to home screen
        }

        if ((runTime > 0) && (currTime > runTime)) {
          break;
        }
        if (flag == 1)
        { //taking the player to home screen
           Text home(675,550,"Going back to home screen.");
           wait(0.5);
           Text home1(675,550,"Going back to home screen..");
           wait(0.5);
           Text home2(675,550,"Going back to home screen...");
           wait(0.5);
          break;
        }
        XEvent e;
        bool pendingEv = checkEvent(e);
        if (pendingEv) {
          char c = charFromEvent(e);//accepts the command key from the user
          msg[msg.length() - 1] = c;
          charPressed.setMessage(msg);
          if (first == 0) {
            c = '=';
            first++;
          }
          // When t is pressed, throw lasso
          // If lasso within range, make coin stick
          // When y is pressed, yank lasso
          // When l is pressed, loop lasso
          // When q is pressed, quit
          // When h is pressed, go back to home screen
          // When = is pressed increse release speed of lasso and when - is pressed decrease it
          // When ] is pressed increse release angle of lasso and when [ is pressed decrease it
          switch (c) {
          case 't':
            lasso.unpause();
            break;
          case 'y':
            lasso.yank();
            break;
          case 'l':
            lasso.loopit();
            lasso.check_for_coin( & coin);
            wait(STEP_TIME * 5);
            break;
          case '[':
            if (lasso.isPaused()) {
              lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);
            }
            break;
          case ']':
            if (lasso.isPaused()) {
              lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);
            }
            break;
          case '-':
            if (lasso.isPaused()) {
              lasso.addSpeed(-RELEASE_SPEED_STEP);
            }
            break;
          case '=':
            if (lasso.isPaused()) {
              lasso.addSpeed(+RELEASE_SPEED_STEP);
            }
            break;
          case 'h':
          flag = 1;

           break;
          case 'q': {
            //hiding graphics then displaying exit message
            lasso.hidefigures();
            coin.hideCoin();
            Rectangle full1(WINDOW_X / 2, WINDOW_Y / 2, WINDOW_X, WINDOW_Y);
            full1.setColor(COLOR(0, 255, 255));
            full1.setFill(true);
            Text thank1(350, 475, "THANKS FOR PLAYING");
            Text thank2(350, 500, "HAVE A NICE DAY!!");
            wait(2);
            exit(0);

          }
          default:
            break;
          }
        }

        lasso.nextStep(stepTime);

        coin.nextStep(stepTime);
        if (coin.isPaused()) {
          if ((currTime - last_coin_jump_end) >= COIN_GAP) {
            coin.unpause();
          }
        }

        //resetting the coin to the original position depending on the current level and trajectory when it hits the ground
          if (coin.getYPos() > PLAY_Y_HEIGHT) {

        if (lasso.getlevel() == 1)
        {
            coin.resetCoin();
            last_coin_jump_end = currTime;
            }
            else if(lasso.getlevel() == 2||lasso.getlevel()==3)
          coin.resetCoin1();
          last_coin_jump_end = currTime;
        }

        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
      } // End for(;;)

      wait(1);

    } else if (click1 / 65536 > intructionsbox_x1 && click1 / 65536 < intructionsbox_x2 && click1 % 65536 < intructionsbox_y2 && click1 % 65536 > intructionsbox_y1) {
      //if the instruction button is pressed
      r2.setColor(COLOR("black"));
      r2.setFill(true);
      wait(0.1);
      r2.setColor(COLOR("white"));
      t.hide();
      r1.hide();
      r11.hide();
      r2.hide();
      r21.hide();
      r3.hide();
      r31.hide();
      r4.hide();
      r41.hide();
      practice.hide();
      start.hide();
      close.hide();
      intructions.hide();
      under.hide();
      Text t1(370, 20, "THE LASSO GAME");
      Text intro(370, 80, "INSTRUCTIONS");

      //displaying all the details of the levels of the game
      Text instruction1(370,150,"THE LASSO GAME has 3 levels.");
      Text instruction2(370,190,"LEVEL 1-");
      Text instruction3(370,215,"In this level you have to catch a coin moving vertically 2 times to move");
      Text instruction4(370,240," to the next level.");
      Text instruction5(370,280,"LEVEL 2-");
      Text instruction6(370,305,"This level is a bit trickier as the coin will be moving 1st vertically then in the");
      Text instruction7(370,330,"shape of a parabola and you have to catch it twice to move to the next level.");
      Text instruction8(370,370,"LEVEL 3-");
      Text instruction9(370,395," This is the final level of the game and in this level you will get three lives");
      Text instruction10(370,420,"and everytime you miss a life gets deducted.Your aim is to collect three coins");
      Text instruction11(370,445,"before you run out of lives while the trajectory is still a parabola.");
      Text instruction12(370,470,"This way, in order to win the game you need to collect a total of 7 coins");
      Text instruction13(370,530,"**CLICK ANYWHERE TO SEE THE COMMAND-KEY LIST**");

      getClick();
      //hiding old intructions
      instruction1.hide();
      instruction2.hide();
      instruction3.hide();
      instruction4.hide();
      instruction5.hide();
      instruction6.hide();
      instruction7.hide();
      instruction8.hide();
      instruction9.hide();
      instruction10.hide();
      instruction11.hide();
      instruction12.hide();
      instruction13.hide();

      //displaying the command-key list
      Text command1(250,170,"  COMMAND                -            KEY   ");
      Text command2(250,220,"   1.  Inc lasso speed                      =   ");
      Text command3(250,250,"   2.  Dec lasso speed                      -   ");
      Text command4(250,280,"   3.  Inc lasso angle                      ]    ");
      Text command5(250,310,"   4.  Dec lasso angle                      [    ");
      Text command6(250,340,"   5.  Release lasso                        t    ");
      Text command7(250,370,"   6.  Loop                                      l    ");
      Text command8(250,400,"   7.  Yank                                      y    ");
      Text command9(250,430,"8.  Go back to home screen               h ");
      Text command10(250,460,"      9.  quit game                              q ");
      Text command11(370,530,"**CLICK ANYWHERE TO GO BACK TO HOME-SCREEN**");

      getClick();
      //going back to the home screen

    } else if (click1 / 65536 > closebox_x1 && click1 / 65536 < closebox_x2 && click1 % 65536 < closebox_y2 && click1 % 65536 > closebox_y1) {
      //when the close/exit button is clicked
      //again hiding irrelevant graphics
      r1.hide();
      r11.hide();
      r2.hide();
      r21.hide();
      r3.hide();
      r31.hide();
      r4.hide();
      r41.hide();
      practice.hide();
      start.hide();
      intructions.hide();
      close.hide();
      t.hide();
      under.hide();
      //displaying the exit message
      Text thank1(350, 475, "THANKS FOR PLAYING");
      Text thank2(350, 500, "HAVE A NICE DAY!!");
      wait(2);
      exit(0);//terminating the program
    } else if (click1 / 65536 > practicebox_x1 && click1 / 65536 < practicebox_x2 && click1 % 65536 < practicebox_y2 && click1 % 65536 > practicebox_y1) {

      //if the practice button if clicked
      r4.setColor(COLOR("black"));
      r4.setFill(true);
      wait(0.1);
      r4.setColor(COLOR("white"));
      //hiding irrelevant graphics
      r1.hide();
      r11.hide();
      r2.hide();
      r21.hide();
      r3.hide();
      r31.hide();
      r4.hide();
      r41.hide();
      under.hide();

      practice.hide();
      start.hide();
      intructions.hide();
      close.hide();

      int stepCount = 0;
      float stepTime = STEP_TIME;
      float runTime = -1; // sec; -ve means infinite
      float currTime = 0;

      // Draw lasso at start position
      double release_speed = INIT_RELEASE_SPEED; // m/s
      double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
      double lasso_ax = 0;
      double lasso_ay = LASSO_G;
      bool paused = true;
      bool rtheta = true;
      Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

      Rectangle b1(WINDOW_X / 2, PLAY_Y_HEIGHT + 64, WINDOW_X + 50, PLAY_Y_HEIGHT / 4);
      b1.setColor(COLOR(165, 42, 42));
      b1.setFill(true);
      Rectangle b2(PLAY_X_START / 2, WINDOW_Y / 2, PLAY_X_START, WINDOW_Y);
      b2.setColor(COLOR("black"));
      b2.setFill(true);

      Line l(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
      l.setColor(COLOR("black"));

      string msg("YOUR COMMAND: _");
      Text charPressed(PLAY_X_START + 50, PLAY_Y_HEIGHT + 20, msg);
      char coinScoreStr[256];
      sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
      Text coinScore(PLAY_X_START + 50, PLAY_Y_HEIGHT + 50, coinScoreStr);

      paused = true;
      rtheta = true;
      double coin_speed = COIN_SPEED;
      double coin_angle_deg = COIN_ANGLE_DEG;
      double coin_ax = 0;
      double coin_ay = COIN_G;
      Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
      coin.coin_circle.hide();
      int flag = 0;

      Text x(375, 175, "ENTER THE KEYS FOR THE COMMANDS THAT WILL BE DISPLAYED");
      wait(4);
      x.hide();

      // After every COIN_GAP sec, make the coin jump
      double last_coin_jump_end = 0;

      //string storing all the commands whose key the player has to input
      string commands[7] = {" INCREASE LASSO SPEED","DECREASE LASSO SPEED","INCREASE LASSO ANGLE","DECREASE LASSO ANGLE","RELEASE LASSO","LOOP","YANK"};

      char cmds[7] = {'=','-',']','[','t','l','y'};//character array storing the correct key corresponding to the string commands
      int c1 = 0;
      for (int i = 0; i < 7; i++) {
      Text command(375, 175, commands[i]);
      if (flag == 1)
      {
         Text home(675,550,"Going back to home screen.");
         wait(0.5);
         Text home1(675,550,"Going back to home screen..");
         wait(0.5);
         Text home2(675,550,"Going back to home screen...");
         wait(0.5);
         break;
      }
     for (;;) {
     if ((runTime > 0) && (currTime > runTime)) {
       break;
     }

     if (flag == 1)
      {
         Text home(675,550,"Going back to home screen.");
         wait(0.5);
         Text home1(675,550,"Going back to home screen..");
         wait(0.5);
         Text home2(675,550,"Going back to home screen...");
         wait(0.5);
         break;
      }

       XEvent e;
       bool pendingEv = checkEvent(e);
       if (pendingEv) {

       char c = charFromEvent(e);
       msg[msg.length() - 1] = c;
       charPressed.setMessage(msg);
       if(c1==0)
       {
       c='s';
       }
       if (c == 'h') //to close practice if 'h' key is pressed and go back to home
       {
         flag = 1;
         break;
       }
       if (c == cmds[i]) { // checking if the key corresponds to the displayed command

         switch (c) {
         case 't':
           lasso.unpause();
           break;
         case 'y':
           lasso.lasso_line.hide();
           lasso.yank();
           break;
         case 'l':
           lasso.loopit();
           lasso.check_for_coin( & coin);
           wait(STEP_TIME * 5);
           break;
         case '[':
           if (lasso.isPaused()) {
             lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);
           }
           break;
         case ']':
           if (lasso.isPaused()) {
             lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);
           }
           break;
         case '-':
           if (lasso.isPaused()) {
             lasso.addSpeed(-RELEASE_SPEED_STEP);
           }
           break;
         case '=':
           if (lasso.isPaused()) {
             lasso.addSpeed(+RELEASE_SPEED_STEP);
           }
           break;
         case 'h':
           flag = 1;
           break;
         case 'q': {
           lasso.hidefigures();
           Rectangle full1(WINDOW_X / 2, WINDOW_Y / 2, WINDOW_X, WINDOW_Y);
           full1.setColor(COLOR(0, 255, 255));
           full1.setFill(true);
           Text thank1(350, 475, "THANKS FOR PLAYING");
           Text thank2(350, 500, "HAVE A NICE DAY!!");
           wait(2);
           exit(0);

         }
         default:
           break;
         }
         Text correct(700, 550, "CORRECT COMMAND"); //display if correct kehy was pressed
         wait(1);
         break;
       } else {
         if (c1 != 0) {
           Text incorrect(700, 550, "INCORRECT COMMAND"); //display if correct kehy was pressed
           Text tryagain(700, 575, "TRY AGAIN");
           wait(1);
         }
         if (c1 == 0) c1++;

       }

     }

     lasso.nextStep(stepTime);
     stepCount++;
     currTime += stepTime;
     wait(stepTime);
   }

 }

      lasso.lasso_line.hide();
      //display messages
      if(flag!=1)
      {
        Text x1(375, 155, "YOU ARE NOW READY");
        Text x2(375, 175, "TRY TO PLAY AROUND AND CATCH THE COIN");
        wait(4);
        x1.hide();
        x2.hide();
      }
       coin.coin_circle.show();//making the coin appear

      //similar to the actual game when start is pressed

      for (;;) {

        if ((runTime > 0) && (currTime > runTime)) {
          break;
        }

        if (flag == 1)
      {
         Text home(675,550,"Going back to home screen.");
         wait(0.5);
         Text home1(675,550,"Going back to home screen..");
         wait(0.5);
         Text home2(675,550,"Going back to home screen...");
         wait(0.5);
         break;
      }

        XEvent e;
        bool pendingEv = checkEvent(e);
        if (pendingEv) {

          char c = charFromEvent(e);
          msg[msg.length() - 1] = c;
          charPressed.setMessage(msg);
          switch (c) {
          case 't':
            lasso.unpause();
            break;
          case 'y':
            lasso.yank();
            break;
          case 'l':
            lasso.loopit();
            lasso.check_for_coin( & coin);
            wait(STEP_TIME * 5);
            break;
          case '[':
            if (lasso.isPaused()) {
              lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);
            }
            break;
          case ']':
            if (lasso.isPaused()) {
              lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);
            }
            break;
          case '-':
            if (lasso.isPaused()) {
              lasso.addSpeed(-RELEASE_SPEED_STEP);
            }
            break;
          case '=':
            if (lasso.isPaused()) {
              lasso.addSpeed(+RELEASE_SPEED_STEP);
            }
            break;
          case 'h':
            flag = 1;
            break;
          case 'q': {
            lasso.hidefigures();
            Rectangle full1(WINDOW_X / 2, WINDOW_Y / 2, WINDOW_X, WINDOW_Y);
            full1.setColor(COLOR(0, 255, 255));
            full1.setFill(true);
            Text thank1(350, 475, "THANKS FOR PLAYING");
            Text thank2(350, 500, "HAVE A NICE DAY!!");
            wait(2);
            exit(0);

          }
          default:
            break;
          }
        }

        lasso.nextStep(stepTime);

        coin.nextStep(stepTime);
        if (coin.isPaused()) {
          if ((currTime - last_coin_jump_end) >= COIN_GAP) {
            coin.unpause();
          }
        }

        if (coin.getYPos() > PLAY_Y_HEIGHT) {

        if (lasso.getlevel() == 1)
        {
            coin.resetCoin();
            last_coin_jump_end = currTime;
            }
            else
          coin.resetCoin1();
          last_coin_jump_end = currTime;
        }

        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);

        stepCount++;
        currTime += stepTime;
        wait(stepTime);
      } // End for(;;)
    } else { // when a random space on the home page is clicked
      Text error(350, 500, "Click on START to play"); // error message
      Text error1(350, 525, "For help click on INSTRUCTIONS");
      wait(2);
    }

  }
}
// End main_program
