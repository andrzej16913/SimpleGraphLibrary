#ifndef SGL_FUNCTIONS_HPP
#define SGL_FUNCTIONS_HPP

#include <concepts>
#include <functional>
#include <iterator>
#include <queue>

#include "sgl_concepts.hpp"
#include "sgl_classes.hpp"

namespace sgl {
	template <VertexVisit Vertex, typename Callable>
	requires std::invocable<Callable, Vertex&>
	void preorderDeepFirstSearchVertex(Vertex& vertex, Callable& callable) {
		vertex.visit();
		std::invoke(callable, vertex);
		
		for (auto neighbor = vertex.vertexBegin(); neighbor != vertex.vertexEnd(); ++neighbor) {
			if (!neighbor->visited()) {
                preorderDeepFirstSearchVertex(*neighbor, callable);
			}
		}
	}

    template <VertexVisit Vertex, typename Callable>
    requires std::invocable<Callable, Vertex&>
	void postorderDeepFirstSearchVertex(Vertex& vertex, Callable& callable) {
		vertex.visit();

        for (auto neighbor = vertex.vertexBegin(); neighbor != vertex.vertexEnd(); ++neighbor) {
            if (!neighbor->visited()) {
                postorderDeepFirstSearchVertex(*neighbor, callable);
			}
		}
		
		std::invoke(callable, vertex);
	}
	
	template <VertexVisit Vertex, typename PreorderCallable, typename PostorderCallable>
	requires std::invocable<PreorderCallable, Vertex&> &&
	         std::invocable<PostorderCallable, Vertex&>
	void deepFirstSearchVertex(Vertex& vertex, const PreorderCallable& preorderCallable,
                               const PostorderCallable& postorderCallable) {
		
		vertex.flags.visit();
		std::invoke(preorderCallable, vertex);

        for (auto neighbor = vertex.vertexBegin(); neighbor != vertex.vertexEnd(); ++neighbor) {
            if (!neighbor->flags.visited()) {
                deepFirstSearchVertex(*neighbor, preorderCallable, postorderCallable);
			}
		}
		
		std::invoke(postorderCallable, vertex);
	}
	
