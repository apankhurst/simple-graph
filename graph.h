/*
 * Alex Pankhurst
 * 02/08/2018
 */

#pragma once

#include <map>
#include <vector>
#include <iostream>

/*
 * Bool2Type struct allows for overloading of functions 
 * that differ between directed and undirected graphs
 */
template <bool b>
struct Bool2Type { enum {value = b}; };

/* 
 * template graph class that allows for the creation of directed 
 * and undirected graphs based on an adjacency matrix
 * V: vertex type
 * E: edge type 
 * D: true = directed; false = undirected
 */
template <typename V, typename E, bool D>
class Graph {
private:
  int v_count; // vertex count
  int e_count; // edge count
  std::map<V,int> vertex_index; // vertex -> index in adjacency matrix
  std::vector<V> index_vertex;  // index in adjacency matrix -> vertex
  std::vector<std::vector<E> > adj_matrix; // adjacency matrix

  /* overloaded functions that differ between directed and undirected graphs */
  void add_edge(Bool2Type<true>, const V v1, const V v2, const E weight); 
  V remove_vertex(Bool2Type<true>, const V v);
  E remove_edge(Bool2Type<true>, const V v1, const V v2);

  void add_edge(Bool2Type<false>, const V v1, const V v2, const E weight); 
  V remove_vertex(Bool2Type<false>, const V v);
  E remove_edge(Bool2Type<false>, const V v1, const V v2);


public:
  Graph(): v_count(0), e_count(0) {}
  void add_vertex(const V v);
  void add_edge(const V v1, const V v2, const E weight) {
    add_edge(Bool2Type<D>(), v1, v2, weight); 
  }
  V remove_vertex(const V v){
    return remove_vertex(Bool2Type<D>(), v);
  }
  E remove_edge(const V v1, const V v2){
    return remove_edge(Bool2Type<D>(), v1, v2);
  }
  V get_vertex(const V v);
  E get_edge(const V v1, const V v2);
  bool has_vertex(const V v);
  bool has_edge(const V v1, const V v2);
  std::vector<V> get_neighbors(const V v);
  int num_vertices();
  int num_edges();
  bool empty();
  void print();
};

/*------ GENERAL FUNCTIONS -----*/

/*
 * add a vertex to the graph
 * input:  V vertex
 * output: void
 */
template<typename V, typename E, bool D> 
void Graph<V,E,D>::add_vertex(const V v){
  vertex_index[v] = v_count;
  index_vertex.push_back(v);
  adj_matrix.push_back(std::vector<E>(v_count+1));
  for(int r = 0; r <= v_count; r++){
    adj_matrix[r].push_back(E());
  }
  for(int c = 0; c <= v_count; c++){
    adj_matrix[v_count].push_back(E());
  }
  v_count++;
}

/*
 * check if a vertex exists
 * input:  V vertex
 * output: bool
 */
template <typename V, typename E, bool D>
bool Graph<V,E,D>::has_vertex(const V v){
  return vertex_index.find(v) != vertex_index.end(); 
}


/*
 * check if an edge exists from v1 to v2
 * input:  V v1, V v2
 * output: bool
 */
template <typename V, typename E, bool D>
bool Graph<V,E,D>::has_edge(const V v1, const V v2){
  int r = vertex_index[v1];
  int c = vertex_index[v2];

  return adj_matrix[r][c] != E();
}

/*
 * return a std::vector<V> of the neighbors of a vertex
 * if a vertex doesn't exist return an empty vector
 * input:  V v
 * output: std::vector<V>
 */
template<typename V, typename E, bool D>
std::vector<V> Graph<V,E,D>::get_neighbors(const V v){
  std::vector<V> neighbors;
  if(has_vertex(v)){
    int r = vertex_index[v];;
    for(int c = 0; c < v_count; c++){
      if(adj_matrix[r][c] != E()){
	neighbors.push_back(index_vertex[c]);
      }
    }
  }
  return neighbors;
}

/* 
 * get the number of vertices in a graph
 * input:  void
 * output: int
 */
template<typename V, typename E, bool D>
int Graph<V,E,D>::num_vertices(){
  return v_count;
}

/* 
 * get the number of edges in a graph
 * input:  void
 * output: int
 */
template<typename V, typename E, bool D>
int Graph<V,E,D>::num_edges(){
  return e_count;
}

/* 
 * check if a graph is empty
 * input:  void
 * output: bool
 */
template<typename V, typename E, bool D>
bool Graph<V,E,D>::empty(){
  return v_count == 0;
}

/* 
 * get the value of a vertex
 * if the vertex doesn't exits return the empty object
 * input:  V v
 * output: V
 */
template<typename V, typename E, bool D>
V Graph<V,E,D>::get_vertex(const V v){
  V vertex = V();
  if(has_vertex(v)){
    vertex = v;
  }
  return vertex;
}

/* 
 * get the value of an edge between v1 and v2
 * if the edge doesn't exits return the empty object
 * input:  V v1, V v2
 * output: E
 */
