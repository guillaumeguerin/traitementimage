#include "graph.h"
#include "OpenGL.h"
Graph::Graph(){
}

void Graph::loadGraph(char* filepath)

{

QFile file(filepath);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream filestream(&file);

    QString line;


    while (line.at(0) == '#' || line.isEmpty())
        line = filestream.readLine();


    while (!filestream.atEnd()) {

   QStringList listCoords = line.split(" ");

        Vector3f v(listCoords.at(0).toFloat(), listCoords.at(1).toFloat(), listCoords.at(2).toFloat());
            std::vector<int> n;

        for (unsigned int i=3; i<listCoords.size()-3; ++i) {
            n.push_back(listCoords.at(i).toInt());
        }
    line = filestream.readLine();

    vertices.push_back(Vertex(v,n));
    }

    file.close();

    for(int i = 0; i < vertices.size(); ++i){

        std::cout<<vertices[i].coordinates(0);
        for(int j = 0; j<vertices[i].neighbors.size(); ++j)
            edges.push_back(Vector3i(vertices[i].coordinates(0), vertices[i].coordinates(1), vertices[i].coordinates(2)));
    }

   }

void Graph::draw(const Camera &camera, Shader& mShader){
GL_TEST_ERR;

initBuffers();
    mShader.activate();

    int viewMatrixLoc = mShader.getUniformLocation("view_matrix");GL_TEST_ERR;
    int projectionMatrixLoc = mShader.getUniformLocation("projection_matrix");
GL_TEST_ERR;
    if(viewMatrixLoc >= 0) {
      Eigen::Matrix4f viewMatrix = camera.viewMatrix();
      glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix.data());
    }GL_TEST_ERR;
    if(projectionMatrixLoc >= 0)
      glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, camera.projectionMatrix().data());
GL_TEST_ERR;
    int prg_id = mShader.id();

    // bind the vertex array
    glBindVertexArray(mVertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

    int vertex_loc = glGetAttribLocation(prg_id, "vtx_position");

    // specify the vertex data
    if(vertex_loc >= 0)
    {
      glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);GL_TEST_ERR;
      glEnableVertexAttribArray(vertex_loc);
    }

    int texcoord_loc = glGetAttribLocation(prg_id, "vtx_texcoord");GL_TEST_ERR;
    if(texcoord_loc >= 0)
    {
      glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
      glEnableVertexAttribArray(texcoord_loc);GL_TEST_ERR;
    }

    // send the geometry
  glDrawElements(GL_LINES, 2*edges.size(), GL_UNSIGNED_INT, (void*)0);

    if(vertex_loc >= 0)
      glDisableVertexAttribArray(vertex_loc);

    // release the vertex array
    glBindVertexArray(0);

    GL_TEST_ERR;
}

Graph::Graph(std::vector<Vertex> vtx){

    for(int i = 0; i < vtx.size(); ++i){
        vertices.push_back(vtx[i]);

        for(int j = 0; j<vtx[i].neighbors.size(); ++j)
            edges.push_back(Vector3i(vtx[i].coordinates(0), vtx[i].coordinates(1), vtx[i].coordinates(2)));
    }

    initBuffers();

}

void Graph::initBuffers() {
  GL_TEST_ERR;

  glGenVertexArrays(1,&mVertexArrayId);
  glBindVertexArray(mVertexArrayId);

  glGenBuffers(1,&mVertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices[0].coordinates.data(), GL_STATIC_DRAW);

  glGenBuffers(1,&mIndexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i)*edges.size(), edges[0].data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  GL_TEST_ERR;
}

int Graph :: getNumVertices(){
    return vertices.size();
}
int Graph :: getNumNeighbors(){

    int nbEdges=0;
    for(int i=0; i<vertices.size();i++)
        for(int j=0; j<vertices[i].neighbors.size(); j++)
            nbEdges++;

    return nbEdges;
}

int Graph :: getArrayDims(int &verticesDim, int &neighboursDim){

    verticesDim = (int)floor(sqrt(vertices.size()));
    neighboursDim = (int)floor(sqrt(getNumNeighbors()));
}

int Graph :: getVertMatrixDim(){

    return ((int)floor(sqrt(vertices.size())));
}

