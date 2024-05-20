#include "model.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <iostream>

namespace s21 {

Model::Model() : count_of_vertices(0), count_of_facets(0), rotation_x(0.0), rotation_y(0.0), rotation_z(0.0) {}

Model::~Model() {
  ClearData();
}

void Model::CountVerticesAndFacets(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::string line;
    count_of_vertices = 0;
    count_of_facets = 0;

    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            count_of_vertices++;
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream iss(line.substr(2));
            int count_vertices_in_facets = 0;
            std::string token;
            while (iss >> token) {
                count_vertices_in_facets++;
            }
            if (count_vertices_in_facets == 2) {
                count_of_facets += count_vertices_in_facets - 1;
            } else if (count_vertices_in_facets > 2) {
                count_of_facets += count_vertices_in_facets - 2;
            }
        }
    }
}

void Model::ParseModelData(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл");
    }
    std::string line;
    int vertex_index = 1, facet_index = 1, count_vertex = 0;
    double x, y, z = 0;

    matrix_3d.resize(count_of_vertices + 1, std::vector<double>(3, 0.0));
    polygons.resize(count_of_facets + 1);

    // Вывод координат после инициализации
    std::cout << "Инициализированные координаты точек:\n";
    for (int i = 0; i <= count_of_vertices; ++i) {
        std::cout << "Точка " << i << ": (" << matrix_3d[i][0] << ", " << matrix_3d[i][1] << ", " << matrix_3d[i][2] << ")\n";
    }

    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream iss(line.substr(2));
            std::vector<double> coords;
            double coord;
            while (iss >> coord) {
                coords.push_back(coord);
            }
            if (coords.size() != 3) { // Проверка, что координат ровно три
                throw std::runtime_error("Каждая вершина должна иметь ровно 3 координаты");
            }
            x = coords[0];
            y = coords[1];
            z = coords[2];
            matrix_3d[vertex_index][0] = x;
            matrix_3d[vertex_index][1] = y;
            matrix_3d[vertex_index][2] = z;
            vertex_index++;
            count_vertex++;
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream iss(line.substr(2));
            std::string token;
            int count_vertex_in_facets = 0;
            while (iss >> token) {
                int current_vertex_index = std::stoi(token);
                if (current_vertex_index < 0) {
                    current_vertex_index = count_vertex + 1 + current_vertex_index;
                }
                if (current_vertex_index == 0 || current_vertex_index > vertex_index) {
                    throw std::runtime_error("Недопустимый индекс вершины");
                } else {
                    polygons[facet_index].vertices.push_back(current_vertex_index);
                    count_vertex_in_facets++;
                }
            }
            polygons[facet_index].count_vertices_in_facets = count_vertex_in_facets;
            if (count_vertex_in_facets > 1) {
                facet_index++;
            }
        }
    }

    // Вывод всех инициализированных точек после обработки файла
    std::cout << "Инициализированные координаты точек после обработки файла:\n";
    for (int i = 1; i <= count_of_vertices; ++i) {
        std::cout << "Точка " << i << ": (" << matrix_3d[i][0] << ", " << matrix_3d[i][1] << ", " << matrix_3d[i][2] << ")\n";
    }
}

void Model::RotateModel(double step, char xyz) {
    switch (xyz) {
        case 'x':
            rotation_x = step;
            break;
        case 'y':
            rotation_y = step;
            break;
        case 'z':
            rotation_z = step;
            break;
        case 'X':
            rotation_x = -step;
            break;
        case 'Y':
            rotation_y = -step;
            break;
        case 'Z':
            rotation_z = -step;
            break;
        default:
            break;
    }
}

void Model::RotatePoint(std::vector<double>& point, double angle, char xyz) {
    double cos_result = cos(angle);
    double sin_result = sin(angle);
    double temp = 0;
    switch (xyz) {
        case 'x':
            temp = point[1] * cos_result - point[2] * sin_result;
            point[2] = point[1] * sin_result + point[2] * cos_result;
            point[1] = temp;
            break;
        case 'y':
            temp = point[0] * cos_result + point[2] * sin_result;
            point[2] = -point[0] * sin_result + point[2] * cos_result;
            point[0] = temp;
            break;
        case 'z':
            temp = point[0] * cos_result - point[1] * sin_result;
            point[1] = point[0] * sin_result + point[1] * cos_result;
            point[0] = temp;
            break;
    }
}

void Model::ApplyRotation() {
    for (int i = 1; i <= count_of_vertices; i++) {
        RotatePoint(matrix_3d[i], rotation_x, 'x');
        RotatePoint(matrix_3d[i], rotation_y, 'y');
        RotatePoint(matrix_3d[i], rotation_z, 'z');
    }
    rotation_x = 0.0;
    rotation_y = 0.0;
    rotation_z = 0.0;
}

void Model::MoveModel(double distance, char xyz) {
    switch (xyz) {
        case 'x':
            for (int i = 1; i <= count_of_vertices; i++) {
                matrix_3d[i][0] += distance;
            }
            break;
        case 'y':
            for (int i = 1; i <= count_of_vertices; i++) {
                matrix_3d[i][1] += distance;
            }
            break;
        case 'z':
            for (int i = 1; i <= count_of_vertices; i++) {
                matrix_3d[i][2] += distance;
            }
            break;
        case 'X':
            for (int i = 1; i <= count_of_vertices; i++) {
                matrix_3d[i][0] -= distance;
            }
            break;
        case 'Y':
            for (int i = 1; i <= count_of_vertices; i++) {
                matrix_3d[i][1] -= distance;
            }
            break;
        case 'Z':
            for (int i = 1; i <= count_of_vertices; i++) {
                matrix_3d[i][2] -= distance;
            }
            break;
        default:
            break;
    }
}

void Model::ClearData() {
    matrix_3d.clear();
    polygons.clear();
    count_of_vertices = 0;
    count_of_facets = 0;
    rotation_x = 0.0;
    rotation_y = 0.0;
    rotation_z = 0.0;
}

}  // namespace s21
