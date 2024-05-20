#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(s21::Controller* controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller_(controller) {
  ui->setupUi(this);

    glWidget = new OpenGLWidget(controller_, ui->display);


  connect(this, SIGNAL(fileSelected(QString)), glWidget,
          SLOT(loadModelFile(QString)));

  // Для окрашивания
  connect(ui->colorVer, SIGNAL(clicked()), this,
          SLOT(onColorButtonVerClicked()));
  connect(ui->colorFon, SIGNAL(clicked()), this,
          SLOT(onColorButtonFoneClicked()));
  connect(ui->colorLine, SIGNAL(clicked()), this,
          SLOT(onColorButtonLineClicked()));

  //Для получения значений SpinBox
  connect(ui->doubleSpinBox_scal,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::scaleModelFromSpinBox);
  connect(ui->doubleSpinBox_interval,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::intervalLines);
  connect(ui->doubleSpinBox_thickness,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::thicknessLines);
  connect(ui->doubleSpinBox_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::sizeVer);

  // Для допки с ребрами
  connect(ui->dotted_line, SIGNAL(toggled(bool)), this,
          SLOT(onLineRadioButtonClicked(bool)));
  connect(ui->solid_line, SIGNAL(toggled(bool)), this,
          SLOT(onLineRadioButtonClicked(bool)));

  // Для допки с вершинами
  connect(ui->Circle, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));
  connect(ui->Square, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));
  connect(ui->No_ver, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));

  // Для отображения вершин, граней и некоректного файла
  connect(glWidget, &OpenGLWidget::countVertexFacets, this,
          &MainWindow::transferVerticesFacets);
  connect(glWidget, &OpenGLWidget::fileIncorrect, this,
          &MainWindow::transferFileIncorrect);

  // Для допки сохранения в форматах
  connect(ui->pushButton_bmp, SIGNAL(clicked()), this,
          SLOT(onSaveBMPButtonClicked()));
  connect(ui->pushButton_jpeg, SIGNAL(clicked()), this,
          SLOT(onSaveJPEGButtonClicked()));

  // Для переворота
  connect(ui->pushButton_xProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_yProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_zProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_xMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_yMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_zMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);

  // Для перемещения
  connect(ui->pushButton_xPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_yPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_zPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_xMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_yMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_zMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);

  // Для проекции
  connect(ui->typeCenter, SIGNAL(toggled(bool)), this,
          SLOT(translateProjectionType(bool)));
  connect(ui->typeParalel, SIGNAL(toggled(bool)), this,
          SLOT(translateProjectionType(bool)));

  // Очищение
  connect(ui->Clear, SIGNAL(clicked()), this, SLOT(clearAllFunc()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_selectionFile_clicked() {
  filePath = QFileDialog::getOpenFileName(this, "Выбрать файл", "",
                                          "Wavefront OBJ файлы (*.obj)");
  if (!filePath.isEmpty()) {
   if (open_file == 1 && (ui->label_file->text() != "File incorrect")) {
     glWidget->clearContent();
   }
    open_file = 1;  // Для отчистки предыдущего объекта
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists()) {
      QString fileName = fileInfo.fileName();
      ui->label_file->setText(fileName);
              emit fileSelected(filePath);
    } else {
      ui->label_file->setText("Файл не существует");
      open_file = 0;
    }
  } else {
    ui->label_file->setText("Выбор файла отменен");
    open_file = 0;
  }
}


void MainWindow::onPushButtonRotateClicked() {
  if (filePath.isEmpty()) {
    return;
  }
  double labelValue = 0;
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton) {
    if (clickedButton == ui->pushButton_xProtat) {
      labelValue = ui->lineEdit_Xrotat->text().toDouble();
      glWidget->rotateModel(labelValue, 'x');
    } else if (clickedButton == ui->pushButton_yProtat) {
      labelValue = ui->lineEdit_Yrotat->text().toDouble();
      glWidget->rotateModel(labelValue, 'y');
    } else if (clickedButton == ui->pushButton_zProtat) {
      labelValue = ui->lineEdit_Zrotat->text().toDouble();
      glWidget->rotateModel(labelValue, 'z');
    } else if (clickedButton == ui->pushButton_xMrotat) {
      labelValue = ui->lineEdit_Xrotat->text().toDouble();
      glWidget->rotateModel(labelValue, 'X');
    } else if (clickedButton == ui->pushButton_yMrotat) {
      labelValue = ui->lineEdit_Yrotat->text().toDouble();
      glWidget->rotateModel(labelValue, 'Y');
    } else if (clickedButton == ui->pushButton_zMrotat) {
      labelValue = ui->lineEdit_Zrotat->text().toDouble();
      glWidget->rotateModel(labelValue, 'Z');
    }
  }
}

