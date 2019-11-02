#ifndef RECURRENT_EYE_H
#define RECURRENT_EYE_H

#include "Shapes.h"
#include <Math.h>
#include <optional>
#include "Vec.h"
#include "Matrix.h"
#include "Bisection.h"
#include "Terrain.h"
#include "Scenario.h"
#include "Chunks.h"


// class RecurrentRay {
//  public:
// 	RecurrentRay(Vec u0_t, Vec u1_t): v0(), v1(), u0(u0_t), u1(u1_t), u((u1_t - u0_t).getUnit()), seeing(std::nullopt), k(1) {}
// 	~RecurrentRay() {}

// 	void draw() const {
// 		Vec s = v0 + (v1 - v0)*fabs(k);
// 		DrawCylinder<Appearance::WHITE>(v0, v1, 0.5);
// 	}
// 	Vec v0;
// 	Vec v1;

// 	const Vec u0;
// 	const Vec u1;
// 	const Vec u;
// };


struct Viewable {
	double distance;
	double angle;
	Vec position;
	Viewable(double d, double a, Vec p) : distance(d), angle(a), position(p) {}
};

class RecurrentEye {
 public:
	const double sight;
	std::vector<Viewable> seeing;
    RecurrentEye(const double sight_distance): sight(sight_distance), seeing({}) {}

	void look(const Vec pos, const Vec vel, const std::vector<Vec> viewable_objects) {
    	seeing.clear();
    	const double speed = vel.length();
		for (const auto& object: viewable_objects) {
		    const Vec dr = Vec(object.x, object.y, 0) - Vec(pos.x, pos.y, 0);
		    const double distance = dr.length();
		    if (distance <= this->sight) {
		    	const Vec normal = Vec(0, 0, 1); // on flat surface!
		    	if (distance < 1e-8 || speed < 1e-8) {
		    		seeing.push_back(
			    		Viewable(0, 0, Vec(0, 0, 0))
			    	);
		    	} else {
		    		const double unsigned_angle = acos(dr.dot(vel) / (distance * speed)) - PI;
			    	const double angle = unsigned_angle * sgn(normal.dot(vel.cross(dr)));
			    	seeing.push_back(
			    		Viewable(distance, angle, object)
			    	);
			    }
		    }
		}
	}

    void draw(const Vec& pos, const Vec& vel, const std::vector<Vec> viewable_objects) const {
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
		}
    }
};

#endif // RECURRENT_EYE_H
