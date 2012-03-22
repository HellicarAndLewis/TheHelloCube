uniform int fx_mirror;
uniform int fx_flip;
uniform vec2 center;

attribute vec4 pos;
attribute vec2 tex;
varying vec2 texcoord;

void main() {
    gl_Position = pos;
	
	texcoord = tex ;
	
	if(fx_mirror == 1) {
		float w = center.x * 2.0;
		texcoord.x = (w - (texcoord.x));
	}
	
	if(fx_flip == 1) {
		float h = center.y * 2.0;
		texcoord.y = h -  texcoord.y;
	}
}
