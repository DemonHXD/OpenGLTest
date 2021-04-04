
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "engine/engine.h"
using namespace std;

int main() {
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	//std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

	if (!engine->initWindow(800, 600)) {
		return -1;
	}

	engine->run();
	engine->deleteRes();
	return 0;
}