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

	template <typename Vertex>
	concept IsVertex =
    std::forward_iterator<typename Vertex::iterator> &&
    //std::same_as<Vertex, typename Vertex::iterator*> &&
    requires(Vertex v, Vertex& to) {
        typename Vertex::FlagType;
        Vertex::DIRECTED;
		{ v.vertexBegin() } -> std::same_as<typename Vertex::iterator>;
		{ v.vertexEnd() } -> std::sentinel_for<typename Vertex::iterator>;
        v.addEdge(to);
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
    concept VertexDirected =
    IsVertex<Vertex> &&
    requires(Vertex v) {
        Vertex::DIRECTED == true;
    };
	
	template <typename Graph>
	concept IsGraph =
    std::forward_iterator<typename Graph::iterator> &&
    //std::same_as<typename Graph::VertexType, typename Graph::iterator*> &&
    requires(Graph g, typename Graph::VertexType& v, typename Graph::iterator i, typename Graph::iterator e) {
        typename Graph::VertexType;
		{ g.vertexBegin() } -> std::same_as<typename Graph::iterator>;
        { g.vertexEnd() } -> std::sentinel_for<typename Graph::iterator>;
		g.addVertex(v);
        g.addVertices(i, e);
        g.reset();
	};

    template <typename Graph>
    concept GraphVisit =
    IsGraph<Graph> &&
    VertexVisit<typename Graph::VertexType>;

    template <typename Graph>
    concept GraphID =
    IsGraph<Graph> &&
    VertexID<typename Graph::VertexType>;
	
	template <typename Graph>
	concept GraphRandomlyAccessible =
    IsGraph<Graph> &&
	requires(Graph g, size_t pos) {
		g.operator[](pos);
	};
}

#endif
