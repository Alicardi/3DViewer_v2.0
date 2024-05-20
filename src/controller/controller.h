#ifndef S21_CONTROLLER_H
#define S21_CONTROLLER_H

#include "../model/model.h"
#include <string>

namespace s21 {

class Controller {
public:
    static Controller& getInstance(Model* model = nullptr) {
            static Controller instance(model);
            return instance;
        }

    void LoadModel(const std::string& file_path);
    void RotateModel(double step, char xyz);
    void ApplyRotation();
    void MoveModel(double distance, char xyz);
    void ClearModelData();

    int GetVertexCount() const { return model_->GetVertexCount(); }
    int GetFacetCount() const { return model_->GetFacetCount(); }
    const std::vector<std::vector<double>>& GetMatrix3D() const { return model_->GetMatrix3D(); }
    const std::vector<Facet>& GetPolygons() const { return model_->GetPolygons(); }

private:
    Controller(Model* model) : model_(model) {}
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    Model* model_;
};

}  // namespace s21

#endif  // S21_CONTROLLER_H
