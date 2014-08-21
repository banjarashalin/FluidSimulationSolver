#version 120

/// @file Colour.vs
/// @brief a basic colour vertex shader

/// @brief the projection matrix from the camera
uniform mat4 projectionMatrix;

/// @brief the view matrix matrix from the camera
uniform mat4 ViewMatrix;

/// @brief the model matrix obtained from the transformations
uniform mat4 ModelMatrix;

/// @brief the vertex color
uniform vec4 inColor = vec4(0, 1, 0, 1);

/// @brief the current vertex position
attribute vec3 inVertex;

/// @brief the interpolated color to shade fragment with
varying vec4 outColor;

void main()
{
    gl_Position = projectionMatrix*ViewMatrix*ModelMatrix * vec4(inVertex.xyz, 1.0);

    outColor = inColor;
}
