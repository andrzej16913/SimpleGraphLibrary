#ifndef SGL_CLASSES_HPP
#define SGL_CLASSES_HPP

#include <concepts>
#include <cstddef>
#include <deque>
#include <iterator>
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

    template <IsVertex Vertex>
    class APFlag {
    private:
        bool visited_ = false;
    public:
        using DType = size_t;
        using LType = size_t;
        DType dist;
        LType low;
        Vertex* prev;

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

    template <typename Data, IsFlag Flag>
    class VectorVertex {
    private:
        std::vector<VectorVertex<Data, Flag>*> vertices_;
    public:
        Flag flags;
        Data data;
        static constexpr bool DIRECTED = false;
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
            to.vertices_.push_back(this);
        }

        void reset() {
            flags.reset();
        }

        iterator vertexBegin() { return iterator(vertices_.begin()); }
        iterator vertexEnd() { return iterator(vertices_.end()); }
    };

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
	
	template <IsVertex Vertex>
	class RandomAccessGraph {
		public:
		using iterator = typename std::deque<Vertex>::iterator;
		typedef Vertex VertexType;
        static constexpr bool DIRECTED = Vertex::DIRECTED;
		
		RandomAccessGraph() : vertices_{} {}
        RandomAccessGraph(const RandomAccessGraph& g) : vertices_{} {
            for (Vertex& v: g) {
                vertices_.emplace_back(v);
            }
        }

        RandomAccessGraph(RandomAccessGraph&& g) noexcept = default;

        RandomAccessGraph& operator=(const RandomAccessGraph& g) {
            if (this == &g)
                return *this;

            vertices_ = g.vertices_;

            return *this;
        }

        RandomAccessGraph& operator=(RandomAccessGraph&& g)  noexcept = default;

        Vertex& operator[] (size_t pos) { return vertices_[pos]; }
        const Vertex& operator[] (size_t pos) const { return vertices_[pos]; }

		void addVertex(const Vertex& vertex) { vertices_.emplace_back(vertex); }

        template <typename... Args>
        requires std::constructible_from<Vertex, Args...>
        void addVertex(Args&&... args) { vertices_.emplace_back(args...); }
		
		template <std::forward_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
		//requires std::same_as<Iterator*, Vertex>
		void addVertices(Iterator begin, Sentinel end) {
			while (begin != end) {
				vertices_.emplace_back(*begin);
				++begin;
			}
		}
		
		void reset() {
			for (auto vertex: vertices_) {
				vertex->reset();
			}
		}

		iterator vertexBegin() { return vertices_.begin(); }
		iterator vertexEnd() { return vertices_.end(); }
		
		/*void print() {
			for (size_t i = 0; i < vertices.size(); ++i) {
				auto v = vertices[i];
				std::cerr << "graph: " << v << ':' << v->getData() << std::endl;
			}
		}*/
		
		private:
		std::deque<Vertex> vertices_;
	};
}

#endif
