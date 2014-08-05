#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include "OpenGL.h"

#include <list>

#include <Eigen/Geometry>

#include <QGLWidget>
#include <QApplication>
#include <QTimer>

#include "graph.h"

class RenderingWidget : public QGLWidget
{
  Q_OBJECT

  Camera mCamera;
  float mCamPhy;
  float mCamTheta;
  float mCamDist;
  Eigen::Vector3f mCamLookAt;
  float mCamFov;

  Graph* graph;

  //pointers on the textures

  GLfloat* mTextureT_cur;
  GLfloat* mTextureT_next;
  GLfloat* mTextureT_nodes;
  GLfloat* mTextureT_edges_list;

  GLfloat* newPositions;
  GLfloat* nextTex;

  //id of textures for working in shaders
  GLuint mTextureT_curId;
  GLuint mTextureT_nextId;
  GLuint mTextureT_nodesId;
  GLuint mTextureT_edges_listId;

  /* Sampler objects */

  GLuint mSampleT_curId;
  GLuint mSampleT_nextId;
  GLuint mSampleT_nodesId;
  GLuint mSampleT_edges_listId;

  GLenum mMinFilter;
  GLenum mMagFilter;

  GLuint mFrameBufferId;

  // a shader program
  Shader mProgram;

  QPoint mLastMousePos;
  QTimer* mTimer;

protected:

  /** This method is automatically called by Qt once the GL context has been created.
    * It is called only once per execution */
  virtual void initializeGL();

  /** This method is automatically called by Qt everytime the opengl windows is resized.
    * \param width the new width of the windows (in pixels)
    * \param height the new height of the windows (in pixels)
    *
    * This function must never be called directly. To redraw the windows, emit the updateGL() signal:
      \code
      emit updateGL();
      \endcode
    */
  virtual void resizeGL(int width, int height);

  /** This method is automatically called by Qt everytime the opengl windows has to be refreshed. */
  virtual void paintGL();

  /** This method is automatically called by Qt everytime a key is pressed */
  void keyPressEvent(QKeyEvent * e);

  void mousePressEvent(QMouseEvent * e);
  void mouseReleaseEvent(QMouseEvent * e);
  void mouseMoveEvent(QMouseEvent * e);
  void wheelEvent(QWheelEvent * e);

  /** Internal function to setup the 3D scene */
  virtual void createScene();

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  // default constructor
  RenderingWidget();
  ~RenderingWidget();
};

#endif // RENDERINGWIDGET_H

