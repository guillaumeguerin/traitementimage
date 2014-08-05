// Copyright (C) 2008-2011 Gael Guennebaud <gael.guennebaud@inria.fr>

// Pestova Valentina UFR

#include "Mesh.h"
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <limits>

#include <QCoreApplication>
#include <Eigen/Geometry>
#include "../ObjFormat/ObjFormat.h"

using namespace Eigen;

Mesh::Mesh(const std::string& filename)
  : mIsInitialized(false)
{
  std::string ext = filename.substr(filename.size()-3,3);
  if(ext=="off" || ext=="OFF")
    loadOFF(filename);
  else if(ext=="obj" || ext=="OBJ")
    loadOBJ(filename);
  else
    std::cerr << "Mesh: extension \'" << ext << "\' not supported." << std::endl;
}

void Mesh::loadOFF(const std::string& filename)
{
  std::ifstream in(filename.c_str(),std::ios::in);
  if(!in)
  {
    std::cerr << "File not found " << filename << std::endl;
    return;
  }

  std::string header;
  in >> header;

  // check the header file
  if(header != "OFF")
  {
      std::cerr << "Wrong header = " << header << std::endl;
      return;
  }

  int nofVertices, nofFaces, inull;
  int nb, id0, id1, id2;
  Vector3 v;

  in >> nofVertices >> nofFaces >> inull;

  for(int i=0 ; i<nofVertices ; ++i)
  {
      in >> v.x() >> v.y() >> v.z();
      mVertices.push_back(v);
  }

  for(int i=0 ; i<nofFaces ; ++i)
  {
      in >> nb >> id0 >> id1 >> id2;
      assert(nb==3);
      mFaces.push_back(FaceIndex(id0, id1, id2));
  }

  in.close();

  computeNormals();
}

void Mesh::loadOBJ(const std::string& filename)
{
  ObjMesh* pRawObjMesh = ObjMesh::LoadFromFile(filename);

  if (!pRawObjMesh)
  {
      std::cerr << "Mesh::loadObj: error loading file " << filename << "." << std::endl;
      return;
  }

  // Makes sure we have an indexed face set
  ObjMesh* pObjMesh = pRawObjMesh->createIndexedFaceSet(Obj::Options(Obj::AllAttribs|Obj::Triangulate));
  delete pRawObjMesh;
  pRawObjMesh = 0;

  // copy vertices
  mVertices.resize(pObjMesh->positions.size());

  for (int i=0 ; i<pObjMesh->positions.size() ; ++i)
  {
    mVertices[i] = Vertex(Vector3f(pObjMesh->positions[i]));

    if(!pObjMesh->texcoords.empty())
      mVertices[i].texcoord = Vector2f(pObjMesh->texcoords[i]);

    if(!pObjMesh->normals.empty())
      mVertices[i].normal = Vector3f(pObjMesh->normals[i]);
  }

  // copy faces
  for (int smi=0 ; smi<pObjMesh->getNofSubMeshes() ; ++smi)
  {
    const ObjSubMesh* pSrcSubMesh = pObjMesh->getSubMesh(smi);

    mFaces.reserve(pSrcSubMesh->getNofFaces());

    for (uint fid = 0 ; fid<pSrcSubMesh->getNofFaces() ; ++fid)
    {
      ObjConstFaceHandle srcFace = pSrcSubMesh->getFace(fid);
      mFaces.push_back(Vector3i(srcFace.vPositionId(0), srcFace.vPositionId(1), srcFace.vPositionId(2)));
    }
  }

  if(pObjMesh->normals.empty())
  {
    std::cout << "compute normals\n";
    computeNormals();
  }
}

Mesh::~Mesh()
{
  if(mIsInitialized)
  {
    glDeleteBuffers(1,&mVertexBufferId);
    glDeleteBuffers(1,&mIndexBufferId);
  }
}

void Mesh::makeUnitary()
{
  // computes the lowest and highest coordinates of the axis aligned bounding box,
  // which are equal to the lowest and highest coordinates of the vertex positions.
  Eigen::Vector3f lowest, highest;
  lowest.fill(std::numeric_limits<float>::max());   // "fill" sets all the coefficients of the vector to the same value
  highest.fill(-std::numeric_limits<float>::max());

  for(VertexArray::iterator v_iter = mVertices.begin() ; v_iter!=mVertices.end() ; ++v_iter)
  {
    // - v_iter is an iterator over the elements of mVertices,
    //   an iterator behaves likes a pointer, it has to be dereferenced (*v_iter, or v_iter->) to access the referenced element.
    // - Here the .aray().min(_) and .array().max(_) operators work per component.
    //
    lowest  = lowest.array().min(v_iter->position.array());
    highest = highest.array().max(v_iter->position.array());
  }

  // TODO: appliquer une transformation à tous les sommets de mVertices de telle sorte
  // que la boite englobante de l'objet soit centrée en (0,0,0)  et que sa plus grande dimension soit de 1
  Eigen::Vector3f center = (lowest+highest)/2.0;
  float m = (highest-lowest).maxCoeff();
  for(VertexArray::iterator v_iter = mVertices.begin() ; v_iter!=mVertices.end() ; ++v_iter)
    v_iter->position = (v_iter->position - center) / m;
}

void Mesh::computeNormals() {
  /* TODO dans le TD 7 */
}

void Mesh::initBuffers() {
  GL_TEST_ERR;

  assert(!mIsInitialized);

  mIsInitialized = true;

  glGenVertexArrays(1,&mVertexArrayId);
  glBindVertexArray(mVertexArrayId);

  glGenBuffers(1,&mVertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertices.size(), mVertices[0].position.data(), GL_STATIC_DRAW);

  glGenBuffers(1,&mIndexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FaceIndex)*mFaces.size(), mFaces[0].data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  GL_TEST_ERR;
}

void Mesh::drawGeometry(int prg_id) const
{
  GL_TEST_ERR;

  // bind the vertex array
  glBindVertexArray(mVertexArrayId);

  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

  int vertex_loc = glGetAttribLocation(prg_id, "vtx_position");

  // specify the vertex data
  if(vertex_loc >= 0)
  {
    glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(vertex_loc);
  }

  /* TODO 1.2: Envoyer les coordonnées de texture. */
  int texcoord_loc = glGetAttribLocation(prg_id, "vtx_texcoord");
  if(texcoord_loc >= 0)
  {
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex) - sizeof(Vertex::texcoord)));
    glEnableVertexAttribArray(texcoord_loc);
  }

  // send the geometry
  glDrawElements(GL_TRIANGLES, 3*mFaces.size(), GL_UNSIGNED_INT, (void*)0);

  if(vertex_loc >= 0)
    glDisableVertexAttribArray(vertex_loc);

  // release the vertex array
  glBindVertexArray(0);

  GL_TEST_ERR;
}



