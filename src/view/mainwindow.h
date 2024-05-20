#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMainWindow>

#include "openglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(s21::Controller* controller, QWidget *parent = nullptr);
  ~MainWindow();
  int open_file = 0;  // База, при которой очистка не требуется

 signals:
  void fileSelected(const QString &filePath);

 private slots:
  void on_selectionFile_clicked();
  void onColorButtonFoneClicked();
  void onColorButtonLineClicked();
  void onColorButtonVerClicked();
  void transferVerticesFacets(int count_vertex, int count_facets);
  void transferFileIncorrect(QString errorMessage);
  void scaleModelFromSpinBox(double scaleFactor);
  void intervalLines(double interval_value);
  void thicknessLines(double thickness_value);
  void sizeVer(double size_ver);
  void onLineRadioButtonClicked(bool isCheckedLine);
  void onVerRadioButtonClicked(bool isCheckedVer);
  void onSaveBMPButtonClicked();
  void onSaveJPEGButtonClicked();
  void translateProjectionType(bool isCheckProjection);
  void clearAllFunc();


 private:
  Ui::MainWindow *ui;
  s21::Controller* controller_;
  OpenGLWidget *glWidget;
  QString filePath;

 public slots:
  void onPushButtonRotateClicked();
  void onPushButtonMoveClicked();
};
#endif  // MAINWINDOW_H
