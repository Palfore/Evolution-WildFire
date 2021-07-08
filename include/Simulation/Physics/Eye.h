#ifndef EYE_H
#define EYE_H

#include "Shapes.h"
#include <Math.h>
#include <optional>
#include "Vec.h"
#include "Matrix.h"
#include "Bisection.h"
#include "Terrain.h"
#include "Scenario.h"
#include "Chunks.h"
class Ray {
 public:
	Ray(Vec u0_t, Vec u1_t): v0(), v1(), u0(u0_t), u1(u1_t), u((u1_t - u0_t).getUnit()), seeing(std::nullopt), k(1) {}
	~Ray() {}

	void draw() const {
		Vec s = v0 + (v1 - v0)*fabs(k);
		DrawCylinder<Appearance::WHITE>(v0, v1, 0.5);
	}
	Vec v0;
	Vec v1;

	const Vec u0;
	const Vec u1;
	const Vec u;
	std::optional<Vec> seeing;
	double k; // has to be calculated outside, maybe update should do it based on the Scenario.
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
	/* Determines if the target is within a square base pyramid with u0 as the apex. */
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
				const double angle = (2.0*i/static_cast<double>(numH) - 1.0) * PI - PI/2.0;
    			rays.insert({{i, j}, Ray(Vec(0, 0, 0), Vec(sin(angle)*cos(O), cos(angle)*cos(O), sin(O)))});
    		}
    	}
    }
    virtual ~Eye() {}

	void look(const Vec pos, const Vec vel, const std::vector<Vec> viewable_objects) {
    	const double angle = atan2(vel.y, vel.x);

		/* Rotate original vectors to point towards velocity */
		for (auto& [key, ray]: this->rays) {
			const auto& u1 = ray.u1;
			ray.v0 = ray.u0 + pos;
			ray.v1 = pos + 400*Vec(
				u1.x*cos(angle) - u1.y*sin(angle),
				u1.x*sin(angle) + u1.y*cos(angle),
				u1.z
			);
    	}

    	/* Determine the k's that each ray sees */
    	for (auto& [key, ray]: this->rays) {
			// This creates a square based pyramid for each set of four rays.
			// With the creature's position as the apex.
			const auto [i, j] = key;
			const int right  = i;
			const int left   = (i + 1 >= numHori) ? 0 : i + 1;
			const int bottom = j;
			const int top    = (j + 1 >= numVert) ? 0 : j + 1;

			const Vec tl = this->rays.at({left, top}).v1;
		    const Vec br = this->rays.at({right, bottom}).v1;
		    const Vec tr = this->rays.at({right, top}).v1;
		    const Vec bl = this->rays.at({left, bottom  }).v1;

		    double k = 1;
		    ray.seeing = std::nullopt;
			for (const auto& food: viewable_objects) {
			    if (isInside(pos, tl, br, tr, bl, food)) {
			    	const double foodk = (food - pos).length() / (bl - pos).length();
			    	if (foodk < k) {
				    	ray.seeing = food;
				    }
			    	k = std::min(k, foodk);
			    }
			}
			ray.k = k;
		}
	}

    void draw(const Vec& pos, const std::vector<Vec> viewable_objects) const {
	    for (const auto& [key, ray]: this->rays) {
	    	const auto [i, j] = key;
			const int right  = i;
			const int left   = (i + 1 >= numHori) ? 0 : i + 1;
			const int bottom = j;
			const int top    = (j + 1 >= numVert) ? 0 : j + 1;

			const Vec tl = this->rays.at({left, top}).v1;
		    const Vec br = this->rays.at({right, bottom}).v1;
		    const Vec tr = this->rays.at({right, top}).v1;
		    const Vec bl = this->rays.at({left, bottom  }).v1;

		    // double color = i % 2;
		    double color = static_cast<double>(i) / static_cast<double>(numHori);
		    Drawing::changeCustomColor(Vec(color, color, color));
		    DrawCylinder<Appearance::WHITE>(pos, tl, .2);
		    DrawCylinder<Appearance::WHITE>(pos, br, .2);
		    DrawCylinder<Appearance::WHITE>(pos, tr, .2);
		    DrawCylinder<Appearance::WHITE>(pos, bl, .2);

		    DrawCylinder<Appearance::CUSTOM>(tl, tr, 1);
		    DrawCylinder<Appearance::CUSTOM>(tr, br, 1);
		    DrawCylinder<Appearance::CUSTOM>(br, bl, 1);
		    DrawCylinder<Appearance::CUSTOM>(bl, tl, 1);
		    // DrawRing<Appearance::WHITE>(pos, 400, 2);

		    if (ray.seeing) {
		    	DrawCylinder<Appearance::CUSTOM>(ray.v0, *ray.seeing, 4*(1 - ray.k));
		    }
			for (const auto& food: viewable_objects) {
			    if (isInside(pos, tl, br, tr, bl, food)) {
			    	DrawCylinder<Appearance::CUSTOM>(pos, food, 0.5);
			    }
			}
		    // if (fabs(ray.k) <= 0.9) {
		    // ray.draw();
		    // if (ray.seeing) {
		    // 	DrawCylinder<Appearance::RED>(ray.v0, *ray.seeing, 2.5*(1-fabs(ray.k)));
		    // }
    		// }
		}
    }
 private:
 	const int numVert;
 	const int numHori;
};

#endif // EYE_H
