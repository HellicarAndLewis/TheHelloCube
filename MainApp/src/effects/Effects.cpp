#include "AppAssets.h"
#include "Effects.h"
#include "Error.h"
#include <cstdlib>

Effects::Effects()
	:vertices(4, Vertex())
	,cleared(false)
	,shake_enabled(false)
	,shake_untill(0)
	,shake_duration(0)
	,ripple_enabled(false)
	,ripple_duration(0)
	,ripple_untill(0)
	,cracks_enabled(false)
	,width(0)
	,height(0)
{
}

Effects::~Effects() {
}

void Effects::setup(int w, int h) {
	width = w;
	height = h;
	
	cracks.setup(w,h);
	
	// create fbo
	glGenFramebuffers(1, &fbo_handle); eglGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
	
	// create texture.
	glGenTextures(1, &fbo_tex); eglGetError();
	glActiveTexture(GL_TEXTURE0); eglGetError();
	glBindTexture(GL_TEXTURE_2D, fbo_tex); eglGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); eglGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); eglGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); eglGetError();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex, 0); eglGetError();
	glGenRenderbuffers(1, &fbo_depth); eglGetError();

	// render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth); eglGetError();	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth); eglGetError();

	GLenum drawbufs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawbufs);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	if(!shader.load("effects/effects")) {
		printf("Error loading effects shader.\n");
	}
	
	vertices[0].setPos(-1, -1, 0);
	vertices[1].setPos(1, -1, 0);
	vertices[2].setPos(1, 1, 0);
	vertices[3].setPos(-1, 1, 0);
	
	float mw = 1;
	float mh = 1;
	vertices[0].setTex(0, 0);
	vertices[1].setTex(mw, 0);
	vertices[2].setTex(mw, mh);
	vertices[3].setTex(0, mh);
	
	glGenVertexArraysAPPLE(1, &vao); eglGetError();
	glBindVertexArrayAPPLE(vao); eglGetError();

	GLint pos_attrib = glGetAttribLocation(shader.getProgram(), "pos");
	GLint tex_attrib = glGetAttribLocation(shader.getProgram(), "tex");
	glEnableVertexAttribArray(pos_attrib);
	glEnableVertexAttribArray(tex_attrib);

	glGenBuffers(1, &vbo); eglGetError();
	glBindBuffer(GL_ARRAY_BUFFER, vbo); eglGetError();
	
	glBufferData(
		GL_ARRAY_BUFFER	
		,sizeof(Vertex) * 4
		,vertices[0].pos
		,GL_STATIC_DRAW
	); eglGetError();
	
	glVertexAttribPointer(
		pos_attrib
		,3
		,GL_FLOAT
		,GL_FALSE
		,sizeof(Vertex)
		,offsetof(Vertex, pos)
	);
	
	glVertexAttribPointer(
		tex_attrib
		,2
		,GL_FLOAT
		,GL_FALSE
		,sizeof(Vertex)
		,(GLvoid*)offsetof(Vertex, tex)
	);
	
	calcCenter();	
	
	unbind();
	
	flip(false);
	mirror(false);	
	crack(false);
}

void Effects::calcCenter() {
#ifndef USE_SMALL_APP
	float total_w = CAMERA_PROJECTION_SCREEN_WIDTH + CUBE_SCREEN_WIDTH;
	cx = (((float)CUBE_SCREEN_WIDTH) * 0.5/total_w);
	cy = (1.0 - ((float)(CUBE_SCREEN_HEIGHT * 0.5)) /((float)CAMERA_PROJECTION_SCREEN_HEIGHT));
#else
	cx = 0.5;
	cy = 0.5;
#endif

}

void Effects::beginGrabPixels() {
	ofEnableNormalizedTexCoords();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,width, height);
}

void Effects::endGrabPixels() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,ofGetWidth(), ofGetHeight());
}

void Effects::update() {
	shader.begin();

		shader.setUniform2f("center", cx, cy);
		shader.setUniform1f("fx_flip_adjust_y", fx_flip_adjust_y);
	
		float now = ofGetElapsedTimef();
		shader.setUniform1f("fx_time", now);
		if(shake_enabled) {
			float p = 1.0 -  MIN(1.0, (shake_untill-now)/shake_duration);
			if(p >= 1.0) {
				shake_enabled = false;
				shader.setUniform1i("fx_shake", 2);
			}
			shader.setUniform1f("fx_shake_p", p);
		}
		
		if(ripple_enabled) {
			float p = 1.0 - MIN(1.0, (ripple_untill-now)/ripple_duration);
			if(p >= 1.0) {
				ripple_enabled = false;
				shader.setUniform1i("fx_ripple", 2);
			} 
			shader.setUniform1f("fx_ripple_p",p);
		}
		
	shader.end();
}

void Effects::draw() {
	ofEnableNormalizedTexCoords();
	bind();
		glDrawArrays(GL_QUADS, 0, 4);
	unbind();
	
	if(cracks_enabled) {
		cracks.draw();
	}

}

