
#pragma once
#include <gui/gl/View.h>
#include <gui/gl/Buffer.h>
#include "Program.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

class View2D : public gui::gl::View
{
    Program _program;
    gui::gl::Buffer _gpuBuffer;
    std::vector<cv::Point3f>_pts;
    float _zoomFactor = 1.0f;
private:
    bool setupShaders()
    {
        if (!_program.link(":shVert", ":shFrag"))
            return false;
        return true;
    }
    //setup data and send it to GPU
    double getDistance(cv::Point3f a, cv::Point3f b) {
        return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }
    void setupVBO()
    {
        std::sort(_pts.begin(), _pts.end(), [](cv::Point3f a, cv::Point3f b) {return a.x < b.x; });


        // Vertex data for a simple rectangle with colors
        gui::gl::VertexType *vertices = new gui::gl::VertexType[5*_pts.size()];
        auto a = 0.5f*getDistance(_pts[0], _pts[_pts.size() - 1]);
        double m = 0;
        for (int i = 0; i < _pts.size(); i++) {
            vertices[i * 5] = getDistance(_pts[0], _pts[i])/a-1.0f;
            if (getDistance(_pts[0], _pts[i]) / a - 1.0f > m) m = getDistance(_pts[0], _pts[i]) / a - 1.0f;
            vertices[i * 5 + 1] = _pts[i].z/100.0f;
            vertices[i * 5 + 2] = 1.0f;
            vertices[i * 5 + 3] = 1.0f;
            vertices[i * 5 + 4] = 1.0f;
        }

        
        _gpuBuffer.appendVertices(&vertices[0], _pts.size());
        
        if (!_gpuBuffer.commit()) //transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        //create command for rect
        auto cmd = _gpuBuffer.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::Points, 0, _pts.size());
        _gpuBuffer.encode(cmd);

        _program.setBuffer(&_gpuBuffer);
        
        //check error
        dbgCheckGLError();
    }
protected:
    
    void onInit() override
    {
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({ gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth });
        
        _program.activate();
        
        _program.setZoom(_zoomFactor);
        
        _program.execute();
        
        _program.deActivate();
    }

public:
    View2D(std::vector<cv::Point3f>pts)
    {
        //buffer must be initialized before context creation (or can be initialized in setupVBO)
        //init buffer with max 4 commands, 20 vertex sets, 0-indices, [vec2 vec3] attribs
        _gpuBuffer.init(4, 20, 0, {gui::gl::DataType::vec2, gui::gl::DataType::vec3});
        
        gui::gl::Context reqContext;
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, 
            gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
        _pts = pts;
    }

    void ReleaseProgram() {
        _program.release();
    }
    
    ~View2D()
    {
        _gpuBuffer.release();
        _program.release();
        //makeCurrentContext();
    }
};
