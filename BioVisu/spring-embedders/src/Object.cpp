// Pestova Valentina UFR

#include <Object.h>

Object::Object()
  : mShader(0), mGeometry(0), mTransformation(Eigen::Matrix4f::Identity())
{}

void Object::attachMesh(/*const*/ Mesh* mesh)
{
  mGeometry = mesh;
}

void Object::attachShader(const Shader* shader)
{
  mShader = shader;
}

void Object::setTransformation(const Eigen::Matrix4f& mat)
{
  mTransformation = mat;
}

void Object::draw(const Camera &camera)
{
  mShader->activate();

  int viewMatrixLoc = mShader->getUniformLocation("view_matrix");
  int projectionMatrixLoc = mShader->getUniformLocation("projection_matrix");

  if(viewMatrixLoc >= 0) {
    Eigen::Matrix4f viewMatrix = camera.viewMatrix() * mTransformation;
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix.data());
  }
  if(projectionMatrixLoc >= 0)
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, camera.projectionMatrix().data());

  mGeometry->drawGeometry(mShader->id());
}

