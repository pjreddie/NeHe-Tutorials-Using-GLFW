#NeHe Tutorials Using GLFW

NeHe has some pretty awesome tutorials for learning OpenGL. The original code is all in Visual C++ using native Windows windowing. This is my port of that code to C using GLFW for windowing.

##Setup

To get this running you will need to install the [Simple OpenGL Image Library](http://www.lonesock.net/soil.html), [GLFW](http://www.glfw.org/).

To install SOIL you will probably have to modify the given Makefile included in `projects/makefile`. On OS X I had to modify the `CXX` line to read:

    CXX = gcc -arch i386 -arch x86_64


##Usage

Each individual tutorial is in its own file. There is a Makefile in the top-level directory that should compile everything just by using `make` (it is written for OS X but you should be able to change the libraries to get it to work on your system).

The finished binaries can be run from the tutorial's folder or from the top-level folder since a data directory exists in both places (files in the code are opened with relative pathing). When you try it out you should see something like this:

    $ make
        cc -Wall  -lglfw ... tutorial_1/tutorial_1.c -o tutorial_1/run

            ... [A few more lines of this] ...

        cc -Wall  -lglfw ... tutorial_10/tutorial_10.c -o tutorial_10/run

    $ tutorial_10/run
        [Window opens with OpenGL Stuff, Huray!]
    
There are some differences in my code vs. NeHe's original code so this is really just meant as a refenece as opposed to a direct port. I was having trouble locating any sample code for doing NeHe's tutorials using GLFW so once I got it working I decided to post what I had so others could use it as well. I hope you find it useful!
