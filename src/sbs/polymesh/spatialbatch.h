/* Spatial partitioning of render triangles. Kept independent of OGRE resources
 * so the clipping can be verified without a window or a GPU.
 * Copyright (C)2026 Ryan Thoryk. GPL version 2 or later. */
#ifndef _SBS_SPATIALBATCH_H
#define _SBS_SPATIALBATCH_H

#include <array>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <vector>

namespace SBS {
namespace SpatialBatch {

// Position, normal, UV, in the same layout as DynamicMesh's vertex buffer.
using Vertex = std::array<float, 8>;
using Cell = std::array<long long, 3>;
using Batches = std::map<Cell, std::map<std::string, std::vector<Vertex>>>;

// Shared across every triangle of a mesh. Reaching a limit tells the caller to
// discard the partial result and render the original mesh instead.
struct Budget
{
	size_t splits = 65536;
	size_t vertices = 1048576;
	size_t cells = 4096;
};

inline Vertex Interpolate(const Vertex &a, const Vertex &b, float t)
{
	Vertex result;
	for (size_t i = 0; i < result.size(); i++)
		result[i] = a[i] + (b[i] - a[i]) * t;
	return result;
}

inline bool Partition(const std::vector<Vertex> &polygon, const std::string &material, float size, Batches &batches, Budget &budget)
{
	if (polygon.size() < 3)
		return true;
	if (!std::isfinite(size) || size <= 0)
		return false;

	// Bisect at a grid plane, choosing the longest span to keep recursion shallow.
	int axis = -1;
	float plane = 0;
	long long span = 0;
	for (int i = 0; i < 3; i++)
	{
		float low = polygon[0][i], high = low;
		for (const auto &v : polygon)
		{
			// Reject invalid/unrepresentable grid coordinates before converting
			// to integers. Double division also avoids float overflow here.
			double coordinate = double(v[i]) / size;
			if (!std::isfinite(coordinate) || std::abs(coordinate) > 1e15)
				return false;
			low = std::min(low, v[i]);
			high = std::max(high, v[i]);
		}
		long long first = static_cast<long long>(std::floor(double(low) / size));
		// A vertex exactly on the upper plane belongs to the cell below it.
		long long last = static_cast<long long>(std::ceil(double(high) / size)) - 1;
		if (last - first > span)
		{
			float split = static_cast<float>(first + (last - first + 1) / 2) * size;
			if (split > low && split < high)
			{
				axis = i;
				plane = split;
				span = last - first;
			}
		}
	}

	if (axis != -1)
	{
		if (budget.splits == 0)
			return false;
		budget.splits--;
		std::vector<Vertex> lower, upper;
		Vertex a = polygon.back();
		for (const auto &b : polygon)
		{
			if ((a[axis] < plane && b[axis] > plane) || (a[axis] > plane && b[axis] < plane))
			{
				Vertex intersection = Interpolate(a, b, (plane - a[axis]) / (b[axis] - a[axis]));
				intersection[axis] = plane;
				lower.push_back(intersection);
				upper.push_back(intersection);
			}
			if (b[axis] <= plane)
				lower.push_back(b);
			if (b[axis] >= plane)
				upper.push_back(b);
			a = b;
		}
		return Partition(lower, material, size, batches, budget) &&
			Partition(upper, material, size, batches, budget);
	}

	Cell cell;
	for (int i = 0; i < 3; i++)
	{
		double center = 0;
		for (const auto &v : polygon)
			center += v[i];
		cell[i] = static_cast<long long>(std::floor(center / polygon.size() / size));
	}
	size_t vertex_count = (polygon.size() - 2) * 3;
	if (vertex_count > budget.vertices || (batches.find(cell) == batches.end() && batches.size() >= budget.cells))
		return false;
	budget.vertices -= vertex_count;
	auto &vertices = batches[cell][material];
	for (size_t i = 1; i + 1 < polygon.size(); i++)
	{
		vertices.push_back(polygon[0]);
		vertices.push_back(polygon[i]);
		vertices.push_back(polygon[i + 1]);
	}
	return true;
}

inline bool Partition(const std::vector<Vertex> &polygon, const std::string &material, float size, Batches &batches)
{
	Budget budget;
	return Partition(polygon, material, size, batches, budget);
}

}
}
#endif
