#ifndef SGL_CLASSES_HPP
#define SGL_CLASSES_HPP

#include <concepts>
#include <cstddef>
#include <deque>
#include <iterator>
#include <utility>
#include <vector>

#include "sgl_concepts.hpp"

namespace sgl {
    class VisitFlag {
    private:
        bool visited_ = false;
    public:
        void visit() { visited_ = true; }
        bool visited() const { return visited_; }
        void reset() { visited_ = false; }
    };

    class VisitIntIDFlag {
    private:
        bool visited_ = false;
    public:
        using IDType = int;
        IDType id;

        VisitIntIDFlag() : visited_{false}, id{0} {}
        explicit VisitIntIDFlag(const int ID) : visited_{false}, id{ID} {}

        void visit() { visited_ = true; }
        bool visited() const { return visited_; }
        void reset() { visited_ = false; id = 0; }
    };

    /*template <IsVertex Vertex>
    class VertexIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Vertex;
        using pointer           = value_type*;
        using reference         = value_type&;

        VertexIterator () : it_{} {}
        explicit VertexIterator(typename Vertex::ContainerType::iterator it) : it_(it) {}

        reference operator*() const { return *(*it_); }
        pointer operator->() { return *it_; }

        VertexIterator& operator++() { it_++; return *this; }

        VertexIterator operator++(int) { VertexIterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const VertexIterator& a, const VertexIterator& b) { return a.it_ == b.it_; };
        friend bool operator!= (const VertexIterator& a, const VertexIterator& b) { return a.it_ != b.it_; };
    private:
        typename Vertex::ContainerType::iterator it_;
    };*/

    template <typename IDT, typename Value>
    class Edge {
    private:
        IDT from_;
        IDT to_;
    public:
        Value value;
        using IDType = IDT;
        using ValueType = Value;
        Edge(IDType from, IDType to, Value values) : from_{from}, to_{to}, value{values} {}
        explicit Edge(std::tuple<IDType, IDType, ValueType> tuple) :
            from_{std::get<0>(tuple)}, to_{std::get<1>(tuple)}, value{std::get<2>(tuple)} {}

        IDType from() { return from_; }
        IDType to() { return to_; }
    };

    template <typename Data, typename Flag>
    class VectorVertex {
    private:
        std::vector<VectorVertex<Data, Flag>*> vertices_;
    public:
        Flag flags;
        Data data;
        //static constexpr bool DIRECTED = false;
        using ThisType = VectorVertex<Data, Flag>;
        using ContainerType = std::vector<ThisType*>;
        using FlagType = Flag;

        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = VectorVertex<Data, Flag>;
            using pointer           = value_type*;
            using reference         = value_type&;

            iterator () : it_{} {}
            explicit iterator(typename ContainerType::iterator it) : it_(it) {}

            reference operator*() const { return *(*it_); }
            pointer operator->() { return *it_; }

