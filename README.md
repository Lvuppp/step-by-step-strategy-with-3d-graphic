# step-by-step-strategy-with-3d-graphic
step-by-step strategy with 3d graphic
In our program we have developed a full-fledged graphical engine for the convenient design
of the game world. We based it on the mechanism of abstract classes and pure virtual functions.

All further classes - Camera, SkyBox, Unit, Block - were inherited from it. This was done to
be ableto control the objects of different classes at the same time, keeping them in a single
array(the vector was instantiated by a pointer to the base abstract class, so we achieved the desired effect).

During the visualization process we also used GLSL shader language, which is integrated into Qt framework.
We wrote some vertex and segment shaders, and used the QOpenGLShaderProgram built-in class as the object that
loads them into the program, which gave us safe compilation and linking with methods that call rendering materials
and placing textures in video memory.
