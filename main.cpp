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
    using ES = sgl::Edge<size_t, WI>;
    using VEV = sgl::VectorEdgeVertex<char, sgl::APEFlag<char, ES>, ES>;
    using RAEG = sgl::RandomAccessEdgeGraph<VEV, ES, false>;
    using DRAEG = sgl::RandomAccessEdgeGraph<VEV, ES, true>;

    using FV = sgl::ValueFlow<int, int>;
    using FE = sgl::Edge<int, FV>;

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

    static_assert(sgl::EdgeFlow<FE>);

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

    std::cout << "Test of breadthFirstSearch() with VectorVertex and RandomAccessGraph" << std::endl;

    correctCount = 0;
    std::vector<char> bfsLettersResult = {'a', 'b', 'c', 'e', 'd', 'f',  'g', 'h'};
    rag.reset();
    lri = bfsLettersResult.begin();

    sgl::breadthFirstSearch(rag, [&lri, &correctCount](VV& v) {
        if (*lri == v.data) ++correctCount; ++lri;
    }, [](VV& v) {});

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

    correctCount = 0;
    std::cout << "Test of articulationPoints()" << std::endl;
    std::vector<std::pair<size_t, size_t>> apEdges = {
            {0, 1}, {0, 4}, {1, 2}, {1, 5}, {2, 3}, {2, 6},
            {2, 7}, {3, 7}, {5, 6}
    };
    std::vector<APV*> apResult = {};
    std::vector<size_t> apCorrect = {2, 1, 0};

    APG apg = {};
    apg.addVertices(letters.begin(), letters.end());
    apg.addEdges(apEdges.begin(), apEdges.end());

    sgl::graphArticulationPoints(apg, apResult);

    for (size_t i = 0; i < apResult.size(); ++i) {
        if (apResult[i]->flags.id == apCorrect[i]) {
            correctCount++;
        }
    }

    printResult(correctCount == 3);

    std::cout << "Test of dijkstra()" << std::endl;
    correctCount = 0;

    std::vector<char> dijChars = {'s', 't', 'x', 'y', 'z'};
    std::vector<std::tuple<int, int, WI>> dijTups = {
            {0, 1, {10}}, {0, 3, {5}}, {1, 2, {1}}, {1, 3, {2}}, {2, 4, {4}},
            {3, 1, {3}}, {3, 2, {9}}, {3, 4, {2}}, {4, 0, {7}}, {4, 2, {6}}
    };
    std::vector<int> dijCorrect = {0, 8, 9, 5, 7};
    size_t i = 0;

    DRAEG draeg = {};
    draeg.addVertices(dijChars.begin(), dijChars.end());
    draeg.addEdges(dijTups.begin(), dijTups.end());

    sgl::dijkstra(draeg, *draeg.vertexBegin());

    for (auto it = draeg.vertexBegin(); it != draeg.vertexEnd(); ++it) {
        if (it->flags.dist == dijCorrect[i]) {
            correctCount++;
        }
        ++i;
    }

    printResult(correctCount == 5);

    std::cout << "Test of prim()" << std::endl;
    correctCount = 0;
    std::vector<char> primChars = {'a', 'b', 'c', 'd', 'e', 'f',  'g', 'h', 'i'};
    std::vector<std::tuple<int, int, WI>> primTups = {
            {0, 1, 4}, {0, 7,  8}, {1, 2,  8}, {1, 7, 11}, {2, 3, 7}, {2, 5, 4}, {2, 8, 2},
            {3, 4, 9}, {3, 5, 14}, {4, 5, 10}, {5, 6,  2}, {6, 7, 1}, {6, 8, 6}, {7, 8, 7}
    };
    std::vector<ES*> primResult = {};
    std::vector<std::pair<int, int>> primCorrect = {
            {0, 1}, {0, 7}, {6, 7}, {5, 6}, {2, 5}, {2, 8}, {2, 3}, {3, 4}
    };
    auto primIt = primCorrect.begin();

    RAEG raeg = {};
    raeg.addVertices(primChars.begin(), primChars.end());
    raeg.addEdges(primTups.begin(), primTups.end());

    sgl::prim(raeg, primResult);

    for (auto e: primResult) {
        if (e->from() == std::get<0>(*primIt) && e->to() == std::get<1>(*primIt)) {
            correctCount++;
        }
        ++primIt;
    }

    printResult(correctCount == 8);

    return 0;
}
