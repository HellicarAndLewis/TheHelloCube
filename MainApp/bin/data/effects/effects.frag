#define TWO_PI 6.2831853
#define PI 3.14159265
uniform int fx_pixelate;
uniform int fx_wave;
uniform int fx_invert;
uniform int fx_shake;
uniform int fx_swirl;
uniform int fx_ripple;
uniform int fx_posterize;
uniform int fx_reflect;
uniform int fx_cracks;

uniform float fx_time;
uniform float fx_shake_p;
uniform float fx_shake_number;
uniform float fx_shake_amplitude;
uniform float fx_pixelate_x;
uniform float fx_pixelate_y;
uniform float fx_wave_num;
uniform float fx_wave_speed;
uniform float fx_wave_displace;
uniform float fx_swirl_radius;
uniform float fx_swirl_angle;
uniform vec3 fx_ripple_params;
uniform float fx_ripple_p;

uniform vec2 center;
uniform sampler2D img;
varying vec2 texcoord;


void main() {
	vec4 color;
	vec2 tc = texcoord;

	if(fx_pixelate == 1) {
		float dx = fx_pixelate_x * (1./1024.);
		float dy = fx_pixelate_y * (1./768.);
		tc = vec2(dx*floor(tc.x/dx), dy*floor(tc.y/dy));
	}
	
	if(fx_wave == 1) {
		tc.x += sin(tc.y * TWO_PI * fx_wave_num + fx_time * fx_wave_speed) * fx_wave_displace;
	}
	
	if(fx_shake == 1) {
		tc.x += cos(fx_shake_number*fx_shake_p*PI) * fx_shake_amplitude * sin(fx_shake_p*PI);
	}
	
	if(fx_swirl == 1) {
		tc -= center;
		float dist = length(tc);
		if(dist < fx_swirl_radius) {
			float p = (fx_swirl_radius - dist) / fx_swirl_radius;
			float angle = p * p * fx_swirl_angle;
			float s = sin(angle);
			float c = cos(angle);
			tc = vec2(
				 tc.x * c - tc.y * s
				,tc.x * s + tc.y * c
			);
			
		}
		tc += center;
	}

	if(fx_ripple == 1) {
		float d = distance(tc, center);
		if( (d <= (fx_ripple_p + fx_ripple_params.z)) &&
			(d >= (fx_ripple_p - fx_ripple_params.z)) )
		{
			float diff = d - fx_ripple_p;
			float pow_diff = 1.0 - pow(abs(diff * fx_ripple_params.x), fx_ripple_params.y);
			float time_diff = diff * pow_diff;
			tc += normalize(tc - center) * time_diff;
		}
	}
	if(fx_reflect == 1) {
		if(tc.s > center.x) {
			tc.s = center.x - (tc.s - center.x );
		}
	}
	
	color = texture2D(img,tc);

	// kaleidoscope test (too much)
	/*
	int kal = 1;
	if(kal == 1) {
		vec2 p = -1.0 + (tc * 2.0)	;
		float a = atan(p.y,p.x);
		float r = sqrt(dot(p,p));
		tc.x =  3.0*a/3.1416;
		tc.y = -fx_time+ sin(3.0*r+fx_time) + .3*cos(fx_time+3.0*a);
		float w = .8+.3*(sin(fx_time+3.0*r)+ .3*cos(fx_time+3.0*a));
		vec3 col = texture2D(img, tc* 0.9).xyz;
		color = vec4(col * w, 1.0);
	}
	*/
	
	if(fx_posterize == 1) {
		vec3 c = vec3(color);
		float gamma = 0.6;
		float num_colors = 8.0;
		c = pow(c, vec3(gamma, gamma, gamma));
		c = c * num_colors;
		c = floor(c);
		c = c / num_colors;
		c = pow(c, vec3(1.0/gamma));
		color.rgb = c;
	}
	
	if(fx_invert == 1) {
		color.r = max(0.0, 1.0 - color.r);
		color.g = max(0.0, 1.0 - color.g);
		color.b = max(0.0, 1.0 - color.b);
	}
	if(fx_cracks == 1) {
		if(color.r != 0.1 && color.g != 0.1 && color.b != 0.1) {
			color += 0.7;
		}
	}
	gl_FragColor = color;
}
