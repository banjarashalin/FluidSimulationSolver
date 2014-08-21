#version 120

/// @file Colour.fs
/// @brief a basic color shader

/// @brief the final colour to shade with
varying vec4 outColor;

void main()
{
    gl_FragColor = outColor;
}
