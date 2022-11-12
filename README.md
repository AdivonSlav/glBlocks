# glBlocks

This is a voxel engine I've been working on to advance my knowledge in OpenGL. For this project I used OpenGL 4.6 Core-profile and C++20 (so I could take advantage of the new std::filesystem first and foremost).

## Terrain generation
Terrain is generated on the fly and stored as chunk files on disk, theoretically allowing for infinite terrain. Due to type limitations, for now chunk positions cannot pass ±32767 in X or Z position (the max amount of chunks that can be generated and stored is then equal to roughly 16 million), however that can probably be changed by sacrificing some memory usage. Obviously, another limitation is disk space for the chunks, one chunk equals 33KB. As mentioned earlier, the terrain height map is calculcated using 2D Perlin noise and then altered with a custom seed implementation (albeit a primitive one) in order to achieve a level of randomness between generation runs. The chunk files are encoded with this seed value, alongside the chunk structure information, so using the same files it is possible to continue generating seed-appropriate terrain. Chunks are loaded and unloaded into/from memory based on the distance between the chunks and the camera and are also rendered depending on the distance. Using std::future I was able to port all of the logic relating to building chunk vertex data and Perlin generation into separate thread calls, thereby unblocking the main thread.

## Lighting
I implemented lighting for the voxels using the Phong lighting method. Ambient lighting is added first, the strength of which depends on the time of day (i.e. the position of the sun/moon). Then, diffuse lighting is calculated based on the light vector and the surface normal of each voxel face. Normals are not passed via buffers to the vertex shader (in order to conserve memory) but are rather calculated by passing information on what face the vertex comprises (if the vertex is on the top face of the voxel, then the corresponding normal vector is just a vector with a Y value of 1). Finally, specular lighting is calculated to simulate reflection of diffuse light based on how the camera views the voxel surface. 

## Planned features

- Breaking and placing of blocks
- ~~Day/night cycle~~
- ~~Multithreaded chunk generation and loading~~
- ~~Chunk persistence between application runs (stored as files)~~
- Fog and ~~lighting effects~~
- 3D Perlin noise for ores

The libraries that I used are:

- [GLAD](https://github.com/Dav1dde/glad) for loading the necessary OpenGL functions
- [stb_image](https://github.com/nothings/stb) for image loading
- [GLM](https://github.com/g-truc/glm) for mathematical types and operations
- [Dear ImGui](https://github.com/ocornut/imgui) for UI overlay elements

## Controls

- WASD - Movement
- Right click (Hold) - Pan camera
- Left shift (Hold) - Accelerate movement
- Left ctrl (Hold) - Downward movement

## Build instructions

Simply run 'genproj.bat' which will generate the necessary project files for Visual Studio 2022 using Premake5. For now I'm only supporting Visual Studio 2022 and MSVC.
