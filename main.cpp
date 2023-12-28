#include <iostream>

#include "sgl/sgl.hpp"

int main(int argc, char *argv[]) {
    using VV = sgl::VectorVertex<char, sgl::VisitIntIDFlag>;
    //using DV = sgl::DirectedVectorVertex<char, sgl::VisitIntIDFlag>;
    using RAG = sgl::RandomAccessGraph<VV, false>;
    using DAG = sgl::RandomAccessGraph<VV, true>;
    using APV = sgl::VectorVertex<char, sgl::APFlag<char>>;

    static_assert(sgl::VertexVisit<VV>);
    static_assert(sgl::VertexID<VV>);
    //static_assert(sgl::VertexDirected<DV>);
    static_assert(sgl::GraphRandomlyAccessible<RAG>);
    static_assert(sgl::VertexAP<APV>);

    return 0;
}
