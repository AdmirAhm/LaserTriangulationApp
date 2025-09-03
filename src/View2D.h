#pragma once
#include <gui/gl/View.h>
#include <gui/gl/Buffer.h>
#include "Program.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <gui/gl/Text.h>

class View2D : public gui::gl::View
{
    Program _program;
    gui::gl::Buffer _gpuBuffer;
    gui::gl::Buffer _gpuBuffer2;
    std::vector<cv::Point3f>_pts;
    int ind = 0;
    float _zoomFactor = 1.0f;
    gui::gl::Text _text;
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
    void updateTexts() {
        gui::Size sz;
        gui::gl::Font& fnt = _text.getFont(0);
        fnt.reset();
        std::stringstream ss;
        auto x = getDistance(_pts[0], _pts[ind]);
        auto y = _pts[ind].z;
        ss << "X:" << x << " Y:" << y;
        std::string pom;
        td::String text1, text2;
        ss >> pom;
        text1 = pom;
        ss >> pom;
        text2 = pom;
        gui::gl::Font::Format format;
        format.scale = 1.0f;
        format.colorID = td::ColorID::Green;
        gui::gl::Size txtSize1 = fnt.addString({10, 10}, text1.c_str(), 0, format);
        gui::gl::Size txtSize2 = fnt.addString({10, 20 + txtSize1.height }, text2.c_str(), 0, format);

    }
    void setTexts() {
        gui::Size sz;
        getSize(sz);
        _text.init(sz, gui::gl::Textures::Unit::T15);
        _text.allocFonts(1);
        _text.loadFont(0, ":arial");
        updateTexts();
        _text.commit();
    }
    void setupVBO()
    {
        std::sort(_pts.begin(), _pts.end(), [](cv::Point3f a, cv::Point3f b) {return a.x < b.x; });


        // Vertex data for a simple rectangle with colors
        gui::gl::VertexType *vertices = new gui::gl::VertexType[5*_pts.size()];
        auto a = 0.5f*getDistance(_pts[0], _pts[_pts.size() - 1]);
        for (int i = 0; i < _pts.size(); i++) {
            vertices[i * 5] = getDistance(_pts[0], _pts[i])/a-1.0f;
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
        setTexts();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({ gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth });
        
        if (!_gpuBuffer.commit()) //transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }

        _program.setBuffer(&_gpuBuffer);
        _program.activate();
        _program.setZoom(_zoomFactor);
        _program.execute();
        _program.deActivate();

        _text.render();
        if (!_gpuBuffer2.commit()) //transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }


        _program.setBuffer(&_gpuBuffer2);
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
        _gpuBuffer2.init(4, 20, 0, { gui::gl::DataType::vec2, gui::gl::DataType::vec3 });

        gui::gl::Context reqContext;
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, 
            gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
        _pts = pts;
    }

    void ReleaseProgram() {
        _program.release();
    }
    
    bool onKeyPressed(const gui::Key& key) override {
        if (key.getChar() == 'a') {
            if (key.getModifiers() == (td::UINT4)gui::KeyModifier::Ctrl) ind -= 10;
            else ind--;
            if (ind < 0) ind = 0;
        }
        if (key.getChar() == 'd') {
            if (key.getModifiers() == (td::UINT4)gui::KeyModifier::Ctrl) ind += 10;
            else ind++;
            if (ind >= _pts.size()) ind = _pts.size()-1;
        }
        auto a = 0.5f * getDistance(_pts[0], _pts[_pts.size() - 1]);
        auto x = getDistance(_pts[0], _pts[ind]) / a - 1.0f;
        auto z = _pts[ind].z/100.0f;
        gui::gl::VertexType vertices[] = {
            x, 1.0f, 1.0f, 0.0f, 0.0f,
            x, -1.0f, 1.0f, 0.0f, 0.0f,
            x-0.01, z-0.01, 0.0f, 1.0f, 0.0f,
            x-0.01, z+0.01, 0.0f, 1.0f, 0.0f,
            x+0.01, z+0.01, 0.0f, 1.0f, 0.0f,
            x+0.01, z-0.01, 0.0f, 1.0f, 0.0f
        };
        _gpuBuffer2.reset();
        _gpuBuffer2.init(4, 20, 0, { gui::gl::DataType::vec2, gui::gl::DataType::vec3 });

        _gpuBuffer2.appendVertices(&vertices[0], 6);
        

        //create command for rect
        auto cmd = _gpuBuffer2.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::Lines, 0, 2);
        _gpuBuffer2.encode(cmd);

        cmd = _gpuBuffer2.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::LineLoop, 2, 4);
        _gpuBuffer2.encode(cmd);

        updateTexts();
        reDraw();
        return true;
    }

    void onResize(const gui::Size& newSize) override
    {
        if (newSize.width == 0)
            return;
        if (newSize.height == 0)
            return;
        _text.setFrameSize(newSize);
    }

    ~View2D()
    {
        _gpuBuffer.release();
        _program.release();
        //makeCurrentContext();
    }
};
