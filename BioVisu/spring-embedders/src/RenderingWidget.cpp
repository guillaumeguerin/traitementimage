// Pestova Valentina UFR

#include "OpenGL.h"
#include <RenderingWidget.h>
#include <Eigen/Geometry>
#include <iostream>
#include <QKeyEvent>
#include <QImage>

using namespace Eigen;


bool checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        std::cout << "Framebuffer complete." << std::endl;
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;
/*
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;
*/
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
        return false;
    }
}


RenderingWidget::RenderingWidget()
  :
#ifdef __APPLE__
  QGLWidget(new Core3_2_context(QGLFormat::defaultFormat())),
#endif
    mCamera(),
    mCamPhy(.5),
    mCamTheta(.2),
    mCamDist(1.5),
    mCamLookAt(Vector3f::Zero()),
    mCamFov(M_PI_2),
    mMinFilter(GL_LINEAR_MIPMAP_LINEAR),
    mMagFilter(GL_LINEAR),
    mFrameBufferId(0),
    mProgram(),
    mLastMousePos(),
    mTimer(new QTimer(this))
{
}

RenderingWidget::~RenderingWidget()
{
}

GLfloat Eltime = 0;

void RenderingWidget::paintGL()
{
  GL_TEST_ERR;

   glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);GL_TEST_ERR;
   glBindFragDataLocation(mProgram.id(), 0, "out_color");GL_TEST_ERR;
   glViewport(0, 0, 512, 512);
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Mise à jour de la caméra
  Vector3f cameraPos = mCamLookAt + mCamDist * Vector3f(
  std::sin(mCamPhy)*std::cos(mCamTheta),
  std::sin(mCamTheta),
  std::cos(mCamPhy)*std::cos(mCamTheta));
  mCamera.lookAt(cameraPos, mCamLookAt, Vector3f::UnitY());

  glSamplerParameteri(mSampleT_curId, GL_TEXTURE_MIN_FILTER, mMinFilter);GL_TEST_ERR;
  glSamplerParameteri(mSampleT_curId, GL_TEXTURE_MAG_FILTER, mMagFilter);GL_TEST_ERR;
  glBindSampler(0, mSampleT_curId);GL_TEST_ERR;

  /* lier une texture pour Tcur (pas affichage, mais les calcules) */
  mProgram.activate();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTextureT_curId);
  glUniform1i(mProgram.getUniformLocation("Tcur"), 0);

  glSamplerParameteri(mSampleT_nextId, GL_TEXTURE_MIN_FILTER, mMinFilter);GL_TEST_ERR;
  glSamplerParameteri(mSampleT_nextId, GL_TEXTURE_MAG_FILTER, mMagFilter);GL_TEST_ERR;
  glBindSampler(0, mSampleT_nextId);GL_TEST_ERR;

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mTextureT_nextId);
  glUniform1i(mProgram.getUniformLocation("Tcur"), 1);

  glSamplerParameteri(mSampleT_nodesId, GL_TEXTURE_MIN_FILTER, mMinFilter);GL_TEST_ERR;
  glSamplerParameteri(mSampleT_nodesId, GL_TEXTURE_MAG_FILTER, mMagFilter);GL_TEST_ERR;
  glBindSampler(0, mSampleT_nodesId);GL_TEST_ERR;

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, mTextureT_nodesId);
  glUniform1i(mProgram.getUniformLocation("Tnodes"), 2);

  glSamplerParameteri(mSampleT_edges_listId, GL_TEXTURE_MIN_FILTER, mMinFilter);GL_TEST_ERR;
  glSamplerParameteri(mSampleT_edges_listId, GL_TEXTURE_MAG_FILTER, mMagFilter);GL_TEST_ERR;
  glBindSampler(0, mSampleT_edges_listId);GL_TEST_ERR;

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, mTextureT_edges_listId);
  glUniform1i(mProgram.getUniformLocation("TedgesList"), 3);

  GLint vertTexSize = glGetUniformLocation(mProgram.id(), "verticeTexturesSize");
  glUniform1i(vertTexSize, graph->getVertMatrixDim());

  GLint neighboursTexSize = glGetUniformLocation(mProgram.id(), "neighboursTexturesSize");
  glUniform1i(neighboursTexSize, graph->getNeighboursMatrixDim());


  // Demonstration: on fait tourner le graph
  Eltime+=0.001;

  GLfloat t = glGetUniformLocation(mProgram.id(), "Eltime");
  glUniform1f(t, Eltime);

  GL_TEST_ERR;

  graph->draw(mCamera, mProgram);

  /* Render to texture ! */

  glBindFramebuffer(GL_FRAMEBUFFER, 0);GL_TEST_ERR;
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, mTextureT_nextId);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBufferId);
  glReadBuffer( GL_COLOR_ATTACHMENT0 );
  glReadPixels(256, 256, 512, 512,GL_RGBA, GL_FLOAT, nextTex);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);GL_TEST_ERR;


  int j = 0;

  for (int i = 0; i<512*512*4; i+=4){

      // on a lu des donnees depuis fbo et on verifie qu'il n'y a pas des pixel de la couleur de fond
      if(nextTex[i]!=0 || nextTex[i+1]!=0 || nextTex[i+2]!=0 ){
          newPositions[j] = nextTex[i];
          newPositions[j + 1] = nextTex[i + 1];
          newPositions[j + 2] = nextTex[i + 2];
          newPositions[j + 3] = nextTex[i + 3];
          j+=4;
      }
  }

  for (int j = 0; j<sizeof(newPositions); j+=4){

     std::cout<<newPositions[j]<<" "<<newPositions[j+1]<<" "<<newPositions[j+2]<<" "<<newPositions[j+3]<<std::endl;

  }


  graph->updateVertices(newPositions);
  graph->draw(mCamera, mProgram);
}

