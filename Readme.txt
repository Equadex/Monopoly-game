README

2d-monopoly is a school project with focus on learning to program by doing something meaningsful. It's build in c and c++ using the Allegro 5 game libary. The latest source code can be found in my github repository here: https://github.com/Equadex/Monopoly-game
Settings for the program is done by commandline so if you want to change player amount or something else use the commands listed below.

Controls

There are a few shortcuts you can use instead of using the mouse

R = roll dice
E = End turn
Esc = exit game

If you using debug mode you can use the keys 1-9 for setting the dices values.


Commandline options

The game doesn't have any meny for settings. Instead it relies on you giving it arguments when you start the game. Below are the supported commands

-players x

Sets number of players in the game. Minium 1 player, maximum 6 players.

-FPS x

Frames rendered per second

-OpenGL x

Render in Opengl

-skip_intro

Self-explanatory

-set_intro_time x

Set the time which the intro is shown

-skip_all

Skips both intro and start screen

-debug x

Enables fps counter and mouse position data