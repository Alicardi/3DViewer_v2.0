#include "controller.h"
#include <stdexcept>

namespace s21 {

void Controller::LoadModel(const std::string& file_path) {
    model_->CountVerticesAndFacets(file_path);
    model_->ParseModelData(file_path);
}

void Controller::RotateModel(double step, char xyz) {
    model_->RotateModel(step, xyz);
}

void Controller::ApplyRotation() {
    model_->ApplyRotation();
}

void Controller::MoveModel(double distance, char xyz) {
    model_->MoveModel(distance, xyz);
}

void Controller::ClearModelData() {
    model_->ClearData();
}

}  // namespace s21