void RenderingWidget::initializeGL()
{
  std::cout << "Using OpenGL version: \"" << glGetString(GL_VERSION) << "\"" << std::endl;
  std::cout << "OpenGL context: " << context()->format().majorVersion()
            << "." << context()->format().minorVersion()
            << " " << ((context()->format().profile() == QGLFormat::CoreProfile)? "Core":
                       (context()->format().profile() == QGLFormat::CompatibilityProfile)? "Compatibility":
                       "No profile")
            << "\n";

  glClearColor(0.,0.,0.,0.);

  glEnable(GL_DEPTH_TEST);

  createScene();

  // Start a timer that redraw the scene every 16ms (around 60 fps).
  connect(mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
  mTimer->start(16);
}

void RenderingWidget::resizeGL(int width, int height)
{
  // configure the rendering target size (viewport)
  glViewport(0, 0, width, height);

  mCamera.setPerspective(mCamFov, float(width)/float(height), .01, 10., 2.);
}

void RenderingWidget::createScene()
{
  GL_TEST_ERR;

  bool ok;

  // load the default shaders
  std::cout << "Loading shaders compute.*...\n";
  ok = mProgram.loadFromFiles(PGHP_DIR"/shaders/compute.vert", PGHP_DIR"/shaders/computeForces.frag");


  assert(ok);

  // fill the arrays of vertices and edges
  std::vector<Vertex> vertices;
  std::vector<int> n1, n2, n3, n4;

  n1.push_back(1);
  n1.push_back(2);
  n1.push_back(3);

  n2.push_back(4);
  n2.push_back(5);
  n2.push_back(6);
  n2.push_back(7);

  vertices.push_back(Vertex(Vector3f(0.0, 0.1, 0.2), n1));
  vertices.push_back(Vertex(Vector3f(0.5, 1.1, 2.2), n2));

  n1[0]  = 5;
  n1[1]  = 9;
  n1[2]  = 8;

  n2[0]  = 0;
  n2[1]  = 9;
  n2[2]  = 4;
  n2[3]  = 5;

  vertices.push_back(Vertex(Vector3f(-0.9, 0.1, -0.2), n1));
  vertices.push_back(Vertex(Vector3f(-0.5, 1.1, -2.2), n2));

  n1[0]  = 1;
  n1[1]  = 5;
  n1[2]  = 6;

  n2[0]  = 0;
  n2[1]  = 2;
  n2[2]  = 8;
  n2[3]  = 9;

  vertices.push_back(Vertex(Vector3f(-1.9, -0.1, 2.2), n1));
  vertices.push_back(Vertex(Vector3f(0.65, -1.1, 3.2), n2));


  n3.push_back(6);
  n4.push_back(1);
  n4.push_back(5);

  vertices.push_back(Vertex(Vector3f(1.9, -1.1, 1.2), n3));
  vertices.push_back(Vertex(Vector3f(1.5, -1.1, 2.1), n4));

  n3[0] = 8;
  n4[0] = 3;
  n4[1] = 6;

  vertices.push_back(Vertex(Vector3f(1.9, 4.1, -1.2), n3));
  vertices.push_back(Vertex(Vector3f(-1.5, 5.1, -2.1), n4));

  // graph with 10 vertices
 graph = new Graph(vertices);
//graph = new Graph();
//graph->loadGraph("graphTest.asc");

    // create and fill the textures
  graph->fillArrays(mTextureT_cur, mTextureT_next, mTextureT_nodes, mTextureT_edges_list );

  // load the textures to opengl

  GL_TEST_ERR;  

  int nbVertices = graph->getNumVertices();
  int nbNeighbors = graph->getNumNeighbors();

  int verticesDim, neighboursDim;

  graph->getArrayDims(verticesDim, neighboursDim);
  newPositions = new GLfloat[graph->getVertMatrixDim()*4];
  nextTex = new GLfloat[512*512*4];
  std::cout << "verticesDim: "<<verticesDim<<", neighboursDim"<<neighboursDim<<std::endl;

  /* charger la texture dan OpenGL
    T_cur*/
  glGenTextures(1,&mTextureT_curId);
  glBindTexture(GL_TEXTURE_2D,mTextureT_curId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, verticesDim, verticesDim, 0, GL_BGRA, GL_FLOAT, &mTextureT_curId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
  glGenSamplers(1,&mSampleT_curId);

  GL_TEST_ERR;

  /*  T_nodes*/
  glGenTextures(1,&mTextureT_nodesId);
  glBindTexture(GL_TEXTURE_2D,mTextureT_nextId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, verticesDim, verticesDim, 0, GL_BGRA, GL_FLOAT,& mTextureT_nodesId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
  glGenSamplers(1,&mSampleT_nodesId);

  GL_TEST_ERR;

  /*  T_nodes*/
  glGenTextures(1,&mTextureT_edges_listId);
  glBindTexture(GL_TEXTURE_2D,mTextureT_edges_listId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, neighboursDim, neighboursDim, 0, GL_BGRA, GL_FLOAT, &mSampleT_edges_listId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
  glGenSamplers(1,&mSampleT_edges_listId);

  /*  T_next*/
  glGenTextures(1,&mTextureT_nextId);
  glBindTexture(GL_TEXTURE_2D,mTextureT_nextId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, verticesDim, verticesDim, 0, GL_BGRA, GL_FLOAT, &mTextureT_nextId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
  glGenSamplers(1,&mSampleT_nextId);

  /* TODO 2.2: Création du FBO */
  glGenFramebuffers(1, &mFrameBufferId);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBufferId);

  glBindTexture(GL_TEXTURE_2D, mTextureT_nextId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_BGRA, GL_FLOAT, NULL);

  GLuint depth_buffer_id = 0;
  glGenRenderbuffers(1, &depth_buffer_id);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 512, 512);

  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureT_nextId, 0);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id);
  GL_TEST_ERR;
}

void RenderingWidget::keyPressEvent(QKeyEvent * e)
{
  /***** ATTENTION ! Evitez les appels OpenGL dans cette fonction ! *****/

  switch(e->key())
  {
    case Qt::Key_A:
      mMagFilter = GL_LINEAR;
      break;
    case Qt::Key_Z:
      mMagFilter = GL_NEAREST;
      break;
    case Qt::Key_E:
      mMinFilter = GL_LINEAR;
      break;
    case Qt::Key_R:
      mMinFilter = GL_NEAREST;
      break;
    case Qt::Key_T:
      mMinFilter = GL_NEAREST_MIPMAP_NEAREST;
      break;
    case Qt::Key_Y:
      mMinFilter = GL_NEAREST_MIPMAP_LINEAR;
      break;
    case Qt::Key_U:
      mMinFilter = GL_LINEAR_MIPMAP_NEAREST;
      break;
    case Qt::Key_I:
      mMinFilter = GL_LINEAR_MIPMAP_LINEAR;
      break;
    case Qt::Key_Up:
      break;
    case Qt::Key_Down:
      break;
    case Qt::Key_Left:
      break;
    case Qt::Key_Right:
      break;
    default:
      break;
  }
}

void RenderingWidget::mousePressEvent(QMouseEvent * e) {
  mLastMousePos = e->pos();
  grabMouse();
  e->accept();
}

void RenderingWidget::mouseReleaseEvent(QMouseEvent * e) {
  mLastMousePos = e->pos();
  releaseMouse();
  e->accept();
}

void RenderingWidget::mouseMoveEvent(QMouseEvent * e) {
  if(e->buttons() & Qt::LeftButton) {
    mCamPhy += -float(e->x() - mLastMousePos.x()) / 256.;
    mCamTheta += float(e->y() - mLastMousePos.y()) / 256.;
    mCamTheta = std::min(M_PI_2-.001, std::max(-M_PI_2+0.001, double(mCamTheta)));
  }
  else if(e->buttons() & Qt::MiddleButton) {
    float offset = mCamDist * std::tan(mCamFov/width());
    Vector3f z = mCamera.getPosition()-mCamLookAt;
    Vector3f x = offset * (Vector3f::UnitY().cross(z)).normalized();
    Vector3f y = offset * (z.cross(x)).normalized();
    mCamLookAt += -x*(e->x() - mLastMousePos.x()) +
                   y*(e->y() - mLastMousePos.y());
  }
  mLastMousePos = e->pos();
  e->accept();
}

void RenderingWidget::wheelEvent(QWheelEvent * e) {
  mCamDist *= (e->delta()>0)? 1./1.1: 1.1;
  e->accept();
}

#include <RenderingWidget.moc>
