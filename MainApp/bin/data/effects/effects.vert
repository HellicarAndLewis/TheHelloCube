
attribute vec4 pos;
attribute vec2 tex;
varying vec2 texcoord;

void main() {
    gl_Position = pos;
	texcoord = tex;
}
