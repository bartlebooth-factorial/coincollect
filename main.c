#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define printcoin() attron(A_BOLD); printw("*"); attroff(A_BOLD)

/*
 * disallow coins from spawning atop other coins
 */

int main()
{
    int ypos, xpos, ycoin, xcoin, yscore, xscore;
    int ystep, xstep, score, ch;
    int coins[80];
    int nthcoin, i;
    int newycoin, newxcoin;
    int rounds;

    srandom(time(NULL));

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    /*
     * SCREEN:
     * - - - - - - - - - - - - - - - - - -
     * [top left]            [top right] -
     *  y=0, x=0    +x   +x              -
     *                                   -
     * +y                                -
     *                                   -
     * +y                                -
     *                      y=max, x=max -
     * [bot left]           [bot right]  -
     * - - - - - - - - - - - - - - - - - -
     */

    ypos = 15;
    xpos = 30;

    ycoin = 5;
    xcoin = 10;

    coins[0] = ycoin;
    coins[1] = xcoin;

    nthcoin = 0;

    move(ycoin, xcoin);
    printcoin();

    yscore = 1;
    xscore = 80;

    ystep = 5;
    xstep = 10;

    score = 0;

    for (rounds=0; rounds<40; ++rounds)
    {
        clear();

        move(ypos, xpos);
        printw("&");

        /* add coin */
        newycoin = 5 * (random() % 8); /* must be a multiple of 5 */
        newxcoin = 10 * (random() % 8); /* must be a multiple of 10 */

        nthcoin += 2;

        coins[nthcoin] = newycoin;
        coins[nthcoin+1] = newxcoin;

        /* display coins */
        for (i=0; i<=nthcoin; i+=2) {

            ycoin = coins[i];
            xcoin = coins[i+1];

            /* print coin only when user is not on the coin's position */
            if (ypos != ycoin || xpos != xcoin) {
                move(ycoin, xcoin);
                printcoin();
            }
        }

        /* display score */
        move(yscore, xscore);
        printw("Score = %d", score);

        /* handle input */
        ch = getch();
        switch (ch)
        {
        // h
        case 104:
            xpos -= xstep;
            break;
        // j
        case 106:
            ypos += ystep;
            break;
        // k
        case 107:
            ypos -= ystep;
            break;
        // l
        case 108:
            xpos += xstep;
            break;
        // q
        case 113:
            endwin();
            exit(0);
        }

        for (i=0; i<=nthcoin; i+=2) {
            ycoin = coins[i];
            xcoin = coins[i+1];

            if (ypos==ycoin && xpos==xcoin)
                ++score;
        }
    }

    clear();
    move(10, 30);
    printw("Score = %d", score);
    getch();
    endwin();
    exit(0);
}
