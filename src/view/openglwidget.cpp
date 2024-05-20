#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(s21::Controller* controller, QWidget *parent)
    : QOpenGLWidget(parent), controller(controller),
      fileLoaded(false),
      scale(1.0),
      pointColor(Qt::red),
      lineColor(Qt::gray) {
  setFixedSize(winWidth, winHeight);
}

void OpenGLWidget::initializeGL() { initializeOpenGLFunctions(); }

void OpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWidget::paintGL() {
    if (!fileLoaded) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистить буферы
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (useDottedLine) {
            glEnable(GL_LINE_STIPPLE);
          } else {
            glDisable(GL_LINE_STIPPLE);
          }
          if (useDottedVer == 1) {
            glEnable(GL_POINT_SMOOTH);
          } else {
            glDisable(GL_POINT_SMOOTH);
          }
          if (useDottedVer == 2) {
            glEnable(GL_TEXTURE_2D);
          } else {
            glDisable(GL_TEXTURE_2D);
          }
          if (useDottedVer != 0) {
              glColor3f(pointColor.redF(), pointColor.greenF(), pointColor.blueF());
              glBegin(GL_POINTS);
              const auto& matrix_3d = controller->GetMatrix3D();
              for (int i = 1; i < matrix_3d.size(); ++i) {
                  glVertex3dv(matrix_3d[i].data());
              }
              glEnd();
          }

    glColor3f(lineColor.redF(), lineColor.greenF(), lineColor.blueF());
    glBegin(GL_LINES);
    for (const auto& facet : controller->GetPolygons()) {
        for (size_t i = 0; i < facet.vertices.size(); ++i) {
            int currentVertexIndex = facet.vertices[i];
            int nextVertexIndex = facet.vertices[(i + 1) % facet.vertices.size()];
            glVertex3dv(controller->GetMatrix3D()[currentVertexIndex].data());
            glVertex3dv(controller->GetMatrix3D()[nextVertexIndex].data());
        }
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    emit countVertexFacets(controller->GetVertexCount(), controller->GetFacetCount());
}

void OpenGLWidget::loadModelFile(const QString &filePath) {
    try {
        if (fileLoaded) {
                controller->ClearModelData();
                update();
            }
            controller->LoadModel(filePath.toStdString());
            centerModel();
            scaleModelToFit(1.0);
            fileLoaded = true;
            update();
    } catch (const std::exception& e) {
        emit fileIncorrect("File incorrect");
        emit countVertexFacets(0, 0);
    }
}

void OpenGLWidget::setProjectionType(int value) {
  makeCurrent();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (value == 0) {
    // Центральная
    float fov = 60 * M_PI / 180;
    float heapHeight = winHeight / (2 * tan(fov / 2));
    glFrustum(-winWidth / 12, winWidth / 12, -winHeight / 12, winHeight / 12,
              heapHeight, 2);
  } else if (value == 1) {
    // Параллельная
    glOrtho(-winWidth / winWidth, winWidth / winWidth, -winHeight / winHeight,
            winHeight / winHeight, -100, 100);
  }
  glTranslatef(0.0, 0.0, -10.0);
  glMatrixMode(GL_MODELVIEW);
  doneCurrent();
  update();
}

void OpenGLWidget::centerModel() {
    const auto& vertices = controller->GetMatrix3D();
       if (vertices.empty()) {
           return;
       }

       double centerX = 0.0;
       double centerY = 0.0;
       double centerZ = 0.0;

       for (const auto& vertex : vertices) {
           centerX += vertex[0];
           centerY += vertex[1];
           centerZ += vertex[2];
       }

       int vertexCount = controller->GetVertexCount();
       centerX /= vertexCount;
       centerY /= vertexCount;
       centerZ /= vertexCount;

       controller->MoveModel(-centerX, 'x');
       controller->MoveModel(-centerY, 'y');
       controller->MoveModel(-centerZ, 'z');
//    controller->MoveModel(-controller->GetVertexCount() / 2.0, 'x');
//    controller->MoveModel(-controller->GetVertexCount() / 2.0, 'y');
//    controller->MoveModel(-controller->GetVertexCount() / 2.0, 'z');
}

void OpenGLWidget::scaleModelToFit(double scaleFactor) {
    makeCurrent();

    const auto& vertices = controller->GetMatrix3D();
    if (vertices.empty()) {
        return;
    }

    double maxDistance = 0.0;
    for (const auto& vertex : vertices) {
        double distance = sqrt(pow(vertex[0], 2) + pow(vertex[1], 2) + pow(vertex[2], 2));
        if (distance > maxDistance) {
            maxDistance = distance;
        }
    }

    if (maxDistance > 0.0) {
        double scale = scaleFactor / maxDistance;
        for (auto& vertex : const_cast<std::vector<std::vector<double>>&>(vertices)) {
            vertex[0] *= scale;
            vertex[1] *= scale;
            vertex[2] *= scale;
        }

    }

    doneCurrent();
    update();
}


void OpenGLWidget::editIntervalLines(double interval_value) {
  makeCurrent();
  glLineStipple(interval_value, 0x0F0F);
  doneCurrent();
  update();
}

void OpenGLWidget::editThicknessLines(double thickness_value) {
  makeCurrent();
  glLineWidth(thickness_value);
  doneCurrent();
  update();
}

void OpenGLWidget::setLineStyle(bool line) {
  makeCurrent();
  useDottedLine = line;
  doneCurrent();
  update();
}

void OpenGLWidget::verStyle(int dottedVer) {
  makeCurrent();
  useDottedVer = dottedVer;
  doneCurrent();
  update();
}

void OpenGLWidget::editSizeVer(double size_ver) {
  makeCurrent();
  glPointSize(size_ver);
  doneCurrent();
  update();
}

void OpenGLWidget::setBackgroundColor(const QColor &color) {
  makeCurrent();
  glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  doneCurrent();
  update();
}

void OpenGLWidget::setColorLineVer(const QColor &color, bool type) {
  if (type) {
    lineColor = color;
  } else {
    pointColor = color;
  }
  update();
}

void OpenGLWidget::rotateModel(double step, char xyz) {
  makeCurrent();
  controller->RotateModel(step, xyz);
  controller->ApplyRotation();
  doneCurrent();
  update();
}

void OpenGLWidget::moveModel(double step, char xyz) {
  makeCurrent();
  controller->MoveModel(step, xyz);
  doneCurrent();
  update();
}

void OpenGLWidget::saveImage(const QString &filename) {
  QImage image = grabFramebuffer();
  image.save(filename);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
  lastMousePos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastMousePos.x();
  int dy = event->y() - lastMousePos.y();
  rotateModel(dy * rotationSpeed, 'x');
  rotateModel(dx * rotationSpeed, 'y');
  lastMousePos = event->pos();
  update();
}

void OpenGLWidget::clearContent() {
    fileLoaded = false;
    controller->ClearModelData();
    update();
}
