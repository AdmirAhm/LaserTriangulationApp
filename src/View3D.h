#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "Program2.h"
#include <gui/gl/CommandList.h>
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>

class View3D : public gui::gl::View
{
    float tar1 = 0.0f;
    float tar2 = 0.0f;
    using Base = gui::gl::View;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;
    glm::vec3 campos = glm::vec3(3.0, 3.0, 3.0f);
    double distance;
    std::vector<std::vector<cv::Point3f> >_pts;

    gui::gl::Buffer _gpuBuffer;
    
    Program2 _program;
    gui::gl::Command* pMtxSetterCmd = nullptr;
    
    float _angleX = 0;
    float _angleY = 0;
    float _dAngle = 0.01f;

private:
    double getDistance(cv::Point3f a, cv::Point3f b) {
        return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }
    bool setupShaders()
    {
        if (!_program.link(":shVert3D", ":shFrag3D"))
            return false;
        return true;
    }
    
    //setup data and send it to GPU, create command list
    void setup()
    {
        double mx = -10000, mn = 10000;
        int sz = 0;
        for (auto p : _pts) {
            for (auto x : p) {
                sz++;
                if (x.z < mn) mn = x.z;
                if (x.z > mx) mx = x.z;
            }
        }
        float* vertices = new float[6 * sz];
        int ind = 0;
        int ind2 = 0;
        for (auto p : _pts) {
            std::sort(p.begin(), p.end(), [](cv::Point3f a, cv::Point3f b) {return a.x < b.x; });
            auto a = 0.5f * getDistance(_pts[0][0], _pts[0][_pts[0].size() - 1]);
            for (int i = 0; i < p.size(); i++) {
                vertices[ind2 * 6] = getDistance(_pts[0][0], p[i])/a;
                vertices[ind2 * 6 + 2] = distance * ind/a;
                vertices[ind2 * 6 + 1] = p[i].z / a;
                vertices[ind2 * 6 + 3] = 1.0f * (p[i].z - mn) / (mx - mn);
                vertices[ind2 * 6 + 4] = 1.0f- 1.0f * (p[i].z - mn) / (mx - mn);
                vertices[ind2 * 6 + 5] = 0.0f;
                ind2++;
            }
            ind++;
        }
        //encode command to set transformation matrix (uniform)
        pMtxSetterCmd = _gpuBuffer.createCommand();
        pMtxSetterCmd->createMVPSetter(&_mvpMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);
        
        // Define the cube's vertices and texture coordinates
        
        
        _gpuBuffer.appendVertices(vertices, sz);
        
        
        //encode command to draw tetured cube
        auto cmd = _gpuBuffer.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::Points, 0, sz);
        _gpuBuffer.encode(cmd);

        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        _program.setBuffer(&_gpuBuffer);
    }
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setup();
        
        
        // Set up the perspective parameters
        float fov = 45.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane
        float farClip = 100.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        glm::vec3 cameraPosition = glm::vec3(3.0, 3.0, 3.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(tar1, 0.0f, tar2);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model
        
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
        
        dbgCheckGLError();
    }
    
    bool prepareNextFrame() override
    {
        _angleX += _dAngle;
//        _angleY += _dAngle/2;
        // Create a model matrix with rotations around X and Y axes
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        //modelMatrix = glm::rotate(modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        //modelMatrix = glm::rotate(modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis

        // Combine the perspective, view, and model matrices to get the final MVP matrix
        glm::vec3 cameraPosition = campos;  // New camera position
        glm::vec3 cameraTarget = glm::vec3(tar1, 0.0f, tar2);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

        _mvpMatrix = _perspectiveMatrix * _viewMatrix * modelMatrix;

        return true;
//        reDraw();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        // Clear
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});

        _program.activate();
        _program.execute();

        // Unbind program
        _program.deActivate();
    }

    bool onKeyPressed(const gui::Key& key) override {
        if (key.getChar() == 'a') {
            tar1 -= 0.02f;
        }
        if (key.getChar() == 'd') {
            tar1 += 0.02f;
        }
        if (key.getChar() == 'w') {
            tar2 -= 0.02f;
        }
        if (key.getChar() == 's') {
            tar2 += 0.02f;
        }
        if (key.getChar() == 'q') {
            glm::vec3 cameraTarget = glm::vec3(tar1, 0.0f, tar2);
            auto dif = cameraTarget - campos;
            glm::normalize(dif);
            dif *= 0.1;
            campos += dif;
        }
        if (key.getChar() == 'e') {
            glm::vec3 cameraTarget = glm::vec3(tar1, 0.0f, tar2);
            auto dif = cameraTarget - campos;
            glm::normalize(dif);
            dif *= 0.1;
            campos -= dif;
        }
       
        reDraw();
        return true;
    }

public:
    View3D(double dist, std::vector<std::vector<cv::Point3f> >pts)
    {
        distance = dist;
        _pts = pts;
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 100, 100, {gui::gl::DataType::vec3, gui::gl::DataType::vec3});
        
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~View3D()
    {
        makeCurrentContext();
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
    }
    
};
