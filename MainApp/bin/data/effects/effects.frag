uniform int fx_pixelate;
uniform float fx_pixelate_x;
uniform float fx_pixelate_y;
uniform sampler2D img;
varying vec2 texcoord;
void main() {
	vec4 color;
	
	if(fx_pixelate == 1) {
		float dx = fx_pixelate_x * (1./1024.);
		float dy = fx_pixelate_y * (1./768.);
		vec2 coord = vec2(dx*floor(texcoord.x/dx), dy*floor(texcoord.y/dy));
		color = texture2D(img, coord);		
	}
	else {
		color = texture2D(img, texcoord);
	}
	gl_FragColor = color;
}
