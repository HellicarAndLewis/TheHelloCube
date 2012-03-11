#include "Effects.h"
#include "Error.h"
#include <cstdlib>

Effects::Effects()
	:vertices(4, Vertex())
	,cleared(false)
{
}

Effects::~Effects() {
}

void Effects::setup(int w, int h) {
	width = w;
	height = h;
	
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
	
	unbind();
	
	flip(false);
	mirror(false);
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
	glViewport(0,0,width, height);
}

void Effects::update() {
	shader.begin();
		shader.setUniform1f("fx_time", ofGetElapsedTimef());
	shader.end();
}

void Effects::draw() {
	ofEnableNormalizedTexCoords();
	bind();
		glDrawArrays(GL_QUADS, 0, 4);
	unbind();
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

void Effects::shake(bool apply, float speed, float displace, float numWaves) {
	shader.begin();
		shader.setUniform1i("fx_shake", apply ? 1: 2);
		shader.setUniform1f("fx_shake_displace", displace);
		shader.setUniform1f("fx_shake_speed", speed);
		shader.setUniform1f("fx_shake_waves", numWaves);
	shader.end();
}

void Effects::pixelate(bool apply, float x, float y) {
	shader.begin();
		shader.setUniform1i("fx_pixelate", apply ? 1: 2);
		shader.setUniform1f("fx_pixelate_x", x);
		shader.setUniform1f("fx_pixelate_y", y);
	shader.end();
}