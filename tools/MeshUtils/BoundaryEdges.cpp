#include "BoundaryEdges.h"
#include <algorithm>
#include <vector>

#include <Mesh.h>
#include <Misc/Triplet.h>

#include "TripletMap.h"


BoundaryEdges::BoundaryEdges(const Mesh& mesh) {
    extract_boundary(mesh);
}

size_t BoundaryEdges::get_num_boundaries() const {
    return m_boundaries.rows();
}

MatrixIr BoundaryEdges::get_boundaries() const {
    return m_boundaries;
}

VectorI BoundaryEdges::get_boundary(size_t bi) const {
    return m_boundaries.row(bi);
}

void BoundaryEdges::extract_boundary(const Mesh& mesh) {
    typedef Triplet Edge;
    typedef TripletMap<size_t> EdgeFaceMap;
    EdgeFaceMap edge_face_map;

    const size_t num_vertex_per_face = mesh.get_vertex_per_face();
    const size_t num_vertices = mesh.get_num_vertices();
    const size_t num_faces = mesh.get_num_faces();

    for (size_t i=0; i<num_faces; i++) {
        const VectorI face = mesh.get_face(i);
        for (size_t j=0; j<num_vertex_per_face; j++) {
            size_t v0 = face[j];
            size_t v1 = face[(j+1) % num_vertex_per_face];
            Edge e(v0, v1);
            edge_face_map.insert(e, i);
        }
    }

    std::vector<size_t> boundaries;
    for (EdgeFaceMap::const_iterator itr=edge_face_map.begin();
            itr != edge_face_map.end(); itr++) {
        if (itr->second.size() == 1) {
            boundaries.push_back(itr->first.get_ori_data()[0]);
            boundaries.push_back(itr->first.get_ori_data()[1]);
        }
    }

    m_boundaries.resize(boundaries.size() / 2, 2);
    std::copy(boundaries.begin(), boundaries.end(), m_boundaries.data());
}