void Effects::bind() {
	glBindVertexArrayAPPLE(vao);
	shader.begin();

	glActiveTexture(GL_TEXTURE0); 
	shader.setUniform1i("img", 0);
	glBindTexture(GL_TEXTURE_2D, fbo_tex);
}

void Effects::unbind() {
	shader.end();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArrayAPPLE(0);
}

void Effects::mirror(bool apply) {
	shader.begin();
		shader.setUniform1i("fx_mirror", apply ? 1: 2);
	shader.end();
}

void Effects::flip(bool apply) {
	shader.begin();
		shader.setUniform1i("fx_flip", apply ? 1: 2);
	shader.end();
}

void Effects::wave(bool apply, float speed, float displace, float num) {
	shader.begin();
		shader.setUniform1i("fx_wave", apply ? 1: 2);
		shader.setUniform1f("fx_wave_displace", displace);
		shader.setUniform1f("fx_wave_speed", speed);
		shader.setUniform1f("fx_wave_num", num);
	shader.end();
}

void Effects::pixelate(bool apply, float x, float y) {
	shader.begin();
		shader.setUniform1i("fx_pixelate", apply ? 1: 2);
		shader.setUniform1f("fx_pixelate_x", x);
		shader.setUniform1f("fx_pixelate_y", y);
	shader.end();
}

void Effects::posterize(bool apply) {
	shader.begin();
		shader.setUniform1i("fx_posterize", apply ? 1: 2);
	shader.end();
}

void Effects::crack(bool apply) {
	cracks_enabled = apply;
	
	shader.begin();
		shader.setUniform1i("fx_cracks", apply ? 1: 2);
	shader.end();
	
	if(apply) {
		cracks.clear();
		cracks.generateCracks();
	}
}


// radius: [0-1]
// angle: 0 - ?? PI
void Effects::swirl(bool apply, float radius, float angle) {
	shader.begin();
		shader.setUniform1i("fx_swirl", apply ? 1 : 2);
		shader.setUniform1f("fx_swirl_radius", radius);
		shader.setUniform1f("fx_swirl_angle", angle);
	shader.end();
}

void Effects::ripple(bool apply, float seconds) {
	ripple_enabled = apply;
	ripple_duration = seconds;
	ripple_untill = ofGetElapsedTimef() + seconds;
	shader.begin();
		shader.setUniform1i("fx_ripple", apply ? 1 : 2);
		shader.setUniform3f("fx_ripple_params", 10.0, 0.9, 0.1);
	shader.end();
}

void Effects::invert(bool apply) {
	shader.begin();
		shader.setUniform1i("fx_invert", apply ? 1: 2);
	shader.end();
}

void Effects::reflect(bool apply) {
	shader.begin();
		shader.setUniform1i("fx_reflect", apply ? 1: 2);
	shader.end();
}

void Effects::love(bool apply) {
	shader.begin();
		shader.setUniform1i("fx_love", apply ? 1: 2);
	shader.end();
}

void Effects::shake(bool apply, float seconds, float number, float amplitude) {
	shake_untill = ofGetElapsedTimef() + seconds;
	shake_enabled = apply;
	shake_duration = seconds;
	shader.begin();
		shader.setUniform1i("fx_shake", apply ? 1: 2);
		shader.setUniform1f("fx_shake_number", number);
		shader.setUniform1f("fx_shake_amplitude", amplitude);
	shader.end();

}

void Effects::applyEffect(const string& fx) {
	
	if(fx == "flip") {
		flip(true);
	}
	else if(fx == "mirror") {
		mirror(true);
	}
	else if(fx == "invert") {
		invert(true);
	}
	else if(fx == "ripple") {
		ripple(true, 3.5);
	}
	else if(fx == "posterize") {
		posterize(true);
	}
	else if(fx == "pixelate") {
		pixelate(true, pixelate_x, pixelate_y);
	}
	else if(fx == "wave") {
		wave(true, wave_speed, wave_displace, wave_num);
	}	
	else if(fx == "swirl") {
		swirl(true, swirl_radius, swirl_angle);
	}
	else if(fx == "shake") {
		shake(true, shake_duration, shake_number, shake_amplitude);
	}
	else if(fx == "reflect") {
		reflect(true);
	}
	else if(fx == "crack") {
		crack(true);
	}
	else if(fx == "love") {
		love(true);
	}
}

void Effects::reset() {
	crack(false);
	shader.begin();
		shader.setUniform1i("fx_flip", 2);
		shader.setUniform1i("fx_mirror", 2);
		shader.setUniform1i("fx_invert", 2);
		shader.setUniform1i("fx_ripple", 2);
		shader.setUniform1i("fx_posterize", 2);
		shader.setUniform1i("fx_pixelate", 2);
		shader.setUniform1i("fx_wave", 2);
		shader.setUniform1i("fx_swirl", 2);
		shader.setUniform1i("fx_reflect", 2);
		shader.setUniform1i("fx_shake", 2);
		shader.setUniform1i("fx_love", 2);
	shader.end();
}