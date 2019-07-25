#ifndef EYE_H
#define EYE_H

#include "Shapes.h"
#include <Math.h>
#include "Vec.h"
#include "Matrix.h"
#include "Bisection.h"
#include "Terrain.h"
#include "Senario.h"
class Ray {
 public:
	Ray(Vec u0_t, Vec u1_t): u0(u0_t), u1(u1_t), v0(), v1(), u((u1_t - u0_t).getUnit()), k(1) {}
	~Ray() {}

	void update(const Vec& v0_t, const Vec& v1_t, const Senario& senario) {
		v0 = v0_t;
		v1 = v1_t;
		k = intersection(senario);
	}

	double intersection(const Senario& senario) const {
		// double terrainRoot = findRoot(
		// 	[this, &senario](double r) {
		// 		const Vec p = u0 + r*(u1 - u0);
		// 		return p.z - senario.terrain.getHeight(p.x, p.y);
		// 	}, 0, 1, 0.1
		// );

		// double closestFood = 1;
		// for (const auto& food: senario.food) {
		// 	if ((food - u0).length() < (u1 - u0).length()) {
		// 		closestFood = std::min(closestFood, findRoot(
		// 			[this, &food](double r) {
		// 				const Vec p = u0 + r*(u1 - u0);
		// 				return (p - food).length() - 10;
		// 			}, 0, 1, 0.1
		// 		));
		// 	}
		// }
		// return closestFood;
		// if (terrainRoot > closestFood) {
		// 	return closestFood;
		// } else {
		// 	return -terrainRoot;
		// }
		return 1;
	}

	void draw() const {
		Vec s = v0 + (v1 - v0)*fabs(k);
		if (fabs(k) <= 0.9)	{
			DrawCylinder<Appearance::RED>(v0, s, 1);
		} else {
			DrawCylinder<Appearance::WHITE>(v0, s, 0.1);
		}
		// if (k < 0) {
		// 	if (fabs(k) <= 0.9) DrawSphere<Appearance::WHITE>(s, 5);
		//     DrawCylinder<Appearance::RED>(u0, s, 0.2);
		// } else {
		// 	if (fabs(k) <= 0.9) DrawSphere<Appearance::WHITE>(s, 5);
		//     DrawCylinder<Appearance::BLUE>(u0, s, 0.2);
		// }
	}
	Vec v0;
	Vec v1;

	const Vec u0;
	const Vec u1;
	const Vec u;
	double k;
};


static double getD(Vec a, Vec b, Vec c, Vec x) {
	Vec B = b - c;
	Vec C = a - c;
	Vec X = x - c;
	return Matrix(
		B.x, B.y, B.z,
		C.x, C.y, C.z,
		X.x, X.y, X.z
	).getDeterminant();
}

static bool isInside(Vec u0, Vec tl, Vec br, Vec tr, Vec bl, Vec target) {
	const int sign = sgn(getD(bl, tl, u0, target));
    if (sgn(getD(tl, tr, u0, target)) != sign) return false;
    if (sgn(getD(tr, br, u0, target)) != sign) return false;
    if (sgn(getD(br, bl, u0, target)) != sign) return false;
    if (sgn(getD(br, tr, tl, target)) != sign) return false;
    return true;
}


class Eye {
 public:
 	std::unordered_map<std::pair<int, int>, Ray, hash_pair> rays;

    Eye(int numV, int numH): rays({}), numVert(numV), numHori(numH) {
    	for (int i = 0; i < numH; i++) {
    		for (int j = 0; j < numV; j++) {
		    	const double O = -(j*PI/(double) numV) / 2;
				const double angle = (2.0*i/static_cast<double>(numH) - 1.0) * PI/2.0 - PI/2.0;
    			rays.insert({{i, j}, Ray(Vec(0, 0, 0), Vec(sin(angle)*cos(O), cos(angle)*cos(O), sin(O)))});
    		}
    	}
    }
    virtual ~Eye() {}


	void look(Vec pos, Vec vel, Senario& senario) {
    	const double angle = atan2(vel.y, vel.x);
		std::vector<Vec> foodToCheck = senario.food.getFrom(pos.x, pos.y, 1);

		/* Rotate original vectors to point towards velocity */
		for (auto& [key, ray]: this->rays) {
    			const auto& u1 = ray.u1;
    			ray.v0 = ray.u0 + pos;
    			ray.v1 = pos + 200*Vec(
    				u1.x*cos(angle) - u1.y*sin(angle),
    				u1.x*sin(angle) + u1.y*cos(angle),
    				u1.z
    			);
    	}

    	/* Determine the k's that each ray sees */
    	for (auto& [key, ray]: this->rays) {
			const auto [i, j] = key;
			if (i + 1 >= numHori) continue;
			if (j + 1 >= numVert) continue;
			const Vec tl = this->rays.at({i+1, j+1}).v1;
		    const Vec br = this->rays.at({i  , j  }).v1;
		    const Vec tr = this->rays.at({i  , j+1}).v1;
		    const Vec bl = this->rays.at({i+1, j  }).v1;

		    double k = 1;
			for (const auto& food: foodToCheck) {
			    if (isInside(pos, tl, br, tr, bl, food)) {
			    	k = std::min(k, (food - pos).length() / (bl - pos).length());
			    }
			}
			ray.k = k;
		}
	}

    void draw() const {
	    for (const auto& [key, ray]: this->rays) {
		    // if (fabs(ray.k) <= 0.9) {
			    ray.draw();
    		// }
		}
    }
 private:
 	const int numVert;
 	const int numHori;
};

#endif // EYE_H
