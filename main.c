#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * enable new coins to take the position of collected coins
 * add 2 coins per round
 * add magic character
 */

int main()
{
    int ypos, xpos;
    int ycoin, xcoin;
    int yscore, xscore;
    int yround, xround;
    int ystep, xstep;
    int score;
    int backscore;
    int key, choice;
    int coins[80];
    int coinvalidity[40];
    int valid;
    int nthcoin;
    int i;
    int newycoin, newxcoin;
    int round, rounds;
    int matches;

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

    while (true)
    {
        srandom(time(NULL));

        ystep = 4;
        xstep = 6;

        ypos = ystep * 4;
        xpos = xstep * 4;

        nthcoin = -2; /* initial offset */

        for (i=0; i<40; i++)
            coinvalidity[i] = 1;

        yscore = 1;
        xscore = 80;

        yround = 26;
        xround = 80;

        rounds = 40;

        score = 0;

        for (round=0; round<rounds; ++round)
        {
            clear();

            /* display player */
            mvaddch(ypos, xpos, '&');

            /* add coin */
            nthcoin += 2;

            while (true)
            {
                newycoin = ystep * (random() % 8); /* must be a multiple of ystep */
                newxcoin = xstep * (random() % 8); /* must be a multiple of xstep */

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
                    mvaddch(ycoin, xcoin, '*');
            }

            /* display score */
            move(yscore, xscore);
            printw("Score = %d", score);

            /* display round number */
            move(yround, xround);
            printw("move %d of 40", round);

            /* handle input */
            key = getch();
            switch (key)
            {
            // h & a
            case 104:
            case 97:
                xpos -= xstep;
                break;
            // j & s
            case 106:
            case 115:
                ypos += ystep;
                break;
            // k & w
            case 107:
            case 119:
                ypos -= ystep;
                break;
            // l & d
            case 108:
            case 100:
                xpos += xstep;
                break;
            // q
            case 113:
                endwin();
                exit(0);
            }

            /* enforce movement bounds */
            if (ypos < 0)
                ypos = 0;

            if (xpos < 0)
                xpos = 0;

            if (ypos > ystep * 8)
                ypos = ystep * 8;

            if (xpos > xstep * 8)
                xpos = xstep * 8;

            /* check for coin collecion */
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
        attron(A_STANDOUT);
        printw("Score = %d", score);
        attroff(A_STANDOUT);

        choice = getch();
        switch(choice)
        {
        // q to quit
        case 113:
            endwin();
            exit(0);
        }
    }
}
