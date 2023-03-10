# Dusk2D
TODO

# Libraries
The game needs the following Libraries to build:

* SDL2
* SDL2_image
* SDL2_gfx (linux)
* SDL2_mixer
* SDL2_ttf
* zlib (linux)
* pthread (linux)

# License
All source code is released under the GNU General Public License Version 2.

# Packages
The game is right now in a pre alpha stage. We will provide packages for Windows and Linux once the game reaches a state where testers will be able to test the game.

# Building the Game
## Linux/BSD 
```bash
mkdir build/ && cd build/ && cmake ../
make
```
## Windows and Mac
### Windows
It is adviced to install a current version of VisualStudio to build the game. The game needs an c++17 compatible compiler.

### macOS
The way to build the game should work the same as on Linux.
