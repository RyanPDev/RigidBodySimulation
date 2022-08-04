# Rigid Body Simulation C++/OpenGL

The main objective is to implement a simple rigid body simulation. Only one object will
be simulated, a cube. It has to start at a random location, with a random orientation inside
the simulation box. At the start, a random ascending force is applied to some random point
within the cube (doesn’t have to be one of the cube’s vertices, necessarily). The other force in
action is gravity.
The result is a rotating cube in air which collides with ground and walls, responding
adequately.
After the simulation runs for 15 seconds, it should reinitialize: the cube again at a random
location within the simulation box and a random ascending force applied to it.

University Physics Project C++/OpenGL by Ryan Palazón & Marc Romera
