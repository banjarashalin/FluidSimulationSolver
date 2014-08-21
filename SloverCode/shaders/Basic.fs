#version 120

/// @file Basic.fs
/// @brief a basic diffuse light shader

/// @brief the vertex normal
varying vec3 fragmentNormal;

/// @brief the final colour to shade with
varying vec4 outColor;

void main ()
{
    // normalize the vertex normal
    vec3 N = normalize(fragmentNormal);

    // get the Light vector
    vec3 L = normalize(gl_LightSource[0].position.xyz);

    // get the halfway vector
    vec3 H = normalize(gl_LightSource[0].halfVector.xyz);

    // calculate diffuse based on Lambert's law (L.N)
    vec4 diffuse = gl_FrontMaterial.diffuse  *gl_LightSource[0].diffuse * max(dot(L, N), 0.0);

    //finally multiply our base constant colour with the diffuse light colour component
    gl_FragColor = outColor * diffuse;
}
