Rouge
=====

A turn based top down RPG game. Written in C++ with Lua bindings.
Very much a work in progress.

Dependencies
------------

Requires
- g++
- OpenGL
- OpenAL
- LibOGG
- LibVorbis
- LibDevil

Windows
-------

Requires Microsoft Visual Studio 2010.
Should build out of the box.
 
Linux
-----

Additional requires FreeGlut for Linux.

    sudo apt-get install g++ libopenal-dev libalut-dev libdevil-dev libogg-dev freeglut3-dev
    
### Build
Defaults to building with debug symbols.

    make NODEP=yes -j 4
    
### Build Unit Tests
Builds in unit testing mode. The tests are found in the tests folder.

    make NODEP=yes TESTING=yes -j 4

Lua Documentation
-----------------

Requires Python 2.7.5 or Python 3.x to create the HTML documentation.

    cd python
    python main.py classList.txt

Prebuild Documentation
----------------------

An example of the documentation can be found [here](http://astrellon.github.io/Rouge/).

Features
--------

- Tile based 2D graphics with a tile overlapping system for blending 
  between tiles, which uses a precedence for determining what goes 
  ontop of what. Specific overlap 
  graphics can be used when two specific tiles meet (ie, sand and water).
- Sound system that supports streaming OGG and WAV files, for long music 
  playback without having to store everything in memory.
- Lua Bindings for controlling just about every aspect of the game.
  From how a new game is created and loaded to how a door should behave
  when interacted with.
- All characters in game are treated equal. Each NPC has their own inventory,
  their own set of stats, and body parts. Each character can have their own
  set of body parts, this allows for races with many arms, or a playable
  horse, or a disabled character that is missing a limb.
- All maps are also Lua scripts, which allows for a map to be procedurally
  generated and saved with a save file to maintain the same map for a given
  save whilst being different for a new game.
- A dialogue system based on The Elder Scrolls: Morrowind with topics, 
  keywords and actions binded to those keywords.
- An asset manager that allows for textures and sounds to be reloaded at run
  time from disk. Scripts can also be reloaded at run time in theory.
- HTML documentation taken from the C++ comments using a Python script.
- In game text uses a simplistic CSS and HTML system for doing styles.
- Saving and loading is done via Lua scripts, where everything is serialised
  to Lua structures.
- Scripts for items and characters can be changed between saves to allow for
  updates without breaking save files. Only changes that affect behaviour
  will take affect however, changes based on the initial stats for a character
  or item are not taken into account except for new items.

Screenshots
-----------

Here is an example of the game screen with current paths for the player and
an NPC. Also showing the Inspector window which tells the player what is 
currently on a given tile.
![Screenshot of the game](http://i.imgur.com/Ntkdf7n.png)

An example of the dialogue system. Words spoken by the NPCs are shown on the 
left with keywords highlighted that can be clicked. Topics that have been 
unlocked by clicking on keywords are shown on the right.
![Dialogue Screenshot](http://i.imgur.com/EYZO3OP.png)

An example of the in game editor. UI is all placeholder, but tiles can be 
painted onto a map of a given size, tile overlapping updates in real time.
![Example of editor](http://i.imgur.com/xQdAVPk.png)
