#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define GRID 8

/*
 * add mid-round retry
 * add magic character
 */

void curses_init(void);
void display_player(int ypos, int xpos);
void add_coin(int ystep, int xstep, int nthcoin, int *coins, int *coinvalidity);
void display_coins(int ypos, int xpos, int nthcoin, int *coins, int *coinvalidity);
void display_midgame_score(int yscore, int xscore, int score);
void display_turn(int yturn, int xturn, int turn);
int collect(int ypos, int xpos, int nthcoin, int *coins, int * coinvalidity, int score);
void display_score(int score, int totalscore, int games);

void
curses_init(void)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

void
display_player(int ypos, int xpos)
{
    mvaddch(ypos, xpos, '&');
}

void
add_coin(int ystep, int xstep, int nthcoin, int *coins, int *coinvalidity)
{
    int newycoin, newxcoin;
    int ycoin, xcoin;
    int valid;
    int matches;
    int i;

    while (true)
    {
        newycoin = ystep * ((random() % GRID) + 1);
        newxcoin = xstep * ((random() % GRID + 1));

        matches = 0;
        for (i=0; i<=nthcoin; i+=2)
        {
            ycoin = coins[i];
            xcoin = coins[i+1];
            valid = coinvalidity[i/2];

            if (ycoin==newycoin && xcoin==newxcoin && valid)
                matches += 1;
        }

        if (matches==0)
            break;
    }

    coins[nthcoin] = newycoin;
    coins[nthcoin+1] = newxcoin;
    coinvalidity[nthcoin/2] = 1;
}

void
display_coins(int ypos, int xpos, int nthcoin, int *coins, int *coinvalidity)
{
    int ycoin, xcoin;
    int valid;
    int i;

    for (i=0; i<=nthcoin; i+=2)
    {
        ycoin = coins[i];
        xcoin = coins[i+1];
        valid = coinvalidity[i/2];

        if ((ypos != ycoin || xpos != xcoin) && valid)
            mvaddch(ycoin, xcoin, '*');
    }
}

void
display_midgame_score(int yscore, int xscore, int score)
{
    move(yscore, xscore);
    printw("Score = %d", score);
}

void
display_turn(int yturn, int xturn, int turn)
{
    move(yturn, xturn);
    printw("turn %d of 40", turn);
}

int
collect(int ypos, int xpos, int nthcoin, int *coins, int * coinvalidity, int score)
{
    int ycoin, xcoin;
    int valid;
    int i;

    for (i=0; i<=nthcoin; i+=2)
    {
        ycoin = coins[i];
        xcoin = coins[i+1];
        valid = coinvalidity[i/2];

        if (ypos==ycoin && xpos==xcoin && valid)
        {
            ++score;
            coinvalidity[i/2] = 0;
        }
    }
    return score;
}

void
display_score(int score, int totalscore, int games)
{
    move(12, 44);
    attron(A_STANDOUT);
    printw("Score = %d", score);
    if (games>1)
    {
        move(16, 44);
        printw("Average score = %d", totalscore/games);
    }
    attroff(A_STANDOUT);
}

int
main(int argc, char *argv[])
{
    int coins[160], coinvalidity[80];
    int ystep, xstep;
    int yscore, xscore, yturn, xturn;
    int turns, coinsperturn;
    int games, totalscore;
    int ypos, xpos;
    int nthcoin;
    int score;
    int turn; 
    int i;
    int key, choice;

    srandom(time(NULL));

    ystep = 3;
    xstep = 6;

    yscore = 1;
    xscore = 80;

    yturn = 25;
    xturn = 80;

    turns = 40;

    coinsperturn = 2;

    games = 0;
    totalscore = 0;

    curses_init();

    while (true)
    {
        ypos = ystep * (GRID / 2);
        xpos = xstep * (GRID / 2);

        nthcoin = -2; /* initial offset */

        for (i=0; i<40; ++i)
            coinvalidity[i] = 1;

        score = 0;

        for (turn=0; turn<turns; ++turn)
        {
            clear();

            display_player(ypos, xpos);

            for (i=0; i<coinsperturn; ++i)
            {
                nthcoin+=2;
                add_coin(ystep, xstep, nthcoin, coins, coinvalidity);
            }

            display_coins(ypos, xpos, nthcoin, coins, coinvalidity);

            display_midgame_score(yscore, xscore, score);

            display_turn(yturn, xturn, turn);

            /* movement */
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

            /* movement bounds */
            if (ypos < ystep)
                ypos = ystep;

            if (xpos < xstep)
                xpos = xstep;

            if (ypos > ystep * GRID)
                ypos = ystep * GRID;

            if (xpos > xstep * GRID)
                xpos = xstep * GRID;

            score = collect(ypos, xpos, nthcoin, coins, coinvalidity, score);
        }

        clear();

        totalscore += score;
        ++games;

        display_score(score, totalscore, games);

        choice = getch();
        // q to quit
        if (choice==113)
        {
            endwin();
            exit(0);
        }
    }
}

