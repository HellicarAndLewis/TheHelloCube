uniform int fx_pixelate;
uniform int fx_shake;
uniform float fx_pixelate_x;
uniform float fx_pixelate_y;
uniform float fx_time;
uniform float fx_shake_waves;
uniform float fx_shake_speed;
uniform float fx_shake_displace;
uniform sampler2D img;
varying vec2 texcoord;
#define TWO_PI 6.2831853

void main() {
	vec4 color;
	vec2 tc = texcoord;
	if(fx_pixelate == 1) {
		float dx = fx_pixelate_x * (1./1024.);
		float dy = fx_pixelate_y * (1./768.);
		tc = vec2(dx*floor(tc.x/dx), dy*floor(tc.y/dy));
	}
	
	if(fx_shake == 1) {
		tc.x += sin(tc.y * TWO_PI * fx_shake_waves + fx_time * fx_shake_speed) * fx_shake_displace;
	}
	
	color = texture2D(img, tc);		
	gl_FragColor = color;
}
