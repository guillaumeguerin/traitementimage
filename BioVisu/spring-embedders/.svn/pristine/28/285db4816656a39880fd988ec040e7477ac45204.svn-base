#include <vector>

#include <Eigen/Geometry>
#include "OpenGL.h"
#include "Camera.h"
#include "Object.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace Eigen;

struct Vertex {



public:
    Vector3f coordinates;
    std::vector<int> neighbors;
    Vector2f texcoord;

    Vertex(){}
    Vertex(Vector3f c, std::vector<int>  n){
        coordinates = c;
        neighbors = n;

    }
    void setCoordinates(Vector3f c){
        coordinates = c;
    }
    void setNeighbors(std::vector<int>  n){
        neighbors = n;
    }
};


class Graph {


public:

    std::vector<Vertex> vertices;
    std::vector<Vector3i> edges;

    unsigned int mVertexBufferId;
    unsigned int mIndexBufferId;
    unsigned int mVertexArrayId;

    Graph();

    // creation de graph depuis un tableau de sommets
    Graph(std::vector<Vertex> vtx);
void loadGraph(char* filepath);
    // remplissage de tableaux pour rendu
    void fillArrays(GLfloat* T_cur, GLfloat* T_next, GLfloat* T_nodes, GLfloat* T_edgesList);
    void initBuffers();

    void draw(const Camera& camera, Shader& mShader);

    int getNumVertices();
    int getNumNeighbors();

    int getVertMatrixDim();
    int getNeighboursMatrixDim();

    int getArrayDims(int &verticesDim, int &neighboursDim);

    void updateVertices(GLfloat* T_cur);
};
