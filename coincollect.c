#include <curses.h>
#include <stdlib.h>
#include <stdio.h>

#define GRID 8
#define YSTEP 3
#define XSTEP 6
#define YSCORE 1
#define XSCORE 80
#define YTURN 25
#define XTURN 80
#define TURNS 40
#define COINS_PER_TURN 2
#define UNLOCK_MAGIC 30

void curses_init(void);
void display_borders(void);
void display_player(int ypos, int xpos, int magic);
void add_coin(int nthcoin, int *coins, int *coinvalidity);
void display_coins(int ypos, int xpos, int nthcoin, int *coins, int *coinvalidity);
void display_midgame_score(int score);
void display_turn(int turn);
void movement(int *ypos, int *xpos, int magic, int *retry);
void normal_bounds(int *ypos, int *xpos);
void magic_bounds(int *ypos, int *xpos);
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
display_borders(void)
{
    int i;

    for (i=0; i<(GRID*XSTEP); i+=XSTEP)
        mvaddch(YSTEP * GRID, i, '-');

    for (i=0; i<(GRID*YSTEP); i+=YSTEP)
        mvaddch(i, XSTEP * GRID, '|');

    mvaddch(GRID*YSTEP, GRID*XSTEP, ';');
}

void
display_player(int ypos, int xpos, int magic)
{
    if (magic)
        mvaddch(ypos, xpos, 'M');
    else
        mvaddch(ypos, xpos, '&');
}

void
add_coin(int nthcoin, int *coins, int *coinvalidity)
{
    int newycoin, newxcoin;
    int ycoin, xcoin;
    int valid;
    int matches;
    int i;

    while (true)
    {
        newycoin = YSTEP * arc4random_uniform(GRID);
        newxcoin = XSTEP * arc4random_uniform(GRID);

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
display_midgame_score(int score)
{
    move(YSCORE, XSCORE);
    printw("Score = %d", score);
}

void
display_turn(int turn)
{
    move(YTURN, XTURN);
    printw("turn %d of 40", turn);
}

void
movement(int *ypos, int *xpos, int magic, int *retry)
{
    int key;

    key = getch();
    switch (key)
    {
    // h & a
    case 104:
    case 97:
        *xpos -= XSTEP;
        break;
    // j & s
    case 106:
    case 115:
        *ypos += YSTEP;
        break;
    // k & w
    case 107:
    case 119:
        *ypos -= YSTEP;
        break;
    // l & d
    case 108:
    case 100:
        *xpos += XSTEP;
        break;
    // space
    case 32:
        if (magic)
        {
            *ypos = YSTEP * (GRID / 2);
            *xpos = XSTEP * (GRID / 2);
        }
        break;
    // r
    case 114:
        *retry = 1;
        break;
    // q
    case 113:
        endwin();
        exit(0);
    }
}

void
normal_bounds(int *ypos, int *xpos)
{
    if (*ypos < 0)
        *ypos = 0;

    if (*xpos < 0)
        *xpos = 0;

    if (*ypos > YSTEP * (GRID - 1))
        *ypos = YSTEP * (GRID - 1);

    if (*xpos > XSTEP * (GRID - 1))
        *xpos = XSTEP * (GRID - 1);
}

void
magic_bounds(int *ypos, int *xpos)
{
    if (*ypos < 0)
        *ypos = YSTEP * (GRID - 1);

    if (*xpos < 0)
        *xpos = XSTEP * (GRID - 1);

    if (*ypos > YSTEP * (GRID - 1))
        *ypos = 0;

    if (*xpos > XSTEP * (GRID - 1))
        *xpos = 0;
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
    int magic;
    int games, totalscore;
    int ypos, xpos;
    int nthcoin;
    int score;
    int retry;
    int turn; 
    int i;
    int key, choice;

    for (i=0; i<80; i++) {
	    coins[2*i] = -1;
	    coins[2*i + 1] = -1;
	    coinvalidity[i] = -1;
    }

    magic = 0;

    games = 0;
    totalscore = 0;

    curses_init();

    while (true)
    {
        ypos = YSTEP * (GRID / 2);
        xpos = XSTEP * (GRID / 2);

        nthcoin = -2; /* initial offset */

        for (i=0; i<40; ++i)
            coinvalidity[i] = 1;

        score = 0;

        retry = 0;

        for (turn=0; turn<TURNS; ++turn)
        {
            clear();

            display_borders();

            display_player(ypos, xpos, magic);

            for (i=0; i<COINS_PER_TURN; ++i)
            {
                nthcoin+=2;
                add_coin(nthcoin, coins, coinvalidity);
            }

            display_coins(ypos, xpos, nthcoin, coins, coinvalidity);

            display_midgame_score(score);

            display_turn(turn);

            movement(&ypos, &xpos, magic, &retry);

            if (retry)
                break;

            if (magic)
                magic_bounds(&ypos, &xpos);
            else
                normal_bounds(&ypos, &xpos);

            score = collect(ypos, xpos, nthcoin, coins, coinvalidity, score);
        }

        if (retry)
            continue;

        clear();

        totalscore += score;
        ++games;

        if (score >= UNLOCK_MAGIC)
            magic = 1;

        display_score(score, totalscore, games);

        choice = getch();
        /* q to quit */
        if (choice==113)
        {
            endwin();
            exit(0);
        }
    }
}

