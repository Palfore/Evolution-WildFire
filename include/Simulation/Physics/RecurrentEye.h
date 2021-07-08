#ifndef RECURRENT_EYE_H
#define RECURRENT_EYE_H

#include "Bisection.h"
#include "Scenario.h"
#include "Terrain.h"
#include "Chunks.h"
#include "Shapes.h"
#include "Matrix.h"
#include "Vec.h"

#include <algorithm>
#include <optional>
#include <Math.h>

struct Viewable {
	double distance;
	double angle;
	Vec position;
	Viewable() : Viewable(0, 0, Vec(0, 0, 0)) {}
	Viewable(double d, double a, Vec p) : distance(d), angle(a), position(p) {}
};

class RecurrentEye {
 public:
	const double sight;
	std::vector<Viewable> seeing;
    RecurrentEye(const double sight_distance): sight(sight_distance), seeing({}) {}

	void look(const Vec pos, const Vec vel, const std::vector<Vec> viewable_objects) {
    	this->seeing.clear();
    	const double speed = vel.length();
		for (const auto& object: viewable_objects) {
		    const Vec dr = Vec(object.x, object.y, 0) - Vec(pos.x, pos.y, 0);
		    const double distance = dr.length();
		    if (distance <= this->sight) {
		    	const Vec normal = Vec(0, 0, 1); // on flat surface!
		    	if (distance < 1e-8 || speed < 1e-8) {
		    		this->seeing.push_back(
			    		Viewable(0, 0, Vec(0, 0, 0))
			    	);
		    	} else {
		    		const double unsigned_angle = acos(std::clamp(dr.dot(vel) / (distance * speed), -1.0, 1.0)) - PI;
			    	const double angle = unsigned_angle * sgn(normal.dot(vel.cross(dr)));
			    	this->seeing.push_back(
			    		Viewable(distance, angle, object)
			    	);
			    }
		    }
		}
		std::sort(this->seeing.begin(), this->seeing.end(), [](const Viewable& inputA, const Viewable inputB) {
	        return inputA.distance < inputB.distance;
	    });
	}

    void draw(const Vec& pos, const Vec& vel, int max_process) const {
    	DrawRing<Appearance::WHITE>(pos, this->sight, 1);
		for (const auto& object: this->seeing) {
	    	const Vec dr = object.position - pos;
	    	const double vel2DLength = Vec(vel.x, vel.y, 0).length() < 1e-8 ? 1e8 : Vec(vel.x, vel.y, 0).length();
	    	const double angle = object.angle + PI;
	    	DrawCylinder<Appearance::WHITE>(pos, object.position, 1);
	    	DrawCylinder<Appearance::BLUE>(pos, pos+object.distance*Vec(
	    		vel.x*cos(angle) - vel.y*sin(angle),
				vel.x*sin(angle) + vel.y*cos(angle),
				0
	    	) / vel2DLength + Vec(0, 0, 5), 1);
	    	if (!--max_process) {
	          break;
	        }
		}
    }
};

#endif // RECURRENT_EYE_H
