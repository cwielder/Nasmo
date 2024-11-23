#include <nsm/ui/uishape.h>

#include <nsm/debug/assert.h>

#include <poly2tri/poly2tri.h>

void nsm::UIShape::setVertices(const std::vector<glm::vec2>& vertices) {
    NSM_ASSERT(vertices.size() >= 3, "A shape must have at least 3 vertices.");
    
    mVertices.clear();
    for (const auto& vertex : vertices) {
        mVertices.push_back(vertex);
    }
}

const std::vector<glm::vec2>& nsm::UIShape::getVertices() const {
    return mVertices;
}

const std::vector<glm::vec2> nsm::UIShape::getVerticesTriangulated() const {
    std::vector<glm::vec2> inputVertices = this->getVertices();

	NSM_ASSERT(inputVertices.size() >= 3, "Cannot triangulate a shape with less than 3 vertices.");

	std::vector<p2t::Point> polylineData;
	for (const auto& vertex : inputVertices) {
		polylineData.push_back(p2t::Point(vertex.x, vertex.y));
	}

	std::vector<p2t::Point*> polyline;
	for (auto& point : polylineData) {
		polyline.push_back(&point);
	}

	p2t::CDT shape(polyline);
	shape.Triangulate();

	std::vector<p2t::Triangle*> triangles = shape.GetTriangles();

	std::vector<glm::vec2> triangulatedVertices;
	for (const auto& triangle : triangles) {
		for (u32 i = 0; i < 3; ++i) {
			const p2t::Point* point = triangle->GetPoint(i);
			triangulatedVertices.push_back(glm::vec2(point->x, point->y));
		}
	}

	return triangulatedVertices;
}