int Graph :: getNeighboursMatrixDim(){

    return ((int)floor(sqrt(getNumNeighbors())));
}

void Graph :: updateVertices(GLfloat* T_cur){

    int sizeVertexArray = sizeof(T_cur)/4;

    for (int i = 0; i< sizeVertexArray; i+=4){

        if(T_cur[i]!=0 || T_cur[i+1]!=0 || T_cur[i+2]!=0){
        vertices[i/4].coordinates[0] = T_cur[i];
        vertices[i/4].coordinates[1] = T_cur[i + 1];
        vertices[i/4].coordinates[2] = T_cur[i + 2];
        }
    }
}

void Graph :: fillArrays(GLfloat* T_cur, GLfloat* T_next, GLfloat* T_nodes, GLfloat* T_edgesList){

    int vertexMartixDim = (int)floor(sqrt(vertices.size()));
    int sizeVertexArray = vertices.size();
    int nbEdges = getNumNeighbors();

    int edgesListMartixDim = (int)floor(sqrt(nbEdges));

    std::cout<<"vertexMartixDim: "<<vertexMartixDim<<", sizeVertexArray: "<<sizeVertexArray<<", nbEdges :"<<
               nbEdges<<", edgesListMartixDim: "<<edgesListMartixDim<<std::endl;

    T_cur = new GLfloat[sizeVertexArray*4];
    T_next = new GLfloat[sizeVertexArray*4];
    T_nodes = new GLfloat[sizeVertexArray*4];
    T_edgesList = new GLfloat[nbEdges*nbEdges*4];


    int Tedges_listIndex = 0, prevTedges_listIndex = 0;

    for(int i = 0, j = i*4; i<sizeVertexArray; ++i, j = i*4){

        T_cur[j] = vertices[i].coordinates[0];
        T_cur[j+1] = vertices[i].coordinates[1];
        T_cur[j+2] = vertices[i].coordinates[2];
        T_cur[j+3] = 1;

        T_next[j] = vertices[i].coordinates[0];
        T_next[j+1] = vertices[i].coordinates[1];
        T_next[j+2] = vertices[i].coordinates[2];
        T_next[j+3] = 1;

        T_nodes[j + 2] = vertices[i].neighbors.size();
        T_nodes[j + 3] = 1;

        prevTedges_listIndex = Tedges_listIndex;


        for(int k = 0; k<vertices[i].neighbors.size(); ++k){

                    T_edgesList[Tedges_listIndex + 1] =(int)floor((float) vertices[i].neighbors[k]/vertexMartixDim); // on prend la partie entier du numero de voisin et la multiplie par 4 car on a 4 elements, donc il y 4 fois plus d'elements dans la matrice que dans la liste de voisins
                    T_edgesList[Tedges_listIndex ] = vertices[i].neighbors[k] - T_edgesList[Tedges_listIndex+1]*vertexMartixDim; // la difference est la coordonnée y

                    T_edgesList[Tedges_listIndex + 2] = 0;
                    T_edgesList[Tedges_listIndex + 3] = 1;

                    Tedges_listIndex += 4;
                }

        T_nodes[j + 1] = prevTedges_listIndex/(edgesListMartixDim*4);
        T_nodes[j] = prevTedges_listIndex/4 - T_nodes[j + 1]*edgesListMartixDim;

    }


/*for(int j = 0; j<sizeVertexArray*4; j+=4){
    std::cout<< j << " cur " <<T_cur[j] << " " << T_cur[j+1] << " " << T_cur[j+2] << " " << T_cur[j+3] << std::endl;
    }

for(int j = 0; j<sizeVertexArray*4; j+=4){
    std::cout << j  << "next " << T_next[j] << " " << T_next[j+1] << " " << T_next[j+2] << " " << T_next[j+3] << std::endl;
    }

for(int j = 0; j<sizeVertexArray*4; j+=4){
    std::cout << j  << "nodes " << T_nodes[j] << " " << T_nodes[j+1] << " " << T_nodes[j+2] << " " << T_nodes[j+3] << std::endl;
    }

for(int j = 0; j<nbEdges*4; j+=4)
    std::cout << j  << "T_edgesList " << T_edgesList[j] << " " << T_edgesList[j+1] << " " << T_edgesList[j+2] << " " << T_edgesList[j+3] << std::endl;
*/
}

