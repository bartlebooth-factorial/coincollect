#include <ncurses.h>
#include <stdio.h>

#define printcoin() attron(A_BOLD); printw("*"); attroff(A_BOLD)

/*
 * We have the initial character, now fix movement drawing
 */

int main()
{
    int ypos, xpos, ycoin, xcoin, yscore, xscore;
    int ystep, xstep, score, ch;
    int coins[20];
    int nthcoin, i;
    int newycoin, newxcoin;

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

    while (true)
    {
        clear();

        move(ypos, xpos);
        printw("&");

        /* add coin */
        nthcoin += 2;

        newycoin = 10 + nthcoin;
        newxcoin = 10 + nthcoin;

        coins[nthcoin] = newycoin;
        coins[nthcoin] = newxcoin;

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

        move(yscore, xscore);
        printw("Score = %d", score);

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
            return 0;
        }

        for (i=0; i<=nthcoin; i+=2) {
            ycoin = coins[i];
            xcoin = coins[i+1];

            if (ypos==ycoin && xpos==xcoin)
                ++score;
        }
    }

    endwin();
}
