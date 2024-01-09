#ifndef SGL_CONCEPTS_HPP
#define SGL_CONCEPTS_HPP

#include <concepts>
#include <iterator>

namespace sgl {
    template <typename Flag>
    concept IsFlag =
    requires(Flag f) {
        f.reset();
    };

    template <typename Flag>
    concept FlagVisit =
    IsFlag<Flag> &&
    requires(Flag f) {
        f.visit();
        f.visited();
    };

    template <typename Flag>
    concept FlagID =
    IsFlag<Flag> &&
    requires(Flag f) {
        typename Flag::IDType;
        f.id;
    };

    template <typename Flag>
    concept FlagDist =
    IsFlag<Flag> &&
    requires(Flag f) {
        typename Flag::DType;
        f.dist;
        { Flag::zeroDist() } -> std::same_as<typename Flag::DType>;
        { Flag::maxDist() } -> std::same_as<typename Flag::DType>;
        f.dist + 1;
    };

    template <typename Flag>
    concept FlagLow =
    IsFlag<Flag> &&
    requires(Flag f) {
        typename Flag::LType;
        f.low;
    };

    template <typename Flag>
    concept FlagPrev =
    IsFlag<Flag> &&
    requires(Flag f) {
        f.prev;
    };

    template <typename Flag>
    concept FlagAP =
    FlagVisit<Flag> &&
    FlagDist<Flag> &&
    FlagLow<Flag> &&
    FlagPrev<Flag>;

    template <typename Edge>
    concept IsEdge =
    requires(Edge e) {
        typename Edge::IDType;
        typename Edge::ValueType;
        { e.from() } -> std::same_as<typename Edge::IDType>;
        { e.to() } -> std::same_as<typename Edge::IDType>;
        e.value;
    };

    template <typename Edge>
    concept EdgeWeight =
    IsEdge<Edge> &&
    requires(Edge e) {
        typename Edge::ValueType::WeightType;
        { e.value.weight() } -> std::same_as<typename Edge::VelueType::WeightType>;
        { Edge::ValueType::zeroWeight() } -> std::same_as<typename Edge::VelueType::WeightType>;
        { Edge::ValueType::maxWeight() } -> std::same_as<typename Edge::VelueType::WeightType>;
    };

	template <typename Vertex>
	concept IsVertex =
    std::forward_iterator<typename Vertex::VertexIterator> &&
    std::same_as<Vertex, typename Vertex::VertexIterator::value_type> &&
    requires(Vertex v, Vertex& to) {
        typename Vertex::FlagType;
        //Vertex::DIRECTED;
		{ v.vertexBegin() } -> std::same_as<typename Vertex::VertexIterator>;
		{ v.vertexEnd() } -> std::sentinel_for<typename Vertex::VertexIterator>;
        //v.addEdge(to);
        v.reset();
        v.flags;
        v.data;
	};
	
	template <typename Vertex>
	concept VertexVisit =
    IsVertex<Vertex> &&
    FlagVisit<typename Vertex::FlagType>;
	
	template <typename Vertex>
	concept VertexID =
    IsVertex<Vertex> &&
    FlagID<typename Vertex::FlagType>;

    template <typename Vertex>
    concept VertexAP =
    IsVertex<Vertex> &&
    FlagAP<typename Vertex::FlagType>;

    template <typename Vertex>
    concept VertexDist =
    IsVertex<Vertex> &&
    FlagDist<typename Vertex::FlagType>;

    template <typename Vertex>
    concept VertexEdge =
    IsVertex<Vertex> &&
    IsEdge<typename Vertex::EdgeType> &&
    std::forward_iterator<typename Vertex::EdgeIterator> &&
    std::forward_iterator<typename Vertex::PairIterator> &&
    std::same_as<typename Vertex::EdgeType, typename Vertex::EdgeIterator::value_type> &&
    std::same_as<typename Vertex::PairType, std::pair<Vertex*, typename Vertex::EdgeType*> > &&
    requires(Vertex v) {
        { v.edgeBegin() } -> std::same_as<typename Vertex::EdgeIterator>;
        { v.edgeEnd() } -> std::sentinel_for<typename Vertex::EdgeIterator>;
        { v.pairBegin() } -> std::same_as<typename Vertex::PairIterator>;
        { v.pairEnd() } -> std::sentinel_for<typename Vertex::PairIterator>;
    };
	
	template <typename Graph>
	concept IsGraph =
    std::forward_iterator<typename Graph::VertexIterator> &&
    std::same_as<typename Graph::VertexType, typename Graph::VertexIterator::value_type> &&
    requires(Graph g, typename Graph::VertexType& v, typename Graph::VertexIterator i, typename Graph::VertexIterator e) {
        typename Graph::VertexType;
		{ g.vertexBegin() } -> std::same_as<typename Graph::VertexIterator>;
        { g.vertexEnd() } -> std::sentinel_for<typename Graph::VertexIterator>;
		g.addVertex(v);
        g.addVertices(i, e);
        g.reset();
        { g.vertexCount() } -> std::convertible_to<size_t>;
	};

    template <typename Graph>
    concept GraphVisit =
    IsGraph<Graph> &&
    VertexVisit<typename Graph::VertexType>;

    template <typename Graph>
    concept GraphID =
    IsGraph<Graph> &&
    VertexID<typename Graph::VertexType> &&
    requires () {
        typename Graph::IDType;
    };

    template <typename Graph>
    concept GraphAP =
    IsGraph<Graph> &&
    VertexAP<typename Graph::VertexType>;

    template <typename Graph>
    concept GraphDist =
    IsGraph<Graph> &&
    VertexDist<typename Graph::VertexType>;
	
	template <typename Graph>
	concept GraphRandomlyAccessible =
    IsGraph<Graph> &&
	requires(Graph g, size_t pos) {
		g.operator[](pos);
	};

    template <typename Graph>
    concept GraphDirected =
    IsGraph<Graph> &&
    Graph::DIRECTED == true;

    template <typename Graph>
    concept GraphUndirected =
    IsGraph<Graph> &&
    Graph::DIRECTED == false;

    template <typename Graph>
    concept GraphEdge =
    IsGraph<Graph> &&
    VertexEdge<typename Graph::VertexType> &&
    std::forward_iterator<typename Graph::EdgeIterator> &&
    std::same_as<typename Graph::EdgeType, typename Graph::EdgeIterator::value_type> &&
    std::same_as<typename Graph::VertexType::EdgeType, typename Graph::EdgeType> &&
    requires(Graph g) {
        typename Graph::EdgeType;
        { g.edgeBegin() } -> std::same_as<typename Graph::EdgeIterator>;
        { g.edgeEnd() } -> std::sentinel_for<typename Graph::EdgeIterator>;
        { g.edgeCount() } -> std::convertible_to<size_t>;
    };

    template <typename Container>
    concept HasPushBack =
    requires(Container c, typename Container::value_type v) {
        c.push_back(v);
    };
}

#endif