	template <GraphVisit Graph, typename Callable>
	requires std::invocable<Callable, typename Graph::vertexType&>
	void preorderDeepFirstSearch(Graph& graph, Callable& callable) {
		for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
			if (!it->visited()) {
                preorderDeepFirstSearchVertex(*it, callable);
			}
		}
	}

    template<GraphVisit Graph, typename Callable>
    requires std::invocable<Callable, typename Graph::vertexType &>
    void postorderDeepFirstSearch(Graph & graph, Callable & callable) {
        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            if (!it->visited()) {
                postorderDeepFirstSearchVertex(*it, callable);
            }
        }
    }

    template <GraphVisit Graph, typename PreorderCallable, typename PostorderCallable>
	requires std::invocable<PreorderCallable, typename Graph::VertexType&> &&
	         std::invocable<PostorderCallable, typename Graph::VertexType&>
	void deepFirstSearch(Graph& graph, const PreorderCallable& preorderCallable,
                         const PostorderCallable& postorderCallable) {
        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            if (!it->flags.visited()) {
                deepFirstSearchVertex(*it, preorderCallable, postorderCallable);
			}
		}
	}

    template <VertexVisit Vertex, typename PreorderCallable, typename PostorderCallable>
    requires std::invocable<PreorderCallable, Vertex&> &&
             std::invocable<PostorderCallable, Vertex&>
    void breadthFirstSearchVertex(Vertex& vertex, const PreorderCallable& preorderCallable,
                               const PostorderCallable& postorderCallable) {

        std::queue<Vertex*> queue = {};

        queue.push(&(*vertex));
        while (!queue.empty()) {
            Vertex* ptr = queue.front();
            queue.pop();

            ptr->flags.visit();
            std::invoke(preorderCallable, *ptr);

            for (auto it = ptr->vertexBegin(); it != ptr->vertexEnd(); ++it) {
                if (!it->flags.visited()) {
                    queue.push(&(*it));
                }
            }

            std::invoke(postorderCallable, *ptr);
        }
    }

    template <GraphVisit Graph, typename PreorderCallable, typename PostorderCallable>
    requires std::invocable<PreorderCallable, typename Graph::VertexType&> &&
             std::invocable<PostorderCallable, typename Graph::VertexType&>
   void breadthFirstSearch(Graph& graph, const PreorderCallable& preorderCallable,
                           const PostorderCallable& postorderCallable) {
       for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
           if (!it->flags.visited()) {
               breadthFirstSearchVertex(*it, preorderCallable, postorderCallable);
           }
       }
   }
	/*
	template <typename Graph, typename Vertex, typename Callable>
	requires std::same_as<typename Graph::vertexType, Vertex> &&
		std::invocable<Callable, Vertex&>
	void dfs(Graph& graph, Callable callable) {
		std::stack<Vertex*> stack;
		
		for (auto vertex: graph) {
			std::cerr << "err: " << vertex->getData() << std::endl;
			if (!vertex->visited()) {
				stack.push(vertex);
				
				while (!stack.empty()) {
					auto v = stack.top();
					stack.pop();
					
					v->visit();
					//callable(v);
					std::invoke(callable, *v);
					
					for (auto v_neighbor: *v) {
						if (!v_neighbor->visited())
							stack.push(v_neighbor);
					}
				}
			}
		}
	}
	*/

    template <GraphID InputGraph, GraphDirected OutputGraph>
    requires std::same_as<typename  InputGraph::VertexType, typename OutputGraph::VertexType> &&
             GraphDirected<InputGraph>
    OutputGraph transponse(InputGraph& input) {
        OutputGraph output = {};
        output.addVertices(input.vertexBegin(), input.vertexEnd());

        for (auto from = input.vertexBegin(); from != input.vertexEnd(); ++from) {
            for (auto to = from->vertexBegin(); to != from->vertexEnd(); ++to) {
                //output[to->flags.id].addEdge(output[from->flags.id]);
                output.addEdge(to->flags.id, from->flags.id);
            }
        }

        return output;
    }
	
	template <GraphID InputGraph>
	requires GraphDirected<InputGraph>
	std::vector<std::vector<typename InputGraph::IDType>>
    stronglyConnectedComponents(InputGraph& graph) {
		typedef typename InputGraph::VertexType Vertex;
		typedef typename Vertex::FlagType::IDType IDType;

		std::vector<IDType> indexes;
        deepFirstSearch(graph, [](Vertex& v) {}, [&indexes](Vertex& v) {
			indexes.push_back(v.flags.id);
		});

        graph.reset();
		
		auto graphT = transponse<InputGraph, RandomAccessGraph<Vertex, InputGraph::DIRECTED>>(graph);
        std::vector<std::vector<IDType>> output = {};
		
		for (auto it = rbegin(indexes); it != rend(indexes); ++it) {
			Vertex* v = &graphT[*it];

			if (!v->flags.visited()) {
                output.push_back(std::vector<IDType>());
                std::vector<IDType>& tree = output.back();
                deepFirstSearchVertex(*v, [&tree](Vertex& ve) {
                    tree.push_back(ve.flags.id);
                }, [](Vertex& v) {});
			}
		}
		
		return output;
	}

    template <VertexAP Vertex, HasPushBack Container>
    requires std::same_as<Vertex*, typename Container::value_type> &&
             std::integral<typename Vertex::FlagType::DType>
    void articulationPoints(Vertex& v, Container& container, typename Vertex::FlagType::DType depth) {
        size_t childCount = 0;
        bool isArticulation = false;
        v.flags.visit();
        v.flags.dist = depth;
        v.flags.low = depth;

        for (auto it = v.vertexBegin(); it != v.vertexEnd(); ++it) {
            if (!it->flags.visited()) {
                it->flags.prev = &v;
                ++childCount;
                articulationPoints(*it, container, depth + 1);
                if (it->flags.low >= v.flags.dist) {
                    isArticulation = true;
                }
                v.flags.low = v.flags.low < it->flags.low ? v.flags.low : it->flags.low;
            } else if (&(*it) != v.flags.prev) {
                v.flags.low = v.flags.low < it->flags.dist ? v.flags.low : it->flags.dist;
            }
        }

        if ((v.flags.prev != nullptr && isArticulation) || (v.flags.prev == nullptr && childCount > 1)) {
            container.push_back(&v);
        }
    }

    template <GraphAP Graph, HasPushBack Container>
    requires std::same_as<typename Graph::VertexType*, typename Container::value_type> &&
             std::integral<typename Graph::VertexType::FlagType::DType>
    void graphArticulationPoints(Graph& graph, Container& container) {
        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            if (!it->flags.visited()) {
                articulationPoints(*it, container, Graph::VertexType::FlagType::zeroDist());
            }
        }

        graph.reset();
    }

    template <GraphEdge Graph>
    requires GraphVisit<Graph> &&
             GraphDist<Graph> &&
             FlagPrev<typename Graph::VertexType::FlagType> &&
             EdgeWeight<typename Graph::EdgeType>
    void dijkstra(Graph& graph, typename Graph::VertexType& start) {
        using Vertex = typename Graph::VertexType;
        using Edge = typename Graph::EdgeType;

        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            it->flags.dist = Vertex::FlagType::maxDist();
            it->flags.prev = nullptr;
        }

        start.flags.dist = Vertex::FalgType::zeroDist();
        auto queue = std::priority_queue([](const Vertex* a, const Vertex* b) {
            return a->flags.dist > b->flags.dist;
            }, std::vector<Vertex*>());

        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            queue.push(&(*it));
        }

        while (!queue.empty()) {
            Vertex* vert = queue.top();
            queue.pop();
            if (!vert->visited()) {
                vert->visit();
                for (auto neighbour = vert->pairBegin(); neighbour != vert->pairEnd(); ++neighbour) {
                    Vertex& nv = *std::get<0>(*neighbour);
                    Edge& ne = *std::get<1>(*neighbour);
                    if (nv.flags.dist > vert->flags.dist + ne.values.weight()) {
                        nv.flags.dist += vert->flags.dist + ne.values.weight();
                        nv.flags.prev = vert;
                        queue.push(&nv);
                    }
                }
            }
        }
    }

    template <GraphEdge Graph, HasPushBack Container>
    requires std::same_as<typename Graph::EdgeType*, typename Container::value_type> &&
             GraphVisit<Graph> &&
             EdgeWeight<typename Graph::EdgeType>
    void prim(Graph& graph, Container& container) {
        using Vertex = typename Graph::VertexType;
        using Edge = typename Graph::EdgeType;
        using Pair = typename Vertex::PairType;

        Vertex& v1 = graph.vertexBegin();
        v1.visited();

        auto queue = std::priority_queue([](const Pair* a, const Pair* b) {
            return std::get<1>(*a)->value.weight() > std::get<1>(*b)->value.weight();
        });

        for (auto it = v1.pairBegin(); it != v1.pairEnd(); ++it) {
            queue.push(&(*it));
        }

        while (!queue.emdpty()) {
            Pair* pair = queue.top();
            Vertex* vert = std::get<0>(*pair);
            Edge* edge = std::get<1>(*pair);
            queue.pop();

            if (!vert->visited()) {
                vert->visit();
                container.push_back(edge);
                for (auto it = vert->pairBegin(); it != vert->pairEnd(); ++it) {
                    if (!std::get<0>(*it)->visited()) {
                        queue.push(&(*it));
                    }
                }
            }
        }
    }

    template <EdgeWeight Weight, IsVertex Vertex, typename IDType>
    void setMatrixPair(std::vector<std::vector<std::pair<Weight, Vertex*>>>& matrix, IDType from, IDType to,
                       Weight weight, Vertex* vertex) {
        std::get<0>(matrix[from][to]) = weight;
        std::get<1>(matrix[from][to]) = vertex;
    }

    template <GraphEdge Graph>
    requires GraphID<Graph> &&
             GraphDirected<Graph> &&
             GraphRandomlyAccessible<Graph> &&
             EdgeWeight<typename Graph::EdgeType>
    std::vector<std::vector<std::pair<typename Graph::EdgeType::ValueType::WeightType, typename Graph::VertexType*>>>
    floydWarshall(Graph& graph) {
        using Vertex = typename Graph::VertexType;
        using Edge = typename Graph::EdgeType;
        using Weight = typename Graph::EdgeType::ValueType::WeightType;
        using IDType = typename Graph::IDType;

        std::vector<std::vector<std::pair<Weight, Vertex*>>> matrix =
                {graph.vertexCount(), {graph.vertexCount(), {Weight::ValueType::maxWeight(), nullptr}}};

        for (auto it = graph.edgeBegin(); it != graph.edgeEnd(); ++it) {
            setMatrixPair(matrix, it->from(), it->to(), it->value.weight, &graph[it->from()]);
        }

        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            setMatrixPair(matrix, it->flags.id, it->flags.id, Weight::ValueType::zeroWeight(), &(*it));
        }

        for (size_t k = 0; k < graph.vertexCount(); ++k) {
            for (IDType i = 0; i < graph.vertexCount(); ++i) {
                for (IDType j = 0; j < graph.vertexCount(); ++j) {
                    Weight detour = std::get<0>(matrix[i][k]) + std::get<0>(matrix[k][j]);
                    if (std::get<0>(matrix[i][j]) > detour) {
                        setMatrixPair(matrix, i, j, detour, std::get<1>(matrix[k][j]));
                    }
                }
            }
        }

        return matrix;
    }
}

#endif
