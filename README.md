#NeHe Tutorials Using GLFW

NeHe has some pretty awesome tutorials for learning OpenGL. The original code is all in Visual C++ using native Windows windowing. This is my port of that code to C using GLFW for windowing.

##Setup

To get this running you will need to install the [Simple OpenGL Image Library](http://www.lonesock.net/soil.html), [GLFW](http://www.glfw.org/).

To install SOIL you will probably have to modify the given Makefile included in `projects/makefile`. On OS X I had to modify the `CXX` line to read:

    CXX = gcc -arch i386 -arch x86_64


##Usage

Each individual tutorial is under it's own branch so if you want to look at Tutorial 5, for example you would clone the repository and then run:

    git checkout tutorial_5

There is a Makefile included that works on OS X and can probably be modified to work on other systems as well by changing the linked libraries.

There are some differences in my code vs. NeHe's original code so this is really just meant as a refenece as opposed to a direct port. I was having trouble locating any sample code for doing NeHe's tutorials using GLFW so once I got it working I decided to post what I had so others could use it as well. I hope you find it useful!
