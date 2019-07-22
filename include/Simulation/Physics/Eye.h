#ifndef EYE_H
#define EYE_H

#include "Shapes.h"
#include <Math.h>
#include <Vec.h>
#include "Bisection.h"
#include "Terrain.h"
class Ray {
 public:
	Ray(): u0(), u1(), u(), k(1) {}

	void update(const Vec& u0_t, const Vec& u1_t, const Terrain& terrain) {
		u0 = u0_t;
		u1 = u1_t;
		u = (u1 - u0).getUnit();
		k = intersection(terrain);
	}

	double intersection(const Terrain& terrain) const {
		return findRoot(
			[this, terrain](double r) {
				const Vec p = u0 + r*(u1 - u0);
				return p.z - terrain.getHeight(p.x, p.y);
			}, 0, 1, 1e-2
		);
	}

	void draw() const {

	}
	Vec u0;
	Vec u1;
	Vec u;
	double k;
};

class Eye {

 public:
	const Terrain terrain;
 	std::vector<Ray> rays;

    Eye(): terrain(), rays({}) {}
    virtual ~Eye() {}


	void look(Vec pos, Vec vel) {
		rays.clear();
		const int numVert = 1;
		const int numHori = 5;
		for (int j = -1; j < numVert - 1; j++) {
    		double O = -(j*PI/numVert) / 3;

			for (int i = 0; i < numHori; i++) {
				double angle = (1.0/3.5)*(2.0*i*PI/numHori - PI);
	    		double px = vel.x * cos(angle)*cos(O) + vel.y * sin(angle)*cos(O);
	    		double py = vel.x * sin(angle)*cos(O) + vel.y * cos(angle)*cos(O);
	    		double pz = sin(O);
	    		Vec p = Vec(px, py, pz).getUnit();


	    		Vec u0 = pos;
			    Vec u1 = pos-100*p;
			    auto ray = Ray();
			    ray.update(u0, u1, terrain);
			    rays.push_back(ray);

			}
		}
	}

    void draw(Vec pos, Vec vel) const {
	    for (const auto& ray: this->rays) {
		    if (ray.k <= 0.99) {
			    Vec s = ray.u0 + (ray.u1 - ray.u0)*ray.k;
			    DrawSphere<Appearance::WHITE>(s, 1);
	    		DrawCylinder<Appearance::BLUE>(pos, s, 0.1);
    		}
		}
    }
};

#endif // EYE_H
