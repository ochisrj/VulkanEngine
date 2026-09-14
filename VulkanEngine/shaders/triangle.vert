#version 450

// No vertex inputs - positions/colors are hardcoded and indexed by gl_VertexIndex.
// Using local const arrays with explicit size [3] avoids the
// "'[]' : scalar integer expression required" error on strict GLSL compilers.

layout(location = 0) out vec3 fragColor;

void main()
{
    const vec2 positions[3] = vec2[3](
        vec2( 0.0, -0.5),
        vec2( 0.5,  0.5),
        vec2(-0.5,  0.5)
    );

    const vec3 colors[3] = vec3[3](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );

    // gl_VertexIndex is the Vulkan GLSL built-in (requires #version 450 + Vulkan).
    // Do NOT use gl_VertexID (that's OpenGL). Requires glslc / glslangValidator -V.
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor   = colors[gl_VertexIndex];
}
