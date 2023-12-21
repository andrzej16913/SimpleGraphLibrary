#include <iostream>

#include "sgl/sgl.hpp"

int main(int argc, char *argv[]) {
    using VV = sgl::VectorVertex<char, sgl::VisitIntIDFlag>;
    using DV = sgl::DirectedVectorVertex<char, sgl::VisitIntIDFlag>;
    using RAG = sgl::RandomAccessGraph<VV>;

    static_assert(sgl::VertexVisit<VV>);
    static_assert(sgl::VertexID<VV>);
    static_assert(sgl::VertexDirected<DV>);
    static_assert(sgl::GraphRandomlyAccessible<RAG>);

    return 0;
}
