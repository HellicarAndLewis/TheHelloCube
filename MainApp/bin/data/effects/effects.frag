uniform sampler2D img;
varying vec2 texcoord;
void main() {
	vec4 color = texture2D(img, texcoord);
	gl_FragColor = color;

}
