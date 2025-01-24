#pragma once
#include <iostream>
#include <array>
#include <vector>

#include "../Backend/Shader.h"
#include "../Backend/Util.hpp"

#include "../Backend/StaticModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

struct Circle {

    Circle(std::shared_ptr<StaticModel> model, const glm::vec3& position)
        : model(std::move(model)), position(position) {}

    void Render() const {
        model->Render(position);
    }

    glm::vec3& GetPosition() {
        return position;
    }  

private:
    std::shared_ptr<StaticModel> model;
    glm::vec3 position;
};

struct Cross {

    Cross(std::shared_ptr<StaticModel> model, const glm::vec3& position)
        : model(std::move(model)), position(position) {}

    void Render() const {
        model->Render(position);
    }

    glm::vec3& GetPosition() {
        return position;
    }

private:
    std::shared_ptr<StaticModel> model;
    glm::vec3 position;
};
