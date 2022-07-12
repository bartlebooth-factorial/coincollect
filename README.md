# Introduction

coincollect is an ncurses game about moving around an 8x8 grid to collect
as many coins as possible before running out of turns.

Each turn, two coins (\*) spawn in random positions on the grid, and the player (&)
gets to move one space in any cardinal direction. When the player touches a coin,
they collect it, removing the coin and incrementing their score by one. After 40
turns the game ends, and the player's score is displayed. Players can continue
and play multiple rounds, with their round-score and average-score being displayed
at the end of each round.

If the player is able to collect 30 coins by the end of a round, they become
the magician (M), who is able to move through walls as if the grid were a torus
and teleport to the center of the grid at any time, enabling the player to reach
higher scores than with the basic character.

# Installation

```sh
git clone https://github.com/bartlebooth-factorial/coincollect
cd coincollect
make
sudo make install
```
Now run the game with the command `coincollect`.

Note that the ncurses library (installed by default on most unix-like systems) is
required for compilation.

# Controls

* w/a/s/d keys or h/j/k/l (vim) keys to move.
* r to restart a round
* q to quit
* space to teleport to grid center (only as the magician)

# Contributing

All questions, comments, and pull requests are appreciated!
Email me at ericmfrederickson@gmail.com to get in touch.

