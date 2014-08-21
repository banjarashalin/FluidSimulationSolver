#version 120

/// @file Basic.vs
/// @brief a basic vertex shader used with the diffuse light fragment shader

/// @brief the projection matrix from the camera
uniform mat4 projectionMatrix;

/// @brief the view matrix matrix from the camera
uniform mat4 ViewMatrix;

/// @brief the model matrix obtained from the transformations
uniform mat4 ModelMatrix;

/// @brief the vertex color
uniform vec4 inColor = vec4(0, 1, 0, 1);

/// @brief the vertice that is being shaded
attribute vec3 inVertex;

/// @brief the interpolated color to shade fragment with
varying vec4 outColor;

/// @brief vertex normal
varying vec3 fragmentNormal;

void main(void)
{
    fragmentNormal = (ViewMatrix*ModelMatrix*vec4(gl_Normal, 0.0)).xyz;

    gl_Position = projectionMatrix*ViewMatrix*ModelMatrix * vec4(inVertex.xyz, 1.0);

    outColor = inColor;
}
