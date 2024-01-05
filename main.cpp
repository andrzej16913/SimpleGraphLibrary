#include <iostream>

#include "sgl/sgl.hpp"

void printResult(bool result) {
    if (result) {
        std::cout << "Test completed successfully";
    } else {
        std::cout << "Test failed";
    }

    std::cout << std::endl << std::endl;
}

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

    std::cout << "Test of deepFirstSearch() with VectorVertex and RandomAccessGraph" << std::endl;

    size_t correctCount = 0;
    std::vector<char> letters =  { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    std::vector<char> letterResult = { 'a', 'b', 'd', 'f', 'e', 'c', 'g', 'h'};
    std::vector<std::pair<int, int>> letterEdges= {
            {0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 6}, {4, 5}
    };

    auto lri = letterResult.begin();
    RAG rag = {};

    rag.addVertices(std::begin(letters), std::end(letters));
    rag.addEdges(letterEdges.begin(), letterEdges.end());
    sgl::deepFirstSearch(rag, [&lri, &correctCount](VV& v) {
        if (*lri == v.data) ++correctCount; ++lri;
        }, [](VV& v){});
    printResult(correctCount == 8);

    correctCount = 0;
    std::cout << "Test of stronglyConnectedComponents()" << std::endl;
    std::vector<std::pair<int, int>> dagEdges = {
            {0, 1}, {1, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 6}, {3, 2},
            {3, 7}, {4, 0}, {4, 5}, {5, 6}, {6, 5}, {6, 7}, {7, 7}
    };
    std::vector<std::vector<int>> sccCorrect = {{0, 4, 1}, {2, 3}, {6, 5}, {7}};

    DAG dag = {};
    dag.addVertices(letters.begin(), letters.end());
    dag.addEdges(dagEdges.begin(), dagEdges.end());

    auto scc = sgl::stronglyConnectedComponents(dag);

    for (size_t i = 0; i < scc.size(); ++i) {
        for (size_t j = 0; j < scc[i].size(); ++j) {
            if (scc[i][j] == sccCorrect[i][j]) {
                correctCount++;
            }
        }
    }

    printResult(correctCount == 8);

    return 0;
}