            iterator& operator++() { it_++; return *this; }

            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const iterator& a, const iterator& b) { return a.it_ == b.it_; };
            friend bool operator!= (const iterator& a, const iterator& b) { return a.it_ != b.it_; };
        private:
            typename ContainerType::iterator it_;
        };

        using VertexIterator = iterator;

        VectorVertex() : vertices_{}, flags{}, data{} {}
        explicit VectorVertex(const Data& d) : vertices_{}, flags{}, data(d) {}
        VectorVertex(const VectorVertex& v) : vertices_{}, flags(v.flags), data(v.data) {}
        VectorVertex(VectorVertex&& v)  noexcept = default;

        template <typename... Args>
        requires std::constructible_from<Data, Args...>
        explicit VectorVertex(Args&&... args) : vertices_{},flags{}, data{args...} {}

        VectorVertex& operator=(const VectorVertex& v) {
            if (this == &v)
                return *this;

            vertices_ = {};
            flags = v.flags;
            data = v.data;

            return *this;
        }

        VectorVertex& operator=(VectorVertex&& v)  noexcept = default;

        void addEdge(ThisType& to) {
            vertices_.push_back(&to);
        }

        void reset() {
            flags.reset();
        }

        iterator vertexBegin() { return iterator(vertices_.begin()); }
        iterator vertexEnd() { return iterator(vertices_.end()); }
    };

    template <typename Data, typename Flag, typename Edge>
    class VectorEdgeVertex {
    private:
        std::vector<std::pair<VectorEdgeVertex<Data, Flag, Edge>*, Edge*>> vertices_;
    public:
        Flag flags;
        Data data;
        using ThisType = VectorEdgeVertex<Data, Flag, Edge>;
        using PairType = std::pair<ThisType*, Edge*>;
        using ContainerType = std::vector<PairType>;
        using FlagType = Flag;
        using EdgeType = Edge;
        using PairIterator = typename ContainerType::iterator;

        class iterator {
        protected:
            iterator () : it_{} {}
            explicit iterator(typename ContainerType::iterator it) : it_(it) {}

        public:
            friend bool operator== (const iterator& a, const iterator& b) { return a.it_ == b.it_; };
            friend bool operator!= (const iterator& a, const iterator& b) { return a.it_ != b.it_; };
        protected:
            typename ContainerType::iterator it_;
        };

        class VertexIterator : public ThisType::iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = ThisType;
            using pointer           = value_type*;
            using reference         = value_type&;

            VertexIterator() : iterator() {}
            explicit VertexIterator(typename ContainerType::iterator it) : iterator(it) {}

            reference operator*() const { return *std::get<0>(*this->it_); }
            pointer operator->() { return std::get<0>(*this->it_); }

            VertexIterator& operator++() { this->it_++; return *this; }
            VertexIterator operator++(int) { VertexIterator tmp = *this; ++(*this); return tmp; }
        };

        class EdgeIterator : public ThisType::iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = Edge;
            using pointer           = value_type*;
            using reference         = value_type&;

            EdgeIterator() : iterator() {}
            explicit EdgeIterator(typename ContainerType::iterator it) : iterator(it) {}

            reference operator*() const { return *std::get<1>(*this->it_); }
            pointer operator->() { return std::get<1>(*this->it_); }

            EdgeIterator& operator++() { this->it_++; return *this; }
            EdgeIterator operator++(int) { EdgeIterator tmp = *this; ++(*this); return tmp; }
        };

        VectorEdgeVertex() : vertices_{}, flags{}, data{} {}
        explicit VectorEdgeVertex(const Data& d) : vertices_{}, flags{}, data(d) {}
        VectorEdgeVertex(const VectorEdgeVertex& v) : vertices_{}, flags(v.flags), data(v.data) {}
        VectorEdgeVertex(VectorEdgeVertex&& v)  noexcept = default;

        template <typename... Args>
        requires std::constructible_from<Data, Args...>
        explicit VectorEdgeVertex(Args&&... args) : vertices_{}, flags{}, data{args...} {}

        VectorEdgeVertex& operator=(const VectorEdgeVertex& v) {
            if (this == &v)
                return *this;

            vertices_ = {};
            flags = v.flags;
            data = v.data;

            return *this;
        }

        VectorEdgeVertex& operator=(VectorEdgeVertex&& v)  noexcept = default;

        void addEdge(ThisType& vertex, Edge& edge) {
            vertices_.push_back(std::pair(&vertex, &edge));
        }

        void reset() {
            flags.reset();
        }

        VertexIterator vertexBegin() { return VertexIterator(vertices_.begin()); }
        VertexIterator vertexEnd() { return VertexIterator(vertices_.end()); }

        EdgeIterator edgeBegin() { return EdgeIterator(vertices_.begin()); }
        EdgeIterator edgeEnd() { return EdgeIterator(vertices_.end()); }

        PairIterator pairBegin() { return vertices_.begin(); }
        PairIterator pairEnd() { return vertices_.end(); }
    };
