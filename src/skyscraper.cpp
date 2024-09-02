#include "skyscraper.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Skyscraper::_bind_methods() {
}

Skyscraper::Skyscraper() {
	// Initialize any variables here.
	time_passed = 0.0;
}

Skyscraper::~Skyscraper() {
	// Add your cleanup here.
}

void Skyscraper::_process(double delta) {
	time_passed += delta;

	//Vector3 new_position = Vector2(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)));

	//set_position(new_position);
}
