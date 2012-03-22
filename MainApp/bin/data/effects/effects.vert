uniform int fx_mirror;
uniform int fx_flip;
uniform float fx_flip_adjust_y;
uniform vec2 center;

attribute vec4 pos;
attribute vec2 tex;
varying vec2 texcoord;

void main() {
    gl_Position = pos;
	
	texcoord = tex ;
	
	if(fx_mirror == 1) {
		float w = center.x * 2.0;
		//texcoord.x = 0.165 + (w - (texcoord.x));
		
		texcoord.x = w - texcoord.x;
		//gl_Position.x *= -1.0;
	}
	
	if(fx_flip == 1) {
		float h = center.y * 2.0;
		texcoord.y = fx_flip_adjust_y + (h -  texcoord.y);
	}
}
