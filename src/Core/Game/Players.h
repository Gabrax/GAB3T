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
        : model(std::move(model)), position(position), explode(false) {}

    void Render() const {
        model->Render(position, glm::vec3(0.0f), glm::vec3(1.0f), explode);
    }

    void SetExplosion(bool shouldExplode) {
        explode = shouldExplode;
    }

    glm::vec3& GetPosition() {
        return position;
    }

private:
    std::shared_ptr<StaticModel> model;
    glm::vec3 position;

    bool explode;               // Explosion state
};

struct Cross {
    Cross(std::shared_ptr<StaticModel> model, const glm::vec3& position)
        : model(std::move(model)), position(position), explode(false) {}

    void Render() const {
        model->Render(position, glm::vec3(0.0f), glm::vec3(1.0f), explode);
    }

    void SetExplosion(bool shouldExplode, float time = 0.0f) {
        explode = shouldExplode;
    }

    glm::vec3& GetPosition() {
        return position;
    }

private:
    std::shared_ptr<StaticModel> model;
    glm::vec3 position;

    bool explode;               // Explosion state
};
