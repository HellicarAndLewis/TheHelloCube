#include "rxVeins.h"

rxVeins::rxVeins() {
}

rxVeins::~rxVeins() {
}

void rxVeins::addSource(rxParticle* p) {
	sources.push_back(p);
	sources_ps.addParticle(p);
}

void rxVeins::addRoot(rxParticle* p) {
	roots.push_back(p);
}

void rxVeins::step() {
	printf("Num sources: %zu, num roots: %zu\n", sources.size(), roots.size());
	vector<rxParticle*>::iterator sit = sources.begin();
	vector<rxParticle*>::iterator rit = roots.begin();
	
	sources_to_nodes.clear();
	while(rit != roots.end()) {
		(*rit)->reset();
		++rit;
	}
	
	// for each source search the particles which is closest auxin sources.
	ofVec3f dir(0,0,0);
	float length_sq = 0.0;
	while(sit != sources.end()) {
		rxParticle& source = *(*sit);
		
		// do we need to remove this auxin source?
		if(source.lifetime < 0) {
			sources_ps.removeParticle(*sit);
			delete *sit;
			sit = sources.erase(sit);
			continue;
		}
		
		// find the closest root for this source
		rit = roots.begin();
		float closest_dist = FLT_MAX;
		rxParticle* closest_node = NULL;
		
		while(rit != roots.end()) {
			rxParticle& root = *(*rit);
			dir = root.position - source.position;
			length_sq = dir.lengthSquared();
			if(length_sq < closest_dist) {
				closest_dist = length_sq;
				closest_node = *rit;
			}
			++rit;
		}
		
		if(closest_node != NULL) {
			closest_node->lifetime = 5;
			sources_to_nodes[*sit].push_back(closest_node);
			closest_node->neighbors.push_back(*sit);
		}
		++sit;
	}
	
	// move roots towards auxin sources.
	float min_dist = 2;
	float min_dist_sq = min_dist*min_dist;
	float dist_sq = 0;
	float num_sources = 0.0f;
	float D = 3;
	ofVec3f average(0,0,0);
	
	rit = roots.begin();
	vector<rxParticle*> new_roots;
	while(rit != roots.end()) {
		rxParticle& root = *(*rit);
		if(root.lifetime != 5) {
			delete *rit;
			rit = roots.erase(rit);
//			++rit;
			continue;
		}
		average.set(0,0,0);
		num_sources = 0;
		vector<rxParticle*>::iterator nit = root.neighbors.begin();
		while(nit != root.neighbors.end()) {
			dir = (*nit)->position - root.position;
			dist_sq = dir.lengthSquared();
			average += dir;
			num_sources++;
			
			// mark for deletion.
			if(dist_sq < min_dist_sq) {
				(*nit)->lifetime = -1;
			}
			++nit;
		}
		average /= num_sources;
		average.normalize();
		average *= D;
		average += root.position;
		rxParticle* new_root = new rxParticle(average, 0);
		new_roots.push_back(new_root);
		nodes.push_back(average);
		++rit;
	}
	
	for(int i = 0; i < new_roots.size(); ++i) {
		addRoot(new_roots[i]);
	}
}

void rxVeins::draw() {
	vector<rxParticle*>::iterator it = sources.begin();
	while(it != sources.end()) {
		rxParticle& p = *(*it);
		glColor3f(1,1,1);
		ofCircle(p.position.x, p.position.y, 3);
		glColor3f(0.22,0.33,0.44);
		ofCircle(p.position.x, p.position.y, 2);
		++it;
	}

	
	glColor3f(0,1,1);
	vector<rxParticle*>::iterator rit = roots.begin();
	while(rit != roots.end()) {
		rxParticle& p = *(*rit);
		glColor3f(1,1,1);
		ofCircle(p.position.x, p.position.y, 2);
		glColor3f(1,0.0,0.0);
		ofCircle(p.position.x, p.position.y, 1);
		++rit;
	}
	
	glColor3f(1,1,1);
	glBegin(GL_POINTS);
	vector<ofVec3f>::iterator nit = nodes.begin();
	while(nit != nodes.end()) {
		glVertex3fv(&nit->x);
		//ofCircle(nit->x, nit->y, 3);
		++nit;
	}
	glEnd();

	glBegin(GL_LINES);
	map<rxParticle*, list<rxParticle*> >::iterator mit = sources_to_nodes.begin();
	while(mit != sources_to_nodes.end()) {
		glColor3f(1,0,0);
		rxParticle& a = *(mit->first);
		glVertex3fv(&a.position.x);
		list<rxParticle*>::iterator lt = mit->second.begin();
		while(lt != mit->second.end()) {
			rxParticle& b = *(*lt);
			glColor3f(1,1,1);
			glVertex3fv(&b.position.x);
			++lt;
		}
		++mit;
	}
	
	glEnd();
}

void rxVeins::update() {
	sources_ps.repel(0.1);
	sources_ps.update();
}