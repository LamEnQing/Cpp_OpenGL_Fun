# What is this?
OpenGL Sandbox is a project where I can quickly write up codes or test codes from websites or Youtube videos.

In an isolated environment, solely focused on OpenGL, with no game engine abstraction over it. It makes the process of testing codes easier.

[Dear ImGui](https://github.com/ocornut/imgui) is also integrated into this project to quickly create GUIs to test the codes in a dynamic or fluid manner.

# I want to see your codes!
The codes are grouped into their own source files, such as BatchRendering, StarSystem, etc. They can be found in the `src` folder.

# What and Where did you implement XXX?
* IBatchRenderer, BatchRendering, GridsBatchRendering  
An attempt at Batch Rendering, using these video guides from [The Cherno](https://www.youtube.com/watch?v=Th4huqR77rI&list=PLlrATfBNZ98f5vZ8nJ6UengEkZUMC4fy5)
* StarSystem  
An attempt to implement basic lighting and a perspective-view camera that orbits around the world origin.  
For basic lighting, I followed a guide from [LearnOpenGL](https://learnopengl.com/Lighting/Basic-Lighting).
To calculate the position of the orbiting camera, I followed a guide from [Megabyte Softworks](https://www.mbsoftworks.sk/tutorials/opengl4/026-camera-pt3-orbit-camera/).
