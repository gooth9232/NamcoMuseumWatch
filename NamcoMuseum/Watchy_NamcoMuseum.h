#ifndef WATCHY_NAMCO_MUSEUM_H
#define WATCHY_NAMCO_MUSEUM_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM

class WatchyNamcoMuseum : public Watchy{
    public:
        using Watchy::Watchy;
        void drawPackage(const int& selectedGame);
        void drawWatchFace();

#ifndef WATCHY_SIM
        void handleButtonPress();
#endif
    private:
        // Digdug
        void digdug_drawWatchFace();
        void digdug_drawBackground();
        void digdug_drawBattery();
        void digdug_draw7Seg(const int& num, int index_x, int index_y);
        void digdug_drawEnemy(const int& num, int index_x, int index_y, int seed);
        void digdug_drawRock();
        void digdug_drawFlower();
        void digdug_drawDate();

        // Skykid
        void skykid_drawWatchFace();
        void skykid_drawBackground();
        void skykid_drawAirEnemy();
        void skykid_drawLandEnemy();
        void skykid_drawTarget();
        void skykid_drawFire();
        void skykid_drawDate();
        void skykid_drawBattery();
        void skykid_drawSeg(const int& num, int index_x, int index_y, bool one_left = false);
        void skykid_drawBomb(const int& playerLocate);
        void skykid_drawBullet(const int& seedNum, const bool& draw);

        // Xevious
        void xevious_drawWatchFace();
        void xevious_drawBackground();
        void xevious_drawBattery();
        void xevious_drawEnemy();
        void xevious_drawZapper();
        void xevious_drawSeg(const int& num, const int& index_x, const int& index_y, const bool& xevi_lang);
        void xevious_drawDate(const bool& xevi_lang);
        void xevious_drawSOL();
#ifndef WATCHY_SIM
        void xevious_push_up();
        void xevious_push_down();
#endif        

};

#endif