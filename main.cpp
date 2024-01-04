#include <iostream>

#include "sgl/sgl.hpp"

int main(int argc, char *argv[]) {
    using VV = sgl::VectorVertex<char, sgl::VisitIntIDFlag>;
    //using DV = sgl::DirectedVectorVertex<char, sgl::VisitIntIDFlag>;
    using RAG = sgl::RandomAccessGraph<VV, false>;
    using DAG = sgl::RandomAccessGraph<VV, true>;
    using APV = sgl::VectorVertex<char, sgl::APFlag<char>>;
    using APG = sgl::RandomAccessGraph<APV, false>;

    using WI = sgl::WeightValue<int>;
    using EI = sgl::Edge<int, WI>;
    using VEV = sgl::VectorEdgeVertex<char, sgl::VisitIntIDFlag, EI>;
    using RAEG = sgl::RandomAccessEdgeGraph<VEV, EI, false>;

    static_assert(sgl::VertexVisit<VV>);
    static_assert(sgl::VertexID<VV>);
    //static_assert(sgl::VertexDirected<DV>);
    static_assert(sgl::GraphRandomlyAccessible<RAG>);
    static_assert(sgl::VertexAP<APV>);
    static_assert(sgl::GraphAP<APG>);

    static_assert(sgl::GraphDirected<DAG>);
    static_assert(sgl::GraphUndirected<RAG>);
    static_assert(!sgl::GraphDirected<RAEG>);

    static_assert(sgl::IsEdge<EI>);
    static_assert(sgl::VertexEdge<VEV>);
    static_assert(sgl::GraphEdge<RAEG>);

    return 0;
}
