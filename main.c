#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define printcoin() attron(A_BOLD); printw("*"); attroff(A_BOLD)

/*
 * fix score breaking on final turn
 */

int main()
{
    int ypos, xpos, ycoin, xcoin, yscore, xscore;
    int ystep, xstep, score, ch;
    int coins[80];
    int coinvalidity[40];
    int valid;
    int nthcoin, i;
    int newycoin, newxcoin;
    int rounds;
    int found, matches;

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

    ypos = 16;
    xpos = 24;

    //ycoin = 5;
    //xcoin = 10;

    //coins[0] = ycoin;
    //coins[1] = xcoin;

    nthcoin = 0;

    for (i=0; i<40; i++)
        coinvalidity[i] = 1;

    move(ycoin, xcoin);
    printcoin();

    yscore = 1;
    xscore = 80;

    ystep = 4;
    xstep = 6;

    score = 0;

    for (rounds=0; rounds<40; ++rounds)
    {
        clear();

        //move(ypos, xpos);
        //printw("&");

        mvaddch(ypos, xpos, '&');

        /* add coin */
        nthcoin += 2;

        while (true)
        {
            newycoin = 4 * (random() % 8); /* must be a multiple of 5 */
            newxcoin = 6 * (random() % 8); /* must be a multiple of 10 */

            matches = 0;
            for (i=0; i<=nthcoin; i+=2)
            {
                ycoin = coins[i];
                xcoin = coins[i+1];

                if (ycoin==newycoin && xcoin==newxcoin)
                    matches += 1;
            }

            if (matches==0)
                break;
        }

        //move(0, 0);
        //printw("%d", newycoin);
        //move(0, 4);
        //printw("%d", newxcoin);

        coins[nthcoin] = newycoin;
        coins[nthcoin+1] = newxcoin;

        /* display coins */
        for (i=0; i<=nthcoin; i+=2)
        {
            ycoin = coins[i];
            xcoin = coins[i+1];
            valid = coinvalidity[i/2];

            /* print coin only when user is not on the coin's position */
            if ((ypos != ycoin || xpos != xcoin) && valid)
            {
                mvaddch(ycoin, xcoin, '*');
                //printcoin();
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
            valid = coinvalidity[i/2];

            if (ypos==ycoin && xpos==xcoin && valid)
            {
                ++score;
                coinvalidity[i/2] = 0;
            }
        }
    }

    clear();
    move(12, 44);
    printw("Score = %d", score);
    getch();
    endwin();
    exit(0);
}
