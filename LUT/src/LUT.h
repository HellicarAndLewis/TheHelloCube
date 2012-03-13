#ifndef ROXLU_LUTH
#define ROXLU_LUTH

#include <fstream>

namespace roxlu {
class LUT {
public:
	LUT();
	~LUT();
	
	void createTexture(const string& path, int dim);
	
private:
};
} // roxlu