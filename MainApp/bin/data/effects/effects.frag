#version 120
#extension GL_ARB_texture_rectangle : enable
//uniform sampler2DRect img;
uniform sampler2D img;
varying vec2 texcoord;
void main() {
//	vec4 color = texture2DRect(img, texcoord);
	vec4 color = texture2D(img, texcoord);
	gl_FragColor = color;
	//gl_FragColor.r *= 0.5;
	//gl_FragColor.rgb = vec3(1.0, 0.0, 0.5);
  	//gl_FragColor = mix(vec4(1.0, 1.0, 0.0, 1.0), color, 0.9);
}
