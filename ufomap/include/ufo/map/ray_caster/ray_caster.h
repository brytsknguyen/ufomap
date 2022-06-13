/*!
 * UFOMap: An Efficient Probabilistic 3D Mapping Framework That Embraces the Unknown
 *
 * @author Daniel Duberg (dduberg@kth.se)
 * @see https://github.com/UnknownFreeOccupied/ufomap
 * @version 1.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Daniel Duberg, KTH Royal Institute of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UFO_MAP_RAY_CASTER_H
#define UFO_MAP_RAY_CASTER_H

// UFO
#include <ufo/map/code/code.h>
#include <ufo/map/key.h>
#include <ufo/map/types.h>
#include <ufo/math/util.h>

// STL
#include <exception>
#include <numeric>

namespace ufo::map
{
KeyRay computeRay(Key origin, Key goal)
{
	if (origin.depth() != goal.depth()) {
		throw std::invalid_argument("origin and goal need to be at the same depth.");
	}

	using namespace ufo::math;

	int size = 1U << origin.depth();

	Vector3d o(origin.x(), origin.y(), origin.z());
	Vector3d g(goal.x(), goal.y(), goal.z());

	Vector3d dir = g - o;
	double distance = dir.norm();
	dir /= distance;

	Vector3i step(sgn(dir.x) * size, sgn(dir.y) * size, sgn(dir.z) * size);

	dir.abs();

	static constexpr auto max = std::numeric_limits<double>::max();
	Vector3d t_delta(step.x ? size / dir.x : max, step.y ? size / dir.y : max,
	                 step.z ? size / dir.z : max);

	Vector3d t_max = t_delta / 2;

	KeyRay ray;
	ray.reserve(Vector3d::abs(g - o).norm());
	ray.push_back(origin);
	while (origin != goal && t_max.min() <= distance) {
		auto advance_dim = t_max.minElementIndex();
		origin[advance_dim] += step[advance_dim];
		t_max[advance_dim] += t_delta[advance_dim];
		ray.push_back(origin);
	}
	return ray;
}

std::vector<Point3> computeRaySimple(Point3 origin, Point3 goal, double step_size)
{
	Point3 dir = goal - origin;
	double distance = dir.norm();
	dir /= distance;

	size_t num_steps = distance / step_size;
	Point3 step = dir * step_size;

	std::vector<Point3> ray{origin};
	ray.reserve(num_steps);
	for (size_t i = 0; i != num_steps; ++i) {
		origin += step;
		ray.push_back(origin);
	}
	return ray;
}
}  // namespace ufo::map

#endif  // UFO_MAP_RAY_CASTER_H