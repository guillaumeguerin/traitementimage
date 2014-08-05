// Pestova Valentina UFR

#include <locale>
#include <RenderingWidget.h>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  std::locale::global(std::locale("C"));

  QGLFormat gl_profile(QGL::DoubleBuffer | QGL::Rgba | QGL::AlphaChannel);
  gl_profile.setVersion(3, 3);
  gl_profile.setProfile(QGLFormat::CoreProfile);
  QGLFormat::setDefaultFormat(gl_profile);

  RenderingWidget simple_gl;
  simple_gl.resize(600,500);
  simple_gl.show();
  return app.exec();
}
