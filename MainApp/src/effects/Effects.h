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
	
	void reset(); 
	void applyEffect(const string& fx);
	void invert(bool apply);
	void mirror(bool apply);
	void flip(bool apply);
	void pixelate(bool apply, float x, float y);
	void shake(bool apply, float speed, float displace, float numWaves);
	void bounce(bool apply, float seconds, float number, float amplitude);
	
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
	float bounce_untill;
	float bounce_duration;
	bool bounce_enabled;
	
};
#endif