template<typename V, typename E, bool D>
E Graph<V,E,D>::get_edge(const V v1, const V v2){
  E edge = E();
  
  if(has_vertex(v1) && has_vertex(v2)){
    int r = vertex_index[v1];
    int c = vertex_index[v2];
  
    edge = adj_matrix[r][c];
  }
  return edge;
}

/*
 * print the graph
 * input:  void 
 * output: void 
 */
template<typename V, typename E, bool D>
void Graph<V,E,D>::print(){
  std::cout << " ";
  for(int c = 0; c < v_count; c++){
    std::cout << " " << index_vertex[c];
  }
  std::cout << "\n";
  for(int r = 0; r < v_count; r++){
    std::cout << index_vertex[r] << " ";
    for(int c = 0; c < v_count; c++){
      std::cout << adj_matrix[r][c] << " ";
    }
    std::cout << "\n";
  }
}

/*----- DIRECTED SPECIFIC FUNCTIONS -----*/

/* 
 * add an edge to a graph
 * input:  V v1, V v2, E weight
 * output: void
 */
template<typename V, typename E, bool D>
void Graph<V,E,D>::add_edge(Bool2Type<true>, const V v1, const V v2, const E weight){
  if(!has_vertex(v1) || !has_vertex(v2)) {
    return;
  }

  int r = vertex_index[v1];
  int c = vertex_index[v2];
  
  adj_matrix[r][c] = weight;

  e_count++;
}

/* 
 * remove a vertex from the graph and return its value
 * if the vertex doesn't exist return the empty object
 * input:  V v
 * output: V
 */
template<typename V, typename E, bool D>
V Graph<V,E,D>::remove_vertex(Bool2Type<true>, const V v){
  V vertex = V();
  if(has_vertex(v)){
    int r = vertex_index[v];
    for(E c : adj_matrix[r]){
      if(c != E()){
	e_count--;
      }
    }
    adj_matrix.erase(adj_matrix.begin() + r);
    for(std::vector<E> row : adj_matrix){
      if(row[r] != E()){
	e_count--;
      }
      row.erase(row.begin() + r);
    }
    for(int i = r+1; i < v_count; i++){
      vertex_index[index_vertex[i]] -= 1;
    }
    index_vertex.erase(index_vertex.begin() + r);
    vertex_index.erase(v);
    v_count--;
  }
  return vertex;
}

/*
 * remove an edge from the graph and return its value
 * if the edge doesn't exist return the empty object
 * input:  V v1, V v2
 * output: E
 */
template<typename V, typename E, bool D>
E Graph<V,E,D>::remove_edge(Bool2Type<true>, const V v1, const V v2){
  E edge = E();
  if(has_edge(v1, v2)){
    int r = vertex_index[v1];
    int c = vertex_index[v2];
    edge = adj_matrix[r][c];
    adj_matrix[r][c] = E();
    e_count--;
  }
  return edge;
}

/*----- UNDIRECTED SPECIFIC FUNCTIONS -----*/

/* 
 * add an edge to a graph
 * input:  V v1, V v2, E weight
 * output: void
 */
template<typename V, typename E, bool D>
void Graph<V,E,D>::add_edge(Bool2Type<false>, const V v1, const V v2, const E weight){
  if(!has_vertex(v1) || !has_vertex(v2)) {
    return;
  }

  int r = vertex_index[v1];
  int c = vertex_index[v2];
  
  adj_matrix[r][c] = weight;
  adj_matrix[c][r] = weight;

  e_count++;
}

/* 
 * remove a vertex from the graph and return its value
 * if the vertex doesn't exist return the empty object
 * input:  V v
 * output: V
 */
template<typename V, typename E, bool D>
V Graph<V,E,D>::remove_vertex(Bool2Type<false>, const V v){
  V vertex = V();
  if(has_vertex(v)){
    int r = vertex_index[v];
    for(E c : adj_matrix[r]){
      if(c != E()){
	e_count--;
      }
    }
    adj_matrix.erase(adj_matrix.begin() + r);
    for(std::vector<E> row : adj_matrix){
      row.erase(row.begin() + r);
    }
    for(int i = r+1; i < v_count; i++){
      vertex_index[index_vertex[i]] -= 1;
    }
    index_vertex.erase(index_vertex.begin() + r);
    vertex_index.erase(v);
    v_count--;
  }
  return vertex;
}

/*
 * remove an edge from the graph and return its value
 * if the edge doesn't exist return the empty object
 * input:  V v1, V v2
 * output: E
 */
template<typename V, typename E, bool D>
E Graph<V,E,D>::remove_edge(Bool2Type<false>, const V v1, const V v2){
  E edge = E();
  if(has_edge(v1, v2)){
    int r = vertex_index[v1];
    int c = vertex_index[v2];
    edge = adj_matrix[r][c];
    adj_matrix[r][c] = E();
    adj_matrix[c][r] = E();
    e_count--;
  }
  return edge;
}

/*----- KEYWORD DEFINITIONS -----*/

/* define DirectedGraph<V,E> */
template <typename V, typename E>
using DirectedGraph = Graph<V,E,true>;

/* define UndirectedGraph<V,E> */
template <typename V, typename E>
using UndirectedGraph = Graph<V,E,false>;
