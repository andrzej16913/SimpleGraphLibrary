#ifndef SGL_FUNCTIONS_HPP
#define SGL_FUNCTIONS_HPP

#include <concepts>
#include <functional>
#include <iterator>

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
	void deepFirstSearchVertex(Vertex& vertex, PreorderCallable& preorderCallable,
                               PostorderCallable& postorderCallable) {
		
		vertex.visit();
		std::invoke(preorderCallable, vertex);

        for (auto neighbor = vertex.vertexBegin(); neighbor != vertex.vertexEnd(); ++neighbor) {
            if (!neighbor->visited()) {
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
	requires std::invocable<PreorderCallable, typename Graph::vertexType&> &&
	         std::invocable<PostorderCallable, typename Graph::vertexType&>
	void deepFirstSearch(Graph& graph, PreorderCallable& preorderCallable,
                         PostorderCallable& postorderCallable) {
        for (auto it = graph.vertexBegin(); it != graph.vertexEnd(); ++it) {
            if (!it->visited()) {
                deepFirstSearchVertex(*it, preorderCallable, postorderCallable);
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

    template <GraphID InputGraph, GraphRandomlyAccessible OutputGraph>
    requires std::same_as<typename  InputGraph::VertexType, typename OutputGraph::VertexType>
    OutputGraph transponse(InputGraph& input) {
        OutputGraph output = {};
        output.addVertices(input.vertexBegin(), input.vertexEnd());

        for (auto from = input.vertexBegin(); from != input.vertexEnd(); ++from) {
            for (auto to = from->vertexBegin(); to != from->vertexEnd(); ++to) {
                output[to->flags.id].addVertex(output[from->flags.id]);
            }
        }

        return output;
    }
	
	template <GraphID InputGraph>
	std::vector<std::vector<typename InputGraph::VertexType*>>
    stronglyConnectedComponents(InputGraph& graph) {
		typedef typename InputGraph::VertexType Vertex;
		typedef typename Vertex::FlagType::IDType IDType;

		std::vector<IDType> indexes;
		postorderDeepFristSearch(graph, [&indexes] (Vertex& v) {
			indexes.push_back(v.flags.id);
		});

        graph.reset();
		
		RandomAccessGraph graphT = transponse(graph);
        std::vector<std::vector<Vertex*>> output = {};
		
		for (auto it = rbegin(indexes); it != rend(indexes); ++it) {
			Vertex* v = &graphT[*it];
            output.emplace_back({});
			std::vector<Vertex>& tree = output.back();

			if (!v->visited()) {
                preorderDeepFirstSearchVertex(*v, [&tree](Vertex& ve) {
                    tree.push_back(&ve);
                });
			}
		}
		
		return output;
	}
}

#endif
