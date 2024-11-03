#include "Watchy_NamcoMuseum.h"
#include "StarLusterData.h"
#include "FreeSansBold4pt7b.h"
#include "FreeSansBold6pt7b.h"

#define INDEX_SIZE 4

#ifdef WATCHY_SIM
extern bool NTP_SYNC;
#else
extern RTC_DATA_ATTR bool NTP_SYNC;
#endif

bool DIVE_WARP = false;

void WatchyNamcoMuseum::starluster_drawWatchFace(){
#ifdef WATCHY_SIM
    srand(currentTime.Hour + currentTime.Minute + currentTime.Wday);
#else
    randomSeed(currentTime.Hour + currentTime.Minute + currentTime.Wday);
#endif

    display.fillScreen(GxEPD_WHITE);

    //Backgroud
    starluster_drawBackground();

    //Hour,Minute
    starluster_drawTime();

    if(DIVE_WARP){
        starluster_drawWarp();
    }
    DIVE_WARP = false;

    //Star
#ifdef WATCHY_SIM
    int star_choice = rand() % 3;
#else
    int star_choice = random(3);
#endif
    display.drawBitmap(0, 0, starArray[star_choice], INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);

    //Enemy
    int hitFlag = starluster_drawEnemy();

    //Pillar
    display.drawBitmap(0, 0, mainFrameUR, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);

    //Target
    display.drawBitmap(INDEX_SIZE * 24, INDEX_SIZE * 17, Target_mask, INDEX_SIZE * 5, INDEX_SIZE * 5, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 24, INDEX_SIZE * 17, Target, INDEX_SIZE * 5, INDEX_SIZE * 5, GxEPD_WHITE);

    //Lazer
    starluster_drawLazer();

    //Explosion
    starluster_drawExplosion(hitFlag);
}

