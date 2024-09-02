#ifndef SKYSCRAPER_H
#define SKYSCRAPER_H

#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class Skyscraper : public Node3D {
	GDCLASS(Skyscraper, Node3D)

private:
	double time_passed;

protected:
	static void _bind_methods();

public:
	Skyscraper();
	~Skyscraper();

	void _process(double delta) override;
};

}

#endif
