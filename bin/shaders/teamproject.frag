#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec3 norm;
in vec2 tc;

// the only output variable
out vec4 fragColor;

uniform bool b_solid_color;
uniform vec4 solid_color;
uniform sampler2D	TEX;

void main()
{
		fragColor = b_solid_color ? solid_color : texture(TEX, tc);
}