void MainWindow::onPushButtonMoveClicked() {
  if (filePath.isEmpty()) {
    return;
  }
  double labelValue = 0;
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton) {
    if (clickedButton == ui->pushButton_xPmove) {
      labelValue = ui->lineEdit_xMove->text().toDouble();
      glWidget->moveModel(labelValue, 'x');
    } else if (clickedButton == ui->pushButton_yPmove) {
      labelValue = ui->lineEdit_yMove->text().toDouble();
      glWidget->moveModel(labelValue, 'y');
    } else if (clickedButton == ui->pushButton_zPmove) {
      labelValue = ui->lineEdit_zMove->text().toDouble();
      glWidget->moveModel(labelValue, 'z');
    } else if (clickedButton == ui->pushButton_xMmove) {
      labelValue = ui->lineEdit_xMove->text().toDouble();
      glWidget->moveModel(labelValue, 'X');
    } else if (clickedButton == ui->pushButton_yMmove) {
      labelValue = ui->lineEdit_yMove->text().toDouble();
      glWidget->moveModel(labelValue, 'Y');
    } else if (clickedButton == ui->pushButton_zMmove) {
      labelValue = ui->lineEdit_zMove->text().toDouble();
      glWidget->moveModel(labelValue, 'Z');
    }
  }
}

void MainWindow::onColorButtonFoneClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
      glWidget->setBackgroundColor(color);
      ui->colorFon_2->setStyleSheet("background-color: " + color.name() +
                                    ";border-radius: 10px");
    }
  }
}

void MainWindow::onColorButtonLineClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
      glWidget->setColorLineVer(color, true);
      ui->colorLine_2->setStyleSheet("background-color: " + color.name() +
                                     ";border-radius: 10px");
    }
  }
}

void MainWindow::onColorButtonVerClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
      glWidget->setColorLineVer(color, false);
      ui->colorVer_2->setStyleSheet("background-color: " + color.name() +
                                    ";border-radius: 10px");
    }
  }
}

void MainWindow::transferVerticesFacets(int count_vertex, int count_facets) {
  ui->label_top->setText(QString::number(count_vertex));
  ui->label_ribs->setText(QString::number(count_facets));
}

void MainWindow::transferFileIncorrect(QString errorMessage) {
  ui->label_file->setText(errorMessage);
}

void MainWindow::scaleModelFromSpinBox(double scaleFactor) {
  glWidget->scaleModelToFit(scaleFactor);
  update();
}

void MainWindow::intervalLines(double interval_value) {
  glWidget->editIntervalLines(interval_value);
  update();
}

void MainWindow::thicknessLines(double thickness_value) {
  glWidget->editThicknessLines(thickness_value);
  update();
}

void MainWindow::sizeVer(double size_ver) {
  glWidget->editSizeVer(size_ver);
  update();
}

void MainWindow::onLineRadioButtonClicked(bool isCheckedLine) {
  if (isCheckedLine) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->dotted_line) {
      glWidget->setLineStyle(true);
    } else if (selectedRadioButton == ui->solid_line) {
      glWidget->setLineStyle(false);
    }
    update();
  }
}

void MainWindow::onVerRadioButtonClicked(bool isCheckedVer) {
  if (isCheckedVer) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->Circle) {
      glWidget->verStyle(1);
    } else if (selectedRadioButton == ui->Square) {
      glWidget->verStyle(2);
    } else if (selectedRadioButton == ui->No_ver) {
      glWidget->verStyle(0);
    }
    update();
  }
}

void MainWindow::onSaveBMPButtonClicked() {
  QString filePath =
      QFileDialog::getSaveFileName(this, "Save Image", "", "Bitmap (*.bmp)");
  if (!filePath.isEmpty()) {
    glWidget->saveImage(filePath);
  }
}

void MainWindow::onSaveJPEGButtonClicked() {
  QString filePath =
      QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG (*.jpeg)");
  if (!filePath.isEmpty()) {
    glWidget->saveImage(filePath);
  }
}

void MainWindow::translateProjectionType(bool isCheckedProjection) {
  if (isCheckedProjection) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->typeCenter) {
      glWidget->setProjectionType(0);
    } else if (selectedRadioButton == ui->typeParalel) {
      glWidget->setProjectionType(1);
    }
    update();
  }
}

void MainWindow::clearAllFunc() {
  if (open_file != 2) {
    if (!filePath.isEmpty() && open_file == 1 &&
        (ui->label_file->text() != "File incorrect")) {
      glWidget->clearContent();
      open_file = 0;
    }
    ui->label_file->setText("");

    ui->lineEdit_Xrotat->setText("0.0175");
    ui->lineEdit_Yrotat->setText("0.0175");
    ui->lineEdit_Zrotat->setText("0.0175");

    ui->lineEdit_xMove->setText("0.25");
    ui->lineEdit_yMove->setText("0.25");
    ui->lineEdit_zMove->setText("0.25");

    ui->doubleSpinBox_interval->setValue(1.00);
    ui->doubleSpinBox_scal->setValue(1.00);
    ui->doubleSpinBox_size->setValue(1.00);
    ui->doubleSpinBox_thickness->setValue(1.00);

    ui->colorFon_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px");
    ui->colorLine_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px; border: "
        "2px solid #8f8f91;");
    ui->colorVer_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px; border: "
        "2px solid #8f8f91;");

    glWidget->setBackgroundColor(Qt::black);
    glWidget->setColorLineVer(Qt::red, false);
    glWidget->setColorLineVer(Qt::gray, true);

    open_file = 2;  // Не дает использоваать очистку дважды

    ui->label_ribs->setText("");
    ui->label_top->setText("");

    glWidget->update();
  }
}
