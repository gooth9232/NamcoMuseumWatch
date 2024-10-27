#include "Watchy_NamcoMuseum.h"
#include "GameTitle.h"

#ifdef ARDUINO_ESP32S3_DEV
  #define ACTIVE_LOW 0
#else
  #define ACTIVE_LOW 1
#endif

enum GameTitle {
    DIGDUG = 0,
    SKYKID,
    XEVIOUS,
    END,
};

#ifdef WATCHY_SIM
bool NTP_SYNC = true;
int selectedGame = GameTitle::SKYKID;
#else
RTC_DATA_ATTR bool NTP_SYNC = false;
RTC_DATA_ATTR int selectedGame = GameTitle::DIGDUG;
#endif


void WatchyNamcoMuseum::drawPackage(const int& selectedGame)
{
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, title_allArray[selectedGame], 200, 200, GxEPD_BLACK);

#ifdef WATCHY_SIM
    Sleep(2000);
#else
    display.display(true);
    delay(2000);
#endif
}


void WatchyNamcoMuseum::drawWatchFace(){
#ifdef WATCHY_SIM
    drawPackage(GameTitle::DIGDUG);
    digdug_drawWatchFace();
    Sleep(2000);

    drawPackage(GameTitle::SKYKID);
    skykid_drawWatchFace();
    Sleep(2000);

    drawPackage(GameTitle::XEVIOUS);
    xevious_drawWatchFace();
    Sleep(2000);

#else
    switch (selectedGame) {
    case GameTitle::DIGDUG:
        digdug_drawWatchFace();
        break;
    case GameTitle::SKYKID:
        skykid_drawWatchFace();
        break;
    case GameTitle::XEVIOUS:
        xevious_drawWatchFace();
        break;
    default:
        break;
    }
#endif
}

#ifndef WATCHY_SIM
void WatchyNamcoMuseum::handleButtonPress() {
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    // Menu Button
    if (wakeupBit & MENU_BTN_MASK) {
        if (guiState ==
            WATCHFACE_STATE) { // enter menu state if coming from watch face
            showMenu(menuIndex, false);
        }
        else if (guiState ==
            MAIN_MENU_STATE) { // if already in menu, then select menu item
            switch (menuIndex) {
            case 0:
                showAbout();
                break;
            case 1:
                showBuzz();
                break;
            case 2:
                showAccelerometer();
                break;
            case 3:
                setTime();
                break;
            case 4:
                setupWifi();
                break;
            case 5:
                showUpdateFW();
                break;
            case 6:
                showSyncNTP();
                break;
            default:
                break;
            }
        }
        else if (guiState == FW_UPDATE_STATE) {
            updateFWBegin();
        }
    }
    // Back Button
    else if (wakeupBit & BACK_BTN_MASK) {
        if (guiState == MAIN_MENU_STATE) { // exit to watch face if already in menu
            RTC.read(currentTime);
            showWatchFace(false);
        }
        else if (guiState == APP_STATE) {
            showMenu(menuIndex, false); // exit to menu if already in app
        }
        else if (guiState == FW_UPDATE_STATE) {
            showMenu(menuIndex, false); // exit to menu if already in app
        }
        else if (guiState == WATCHFACE_STATE) {

            selectedGame = (++selectedGame % GameTitle::END);

            drawPackage(selectedGame); // package image

            RTC.read(currentTime);
            drawWatchFace();
            display.display(true); // partial refresh

            return;
        }
    }
    // Up Button
    else if (wakeupBit & UP_BTN_MASK) {
        if (guiState == MAIN_MENU_STATE) { // increment menu index
            menuIndex--;
            if (menuIndex < 0) {
                menuIndex = MENU_LENGTH - 1;
            }
            showMenu(menuIndex, true);
        }
        else if (guiState == WATCHFACE_STATE) {
            if(selectedGame == GameTitle::XEVIOUS){
              xevious_push_up();
            }
            return;
        }
    }
    // Down Button
    else if (wakeupBit & DOWN_BTN_MASK) {
        if (guiState == MAIN_MENU_STATE) { // decrement menu index
            menuIndex++;
            if (menuIndex > MENU_LENGTH - 1) {
                menuIndex = 0;
            }
            showMenu(menuIndex, true);
        }
        else if (guiState == WATCHFACE_STATE) {
            if(selectedGame == GameTitle::XEVIOUS){
              xevious_push_down();
            }
            return;
        }
    }

    /***************** fast menu *****************/
    bool timeout = false;
    long lastTimeout = millis();
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(BACK_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);
    while (!timeout) {
        if (millis() - lastTimeout > 5000) {
            timeout = true;
        }
        else {
            if (digitalRead(MENU_BTN_PIN) == ACTIVE_LOW) {
                lastTimeout = millis();
                if (guiState ==
                    MAIN_MENU_STATE) { // if already in menu, then select menu item
                    switch (menuIndex) {
                    case 0:
                        showAbout();
                        break;
                    case 1:
                        showBuzz();
                        break;
                    case 2:
                        showAccelerometer();
                        break;
                    case 3:
                        setTime();
                        break;
                    case 4:
                        setupWifi();
                        break;
                    case 5:
                        showUpdateFW();
                        break;
                    case 6:
                        showSyncNTP();
                        break;
                    default:
                        break;
                    }
                }
                else if (guiState == FW_UPDATE_STATE) {
                    updateFWBegin();
                }
            }
            else if (digitalRead(BACK_BTN_PIN) == ACTIVE_LOW) {
                lastTimeout = millis();
                if (guiState ==
                    MAIN_MENU_STATE) { // exit to watch face if already in menu
                    RTC.read(currentTime);
                    showWatchFace(false);
                    break; // leave loop
                }
                else if (guiState == APP_STATE) {
                    showMenu(menuIndex, false); // exit to menu if already in app
                }
                else if (guiState == FW_UPDATE_STATE) {
                    showMenu(menuIndex, false); // exit to menu if already in app
                }
            }
            else if (digitalRead(UP_BTN_PIN) == ACTIVE_LOW) {
                lastTimeout = millis();
                if (guiState == MAIN_MENU_STATE) { // increment menu index
                    menuIndex--;
                    if (menuIndex < 0) {
                        menuIndex = MENU_LENGTH - 1;
                    }
                    showFastMenu(menuIndex);
                }
            }
            else if (digitalRead(DOWN_BTN_PIN) == ACTIVE_LOW) {
                lastTimeout = millis();
                if (guiState == MAIN_MENU_STATE) { // decrement menu index
                    menuIndex++;
                    if (menuIndex > MENU_LENGTH - 1) {
                        menuIndex = 0;
                    }
                    showFastMenu(menuIndex);
                }
            }
        }
    }
}
#endif
