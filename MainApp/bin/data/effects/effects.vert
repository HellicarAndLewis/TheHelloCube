uniform int fx_mirror;
uniform int fx_flip;

attribute vec4 pos;
attribute vec2 tex;
varying vec2 texcoord;

void main() {
    gl_Position = pos;
	
	texcoord = tex ;
	
	if(fx_mirror == 1) {
		texcoord.x = 1.0 - texcoord.x;
	}
	
	if(fx_flip == 1) {
		texcoord.y = 1.0 - texcoord.y;
	}
}