/*
    template <typename Data, IsFlag Flag>
    class DirectedVectorVertex {
    private:
        std::vector<DirectedVectorVertex<Data, Flag>*> vertices_;
    public:
        Flag flags;
        Data data;
        static constexpr bool DIRECTED = true;
        using ThisType = DirectedVectorVertex<Data, Flag>;
        using ContainerType = std::vector<ThisType*>;
        using FlagType = Flag;
        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = DirectedVectorVertex<Data, Flag>;
            using pointer           = value_type*;
            using reference         = value_type&;

            iterator () : it_{} {}
            explicit iterator(typename ContainerType::iterator it) : it_(it) {}

            reference operator*() const { return *(*it_); }
            pointer operator->() { return *it_; }

            iterator& operator++() { it_++; return *this; }

            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const iterator& a, const iterator& b) { return a.it_ == b.it_; };
            friend bool operator!= (const iterator& a, const iterator& b) { return a.it_ != b.it_; };
        private:
            typename ContainerType::iterator it_;
        };

        DirectedVectorVertex() : vertices_{}, flags{}, data{} {}
        explicit DirectedVectorVertex(const Data& d) : vertices_{}, flags{}, data(d) {}
        DirectedVectorVertex(const DirectedVectorVertex& v) : vertices_{}, flags(v.flags), data(v.data) {}
        DirectedVectorVertex(DirectedVectorVertex&& v)  noexcept = default;

        template <typename... Args>
        requires std::constructible_from<Data, Args...>
        explicit DirectedVectorVertex(Args&&... args) : vertices_{},flags{}, data{args...} {}

        DirectedVectorVertex& operator=(const DirectedVectorVertex& v) {
            if (this == &v)
                return *this;

            vertices_ = {};
            flags = v.flags;
            data = v.data;

            return *this;
        }

        DirectedVectorVertex& operator=(DirectedVectorVertex&& v)  noexcept = default;

        void addEdge(ThisType& to) {
            vertices_.push_back(&to);
        }

        void reset() {
            flags.reset();
        }

        iterator vertexBegin() { return iterator(vertices_.begin()); }
        iterator vertexEnd() { return iterator(vertices_.end()); }
    };
*/
	template <VertexID Vertex, bool Directed>
	requires std::convertible_to<typename Vertex::FlagType::IDType, size_t>
	class RandomAccessGraph {
		public:
		using VertexIterator = typename std::deque<Vertex>::iterator;
        using VertexType = Vertex;
        using IDType = typename Vertex::FlagType::IDType;
        static constexpr bool DIRECTED = Directed;
		
		RandomAccessGraph() : vertices_{} {}
        RandomAccessGraph(const RandomAccessGraph& g) : vertices_{} {
            addVertices(g.vertexBegin(), g.vertexEnd());
            copyEdges(g);
        }

        RandomAccessGraph(RandomAccessGraph&& g) noexcept = default;

        RandomAccessGraph& operator=(const RandomAccessGraph& g) {
            if (this == &g)
                return *this;

            vertices_ = {};
            addVertices(g.vertexBegin(), g.vertexEnd());
            copyEdges(g);

            return *this;
        }

        RandomAccessGraph& operator=(RandomAccessGraph&& g)  noexcept = default;

        Vertex& operator[] (size_t pos) { return vertices_[pos]; }
        const Vertex& operator[] (size_t pos) const { return vertices_[pos]; }

		void addVertex(const Vertex& vertex) { vertices_.emplace_back(vertex); }

        template <typename... Args>
        requires std::constructible_from<Vertex, Args...>
        void addVertex(Args&&... args) {
            vertices_.emplace_back(args...);
            vertices_.back().flags.id = vertices_.size() - 1;
        }
		
		template <std::forward_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
        requires std::constructible_from<Vertex, typename Iterator::reference>
		void addVertices(Iterator begin, Sentinel end) {
			while (begin != end) {
				addVertex(*begin);
				++begin;
			}
		}

        void addEdge(IDType from, IDType to) {
            vertices_[from].addEdge(vertices_[to]);
            if constexpr (!DIRECTED) {
                vertices_[to].addEdge(vertices_[from]);
            }
        }

        template <std::forward_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
        requires std::same_as<std::pair<IDType, IDType>, typename Iterator::value_type>
        void addEdges(Iterator begin, Sentinel end) {
            while (begin != end) {
                addEdge(std::get<0>(*begin), std::get<1>(*begin));
                ++begin;
            }
        }

        template <GraphID Graph>
        void copyEdges(const Graph& g) {
            for (auto from = g.vertexBegin(); from != g.vertexEnd(); ++from) {
                for (auto to = from->vertexBegin(); to != from->vertexEnd(); ++to) {
                    vertices_[from->flags.id].addEdge(vertices_[to->flags.id]);
                }
            }
        }
		
		void reset() {
			for (auto vertex: vertices_) {
				vertex->reset();
			}
		}

		VertexIterator vertexBegin() { return vertices_.begin(); }
		VertexIterator vertexEnd() { return vertices_.end(); }
		
		/*void print() {
			for (size_t i = 0; i < vertices.size(); ++i) {
				auto v = vertices[i];
				std::cerr << "graph: " << v << ':' << v->getData() << std::endl;
			}
		}*/
		
		private:
		std::deque<Vertex> vertices_;
	};

    template <VertexID Vertex, IsEdge Edge, bool Directed>
    requires std::same_as<typename Vertex::EdgeType, Edge> &&
             std::same_as<typename Vertex::FlagType::IDType, typename Edge::IDType> &&
             std::convertible_to<typename Vertex::FlagType::IDType, size_t>
    class RandomAccessEdgeGraph {
    public:
        using VertexIterator = typename std::deque<Vertex>::iterator;
        using EdgeIterator = typename std::deque<Edge>::iterator;
        using VertexType = Vertex;
        using EdgeType = Edge;
        using IDType = typename Vertex::FlagType::IDType;
        using EdgeValueType = typename Edge::ValueType;
        static constexpr bool DIRECTED = Directed;

        RandomAccessEdgeGraph() : vertices_{} {}
        RandomAccessEdgeGraph(const RandomAccessEdgeGraph& g) : vertices_{} {
            addVertices(g.vertexBegin(), g.vertexEnd());
            copyEdges(g);
        }

        RandomAccessEdgeGraph(RandomAccessEdgeGraph&& g) noexcept = default;

        RandomAccessEdgeGraph& operator=(const RandomAccessEdgeGraph& g) {
            if (this == &g)
                return *this;

            vertices_ = {};
            addVertices(g.vertexBegin(), g.vertexEnd());
            copyEdges(g);

            return *this;
        }

        RandomAccessEdgeGraph& operator=(RandomAccessEdgeGraph&& g)  noexcept = default;

        Vertex& operator[] (size_t pos) { return vertices_[pos]; }
        const Vertex& operator[] (size_t pos) const { return vertices_[pos]; }

        void addVertex(const Vertex& vertex) { vertices_.emplace_back(vertex); }

        template <typename... Args>
        requires std::constructible_from<Vertex, Args...>
        void addVertex(Args&&... args) {
            vertices_.emplace_back(args...);
            vertices_.back().flags.id = vertices_.size() - 1;
        }

        template <std::forward_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
        requires std::constructible_from<Vertex, typename Iterator::reference>
        void addVertices(Iterator begin, Sentinel end) {
            while (begin != end) {
                addVertex(*begin);
                ++begin;
            }
        }

        template <typename... Args>
        requires std::constructible_from<Edge, Args...>
        void addEdge(Args&&... args) {
            edges_.emplace_back(args...);
            Edge& newest = edges_.back();

            vertices_[newest.from()].addEdge(vertices_[newest.to()], newest);
            if constexpr (!DIRECTED) {
                vertices_[newest.to()].addEdge(vertices_[newest.from()], newest);
            }
        }

        template <std::forward_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
        requires std::constructible_from<Edge, typename Iterator::reference>
        void addEdges(Iterator begin, Sentinel end) {
            while (begin != end) {
                addEdge(*begin);
                ++begin;
            }
        }

        template <GraphEdge Graph>
        void copyEdges(const Graph& g) {
            addEdges(g.edgeBegin(), g.edgeEnd());
        }

        void reset() {
            for (auto vertex: vertices_) {
                vertex->reset();
            }
        }

        VertexIterator vertexBegin() { return vertices_.begin(); }
        VertexIterator vertexEnd() { return vertices_.end(); }

        EdgeIterator edgeBegin() { return  edges_.begin(); }
        EdgeIterator edgeEnd() { return edges_.end(); }

    private:
        std::deque<Vertex> vertices_;
        std::deque<Edge> edges_;
    };

    template <typename Data>
    class APFlag {
    private:
        bool visited_ = false;
    public:
        using DType = size_t;
        using LType = DType;
        using IDType = DType;
        DType dist;
        LType low;
        IDType id;
        VectorVertex<Data, APFlag<Data>>* prev;

        APFlag() : visited_{false}, dist{0}, low{0}, prev{nullptr} {}

        void visit() { visited_ = true; }
        bool visited() const { return visited_; }
        void reset() {
            visited_ = false;
            dist = 0;
            low = 0;
            prev = nullptr;
        }
    };
}

#endif
