Simple Ball Physics Simulation
Eduardo Rodrigues - emr4378

-----------------
Introduction
-----------------
-----------------

The project was written for my Computer Graphics I course as the final project.
It was written in C++ and uses freeglut (http://freeglut.sourceforge.net/).

It is a simple rigid-body ball/particle physics simulation, where each ball
attracts every other ball based of a mass value (which is typically related to
the balls' radius). Theres some basic interaction available; it's all listed in
the code and displayed when running.


-----------------
Building
-----------------
-----------------

Should be able to build on Windows/Linux/Mac without changing any code.
You just need to link the glut (freeglut), GLU and GL (OpenGL) libraries.
There's an example Makefile included.

On Linux it'd be something like: "-lglut -lGLU -lGL"
On Windows, you'd want to link these: "opengl32.lib; glut32.lib; glu32.lib;"