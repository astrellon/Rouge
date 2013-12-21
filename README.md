Rouge
=====

A turn based top down RPG game. Written in C++ with Lua bindings.
Very much a work in progress.

Dependencies
------------

Requires 
- OpenGL
- OpenAL
- LibOGG
- LibVorbis
- LibDevil

Windows
-------

Requires Microsoft Visual Studio 2010.
 
Linux
-----

Additional requires FreeGlut for Linux.

    sudo apt-get install libopenal-dev libdevil-dev libogg-dev freeglut3-dev
    
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
