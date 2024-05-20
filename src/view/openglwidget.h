#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QImageWriter>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../controller/controller.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OpenGLWidget(s21::Controller* controller, QWidget *parent = nullptr);
  void rotateModel(double step, char xyz);
  void moveModel(double step, char xyz);
  void editIntervalLines(double scaleFactor);
  void editThicknessLines(double scaleFactor);
  void editSizeVer(double size_ver);
  void clearContent();
  void setProjectionType(int value);

 public slots:
  void loadModelFile(const QString &filePath);
  void centerModel();
  void scaleModelToFit(double scaleFactor);
  void setBackgroundColor(const QColor &color);
  void setColorLineVer(const QColor &color, bool type);
  void saveImage(const QString &filePath);
  void verStyle(int dottedLine);
  void setLineStyle(bool line);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

 private:
  s21::Controller* controller;
  bool fileLoaded;
  float scale;
  QColor pointColor;
  QColor lineColor;
  QPoint lastMousePos;
  double rotationSpeed = 0.01;
  bool useDottedLine;
  int useDottedVer = 1;
  int winHeight = 540, winWidth = 650;

 signals:
  void countVertexFacets(int count_vertex, int count_facets);
  void fileIncorrect(QString errorMessage);
};

#endif  // OPENGLWIDGET_H
