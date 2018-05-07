#include <gui.h>
#include <fcinterface.h>
#include <pid.h>
#include <serial.h>
#include <ncurses.h>
#include <sys/ioctl.h>

void startGUI() {
    //pthread_t keyThread;
    //pthread_create(&keyThread, NULL, keyLoop, NULL);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    //Setup Menu
    int menuSize = 3;
    std::string options[menuSize] = {"Motor Throttle Test", "Serial Console", "Sensor Readout"};
    int choice;
    int highlight = 0;
    while(1) {
        printw("Welcome to Halo -- CTRL-C to quit");
        mvprintw(1,0,"Options:");
        while(1) {
            for(int i = 0; i < menuSize; i++) {
                if (i == highlight) attron(A_REVERSE);
                mvprintw(i+2, 1, options[i].c_str());
                attroff(A_REVERSE);
            }
            choice = getch();
            switch (choice) {
                case KEY_UP:
                    highlight = (highlight == 0) ? 0 : highlight - 1;
                    break;
                case KEY_DOWN:
                    highlight = (highlight == menuSize-1) ? menuSize-1 : highlight + 1;
                    break;
                default:
                    break;
            }
            if (choice == 10) break;
        }
        switch (highlight) {
            case 0:
                openMotorThrottleTest();
                break;
            case 1:
                openSerialConsole();
                break;
            case 2:
                openSensorReadout();
                break;
            default:
                openSensorReadout();
        }
    }
    
    //keyLoopActive = false;
    //delay(1000);
    //pthread_join(keyThread, NULL);
}

WINDOW* getNewFullWindow(bool keypadEnabled) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *win = newwin(yMax, xMax, 0,0);
    if (keypadEnabled) keypad(win, true);
    return win;
}

void openSensorReadout() {
    WINDOW *win = getNewFullWindow(true);
    mvwprintw(win, 0,0, "Sensors & Data:\nPressure Altitude:\nGPS Data:\n-----\nAttitude Data:\nPitch:\nRoll:"\n);
    
}

void openMotorThrottleTest() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *win = newwin(yMax, xMax, 0,0);
    keypad(win, true);
    mvwprintw(win,1,0,"Press 'a' to arm: currently NOT ARMED");
    mvwprintw(win,2,0,"Throttle: ");
    //mvprintw(3,0,"GPS Status: ");
    mvwprintw(win,4,0,"Last Key: ");
    //mvprintw(8,0,"Press 'c' to open command line");
    wrefresh(win);
    int key = 0;
    int throttle = 1000;
    while(run) {
        if (armed) mvwprintw(win,1,0,"Press 'd' to disarm: currently *ARMED*");
        if (!armed) mvwprintw(win,1,0,"Press 'a' to arm: currently NOT ARMED");
        if (!armed && key == 97) armRequest = true; //a
        else if (armed && key == 100) disarmRequest = true; //d
        else if (key == KEY_UP) throttle += 50;
        else if (key == KEY_DOWN) throttle -= 50;
        else if (key == 27) break;
        if (throttle < 1000) throttle = 1000;
        else if (throttle > 2000) throttle = 2000;
        newThrottle = throttle;

        mvwprintw(win,5,0,"Arm Request: %d", armRequest);
        mvwprintw(win,6,0,"Disarm Request: %d", disarmRequest);
        mvwprintw(win,7,0,"Auth Request: %d", authRequest);
        //readGPS();
        mvwprintw(win,2,11,"%d", newThrottle);
        //mvprintw(3,13,"%c", gps.fix.status);
        mvwprintw(win,4,11,"%d ", lastKey);
        wrefresh(win);
        //if (startCli) openCli();
        key = wgetch(win);
    }
    wclear(win);
    delwin(win);
}

void openSerialConsole() {
    Serial radio;
    radio.setupSerial("/dev/serial0", 9600);
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW *win = newwin(yMax, xMax, 0,0);
    mvwprintw(win,1,0,"Serial Console");
    mvwprintw(win,2,0,"Sending Heartbeat");
    noecho();
    delwin(win);
}

// void *keyLoop(void*) {
//     int throttle = 1000;
//     keyLoopActive = true;
//     keypad(stdscr, true);
//     while (keyLoopActive) {
//         int key = getch();
//         lastKey = key;
//         if (!armed && key == a_KEY) armRequest = true;
//         else if (armed && key == d_KEY) disarmRequest = true;
//         // else if (key == c_KEY) {
//         //     startCli = true;
//         //     while (startCli) {
//         //         int key = getch();
//         //         if (key == 27) startCli = false;
//         //     }
//         // }
//         else if (key == KEY_UP) throttle += 50;
//         else if (key == KEY_DOWN) throttle -= 50;
//         if (throttle < 1000) throttle = 1000;
//         else if (throttle > 2000) throttle = 2000;
//         newThrottle = throttle;
//         delay(50);
//     }
//     return NULL;
// }

// void openCli() {
//     int yMax, xMax;
//     getmaxyx(stdscr, yMax, xMax);
//     WINDOW *cliWindow = newwin(20, xMax, 8, 0);
//     keypad(cliWindow, true);
//     mvwprintw(cliWindow, 0,0,"Command Line:");
// }
