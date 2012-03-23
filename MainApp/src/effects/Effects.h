/**
 * Post FX
 * 
 * 
 * 
 * @date	2012.03.xx
 * @author	Diederick Huijbers <diederick@apollomedia.nl>
 */
#ifndef ROXLU_EFFECTSH
#define ROXLU_EFFECTSH

#include "ofMain.h"
#include "rxCracks.h"

struct Vertex {
	float pos[3];
	float tex[2];
	
	Vertex(){}	
	void setPos(float x, float y, float z) { pos[0] = x; pos[1] = y; pos[2] = z; };
	void setTex(float s, float t) { tex[0] = s; tex[1] = t;}
};

class Effects {
public:
	Effects();
	~Effects();
	void setup(int w, int h);
	void update();
	void draw();
	void beginGrabPixels();
	void endGrabPixels();	
	void bind();
	void unbind();
	void calcCenter();
	
	void reset(); 
	void applyEffect(const string& fx);
	void invert(bool apply);
	void mirror(bool apply);
	void flip(bool apply);
	void pixelate(bool apply, float x, float y);
	void wave(bool apply, float speed, float displace, float num);
	void shake(bool apply, float seconds, float number, float amplitude);
	void swirl(bool apply, float radius, float angle);
	void ripple(bool apply, float seconds);
	void reflect(bool apply);
	void posterize(bool apply);
	void crack(bool apply);
	void love(bool apply);
	
private:
	ofShader shader;
	ofFbo fbo;
	GLuint vbo;
	GLuint vao;
	GLuint fbo_tex;
	GLuint fbo_handle;
	GLuint fbo_depth;
	vector<Vertex> vertices;
	bool cleared;
	int width;
	int height;
	
	float shake_untill;
	bool shake_enabled;
	bool ripple_enabled;
	bool cracks_enabled;
	float ripple_duration;
	float ripple_untill;
	rxCracks cracks;

public:
	// quickfixes
	float cx; 
	float cy; 
	float fx_flip_adjust_y;
	float fx_love_scale_x;
	float fx_love_scale_y;
	float fx_love_x;
	float fx_love_y;
	
	// used when calling applyEffect
	float shake_duration;
	float shake_number;
	float shake_amplitude;
	float pixelate_x;
	float pixelate_y;
	float wave_displace;
	float wave_num;
	float wave_speed;
	float swirl_radius;
	float swirl_angle;
};
#endif