void WatchyNamcoMuseum::starluster_drawBackground() {

    display.drawBitmap(0, 0, mainFrameU, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
    display.drawBitmap(0, INDEX_SIZE * 33, mainFrameD, INDEX_SIZE * 50, INDEX_SIZE * 17, GxEPD_BLACK);

    //Battery
    starluster_drawBattery();

    //Condition
    starluster_drawCondition();
}

int WatchyNamcoMuseum::starluster_drawEnemy() {
    int hitFlag = 0;
    int enemy_index[9][2] = { {1,25},{6,16},{10,18},{21,17},{24,2},{27,12},{34,22},{39,9},{43,1} };

    int randMax = 9;
    int choiced[5] = { -1, -1, -1, -1, -1 };

#ifdef WATCHY_SIM
    int enemyNum = rand() % 4 + 2;
#else
    int enemyNum = random(4) + 2;
#endif

    for (int cnt = 0; cnt < enemyNum; ) {
#ifdef WATCHY_SIM
        int choice = rand() % randMax;
#else
        int choice = random(randMax);
#endif
        int cnt2 = 0;
        for (; cnt2 < enemyNum; cnt2++) {
            if (choiced[cnt2] == choice) {
                break;
            }
        }
        if (cnt2 != enemyNum) {
            continue;
        }
        // locationCheck
        if (choice == 3) {
            hitFlag++;
        }
        else if(choice == 5) {
            hitFlag+=2;
        }
        choiced[cnt] = choice;
        cnt++;
    }

    for (int cnt = 0; cnt < enemyNum; cnt++) {
#ifdef WATCHY_SIM
        int choice_enemy = rand() % 3;
#else
        int choice_enemy = random(3);
#endif

        display.drawBitmap(INDEX_SIZE * enemy_index[choiced[cnt]][0], INDEX_SIZE * enemy_index[choiced[cnt]][1], enemyArray[choice_enemy], INDEX_SIZE * 6, INDEX_SIZE * 6, GxEPD_WHITE);
    }

    return hitFlag;
}

void WatchyNamcoMuseum::starluster_drawLazer()
{
#ifdef WATCHY_SIM
    Sleep(500);
    display.drawBitmap(INDEX_SIZE * 15, INDEX_SIZE * 23, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    Sleep(300);
    display.drawBitmap(INDEX_SIZE * 15, INDEX_SIZE * 23, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 33, INDEX_SIZE * 24, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    Sleep(300);
    display.drawBitmap(INDEX_SIZE * 33, INDEX_SIZE * 24, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 19, INDEX_SIZE * 22, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    Sleep(300);
    display.drawBitmap(INDEX_SIZE * 19, INDEX_SIZE * 22, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 29, INDEX_SIZE * 21, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    Sleep(300);
    display.drawBitmap(INDEX_SIZE * 29, INDEX_SIZE * 21, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
#else
    display.display(true);
    delay(500);
    display.drawBitmap(INDEX_SIZE * 14, INDEX_SIZE * 26, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    display.display(true);
    delay(200);
    display.drawBitmap(INDEX_SIZE * 14, INDEX_SIZE * 26, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 34, INDEX_SIZE * 25, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    display.display(true);
    delay(200);
    display.drawBitmap(INDEX_SIZE * 34, INDEX_SIZE * 25, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 19, INDEX_SIZE * 22, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    display.display(true);
    delay(200);
    display.drawBitmap(INDEX_SIZE * 19, INDEX_SIZE * 22, LazerL, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.drawBitmap(INDEX_SIZE * 29, INDEX_SIZE * 21, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_WHITE);
    display.display(true);
    delay(200);
    display.drawBitmap(INDEX_SIZE * 29, INDEX_SIZE * 21, LazerR, INDEX_SIZE * 4, INDEX_SIZE * 3, GxEPD_BLACK);
    display.display(true);
#endif


}

void WatchyNamcoMuseum::starluster_drawExplosion(const int& hitFlag)
{
#ifdef WATCHY_SIM
    Sleep(200);
#else
    display.display(true);
    delay(200);
#endif
    if (hitFlag == 1 || hitFlag == 3) {
        display.drawBitmap(INDEX_SIZE * 20, INDEX_SIZE * 18, explosion1_mask, 24, 18, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 20, INDEX_SIZE * 18, explosion1, 24, 18, GxEPD_WHITE);
    }
#ifdef WATCHY_SIM
    Sleep(200);
#else
    display.display(true);
    delay(200);
#endif
    if (hitFlag == 2 || hitFlag == 3) {
        display.drawBitmap(INDEX_SIZE * 26, INDEX_SIZE * 13, explosion1_mask, 24, 18, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 26, INDEX_SIZE * 13, explosion1, 24, 18, GxEPD_WHITE);
    }
#ifdef WATCHY_SIM
    Sleep(200);
#else
    display.display(true);
    delay(200);
#endif
    if (hitFlag == 1 || hitFlag == 3) {
        display.drawBitmap(INDEX_SIZE * 20, INDEX_SIZE * 18, explosion1, 24, 18, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 20, INDEX_SIZE * 18, explosion2_mask, 24, 18, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 20, INDEX_SIZE * 18, explosion2, 24, 18, GxEPD_WHITE);
    }
#ifdef WATCHY_SIM
    Sleep(200);
#else
    display.display(true);
    delay(200);
#endif
    if (hitFlag == 2 || hitFlag == 3) {
        display.drawBitmap(INDEX_SIZE * 26, INDEX_SIZE * 13, explosion1, 24, 18, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 26, INDEX_SIZE * 13, explosion2_mask, 24, 18, GxEPD_BLACK);
        display.drawBitmap(INDEX_SIZE * 26, INDEX_SIZE * 13, explosion2, 24, 18, GxEPD_WHITE);
    }
}

void WatchyNamcoMuseum::starluster_drawWarp() {
#ifdef WATCHY_SIM
    for (int cnt = 20; cnt > 3; cnt--) {
        display.drawBitmap(0, 0, warp1, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        Sleep(cnt * 12);

        display.drawBitmap(0, 0, warp1, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
        display.drawBitmap(0, 0, warp2, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        Sleep(cnt * 12);

        display.drawBitmap(0, 0, warp2, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
        display.drawBitmap(0, 0, warp3, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        Sleep(cnt * 12);

        display.drawBitmap(0, 0, warp3, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
    }

    for (int cnt = 0; cnt < 3; cnt++) {
        Sleep(50);
        display.drawBitmap(0, 0, window, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        Sleep(50);
        display.drawBitmap(0, 0, window, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
    }

#else
    for (int cnt = 8; cnt > 0; cnt--) {
        display.drawBitmap(0, 0, warp1, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        display.display(true);
        delay(cnt * 5);

        display.drawBitmap(0, 0, warp1, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
        display.drawBitmap(0, 0, warp2, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        display.display(true);
        delay(cnt * 5);

        display.drawBitmap(0, 0, warp2, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
        display.drawBitmap(0, 0, warp3, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        display.display(true);
        delay(cnt * 5);

        display.drawBitmap(0, 0, warp3, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
    }

    for (int cnt = 0; cnt < 3; cnt++) {
        delay(60);
        display.drawBitmap(0, 0, window, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_WHITE);
        display.display(true);
        delay(60);
        display.drawBitmap(0, 0, window, INDEX_SIZE * 50, INDEX_SIZE * 33, GxEPD_BLACK);
        display.display(true);
    }

#endif
}

void WatchyNamcoMuseum::starluster_drawTime() {
    display.setTextColor(GxEPD_WHITE);

    //Hour
    starluster_printSeg(INDEX_SIZE * 18, INDEX_SIZE * 30-1, currentTime.Hour / 10);
    starluster_printSeg(INDEX_SIZE * 23, INDEX_SIZE * 30-1, currentTime.Hour % 10);

    //Minute
    starluster_printSeg(INDEX_SIZE * 23, INDEX_SIZE * 38 - 2, currentTime.Minute / 10);
    starluster_printSeg(INDEX_SIZE * 28, INDEX_SIZE * 38 - 2, currentTime.Minute % 10);
}

void WatchyNamcoMuseum::starluster_printSeg(int x, int y, int number)
{
    int sizeX = 5;
    int sizeY = 6;

    for (int cnt = 0;; cnt++) {
        if (seg_index[number][cnt][0] == -1 && seg_index[number][cnt][1] == -1) {
            break;
        }
        display.setCursor(x + seg_index[number][cnt][0] * sizeX, y + seg_index[number][cnt][1] * sizeY);

        display.fillRect(x + seg_index[number][cnt][0] * sizeX, y -5 + seg_index[number][cnt][1] * sizeY, 6, 6, GxEPD_WHITE);

#ifdef WATCHY_SIM
        int mark_id = rand() % 3;
#else
        int mark_id = random(4);
#endif
        display.setTextColor(GxEPD_BLACK);
        switch (mark_id) {
        case 0:
            display.setFont(&FreeSansBold4pt7b);
            display.setCursor(x + seg_index[number][cnt][0] * sizeX, y + seg_index[number][cnt][1] * sizeY);
            display.print("E");
            break;
        case 1:
            display.setFont(&FreeSansBold4pt7b);
            display.setCursor(x + seg_index[number][cnt][0] * sizeX, y + seg_index[number][cnt][1] * sizeY);
            display.print("B");
            break;
        case 2:
            display.setFont(&FreeSansBold6pt7b);
            display.setCursor(x + 1 +  seg_index[number][cnt][0] * sizeX, y +4+ seg_index[number][cnt][1] * sizeY);
            display.print("*");
            break;
        default:
            break;
        }

    }
}

void WatchyNamcoMuseum::starluster_drawBattery()
{
    float VBAT = getBatteryVoltage();
    if (VBAT > 4.00) { VBAT = 4.00; };
    float VLevel = VBAT - 3.0; // meter:3.99Å`3.00

    // 7,158-43,161
    int Width_X = float(43 - 7) * VLevel;
    display.fillRect(7, 158, Width_X, 4, GxEPD_WHITE);
}

void WatchyNamcoMuseum::starluster_drawCondition()
{
    if (currentTime.Minute == 0) {
        NTP_SYNC = false;
#ifndef WATCHY_SIM
        connectWiFi();
#endif
        if (WIFI_CONFIGURED) {
#ifdef WATCHY_SIM
            NTP_SYNC = true;
#else
            NTP_SYNC = syncNTP();
#endif
}
    }

    display.fillRect(150, 151, 45, 23, GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&FreeSansBold6pt7b);

    display.setCursor(151, 160);
    if (WIFI_CONFIGURED) {
        display.print("WIFI OK");
    }
    else {
        display.print("WIFI NG");
    }

    display.setCursor(151, 172);
    if (NTP_SYNC) {
        display.print("NTP OK");
    }
    else {
        display.print("NTP NG");
    }
#ifndef WATCHY_SIM
    WiFi.mode(WIFI_OFF);
    btStop();
#endif
}


#ifndef WATCHY_SIM
void WatchyNamcoMuseum::starluster_push_down()
{
    DIVE_WARP = true;
    RTC.read(currentTime);
    drawWatchFace();
    display.display(true); // partial refresh    
}
#endif
