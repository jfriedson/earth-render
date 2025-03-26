# Earth Renderer in C++ and OpenGL
An old tessellation experiment

![Screenshot of Earth Render depicting the Americas at Dusk](readme_assets/demo.webp?raw=true)

This project achieves a relatively convincing rendering of the Earth from space on an unrealistically clear day and with exagerated height mapping by layering multiple high resolution textures (8k-16k resolution) sourced from public scientific publications and processing them using vertex, tessellation, and fragment shaders to achieve:

### Features
- Tessellation
    + Considers the local complexity of the height-map texture in addition to the distance that the patch edge is from the camera before introducing new vertices for efficiency
    + Culling based on patch face normal
    + Fractional tessellation for smooth transitions
    + Height mapping performed in eval shader
- Normal mapping  
- Shadow mapping  
- Specular, Ambient, and Diffuse lighting  
- Night/Day Cycle depicting the light pollution texture at night time
- Clouds
    + rendered on a larger, non-tessellated Earth model with a single, patterned texture
    + clouds match the Earth's rotation but slowly scroll vertically to add variation
- (TODO) parallax skybox effect
- (TODO) find light pollution map without relief mapping

### Controls
mouse - pivot the camera around Earth  
arrows - move through day-night cycle  
esc - quit

### Libraries Utilized
GLFW - Window creation and hardware peripherals communication  
GL3W - Expose OpenGL API  
GLM - OpenGL math types and methods  
Freetype - Font loading and rendering  
