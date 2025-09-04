#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/Button.h>
#include <gui/GridComposer.h>
#include <gui/NumericEdit.h>
#include <gui/FileDialog.h>
#include <xml/Writer.h>
#include <xml/DOMParser.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <filesystem>

class ViewCalib : public gui::View
{
private:
protected:
    gui::GridLayout _gl;
    gui::VerticalLayout _vl;
    gui::HorizontalLayout _hlbtn;
    gui::HorizontalLayout _hldist;
    gui::HorizontalLayout _hlr;
    gui::HorizontalLayout _hlt;
    gui::Label _autolbl;
    gui::Label _poljelbl;
    gui::Label _plocalbl;
    gui::Label _putlbl;

    gui::LineEdit _put;
    gui::NumericEdit _poljex;
    gui::NumericEdit _poljey;
    gui::NumericEdit _plocax;
    gui::NumericEdit _plocay;
    gui::GridComposer gc;

    gui::Button _browse;
    gui::Button _auto;
    gui::Button _import;
    gui::Button _export;
    gui::Button _manual;

    gui::Label _manuallbl;
    gui::Label _fxlbl;
    gui::Label _fylbl;
    gui::Label _cxlbl;
    gui::Label _cylbl;
    gui::Label _distlbl;
    gui::Label _rveclbl;
    gui::Label _tveclbl;

    gui::NumericEdit _fx;
    gui::NumericEdit _fy;
    gui::NumericEdit _cx;
    gui::NumericEdit _cy;
    gui::NumericEdit _dist1;
    gui::NumericEdit _dist2;
    gui::NumericEdit _dist3;
    gui::NumericEdit _dist4;
    gui::NumericEdit _dist5;
    gui::NumericEdit _rvec1;
    gui::NumericEdit _rvec2;
    gui::NumericEdit _rvec3;
    gui::NumericEdit _tvec1;
    gui::NumericEdit _tvec2;
    gui::NumericEdit _tvec3;

    cv::Mat* cameraMatrix;
    cv::Mat* dist;
    cv::Mat* r;
    cv::Mat* t;
public:
    ViewCalib(cv::Mat *cam, cv::Mat *dis, cv::Mat* rr, cv::Mat* tt)
    : _gl(13, 2)
    , _vl(6)
    , _autolbl("Auto Calibration:")
    , _poljelbl("Square Dimension:")
    , _plocalbl("Number of Squares:")
    , _putlbl("Image Path:")
    , _poljex(td::int4)
    , _poljey(td::int4)
    , _plocax(td::int4)
    , _plocay(td::int4)
    , _rveclbl("rvec: ")
    , _tveclbl("tvec: ")
    , gc(_gl)
    , _browse("Browse")
    , _auto("Auto")
    , _manual("Save")
    , _import("Import")
    , _export("Export")
    , _manuallbl("Manual Calibration:")
    , _fxlbl("Fx=")
    , _cxlbl("Cx=")
    , _fylbl("Fy=")
    , _cylbl("Cy=")
    , _fx(td::real8)
    , _fy(td::real8)
    , _cx(td::real8)
    , _cy(td::real8)
    , _hlbtn(5)
    , _hldist(6)
    , _hlt(4)
    , _hlr(4)
    , _dist1(td::real8)
    , _dist2(td::real8)
    , _dist3(td::real8)
    , _dist4(td::real8)
    , _dist5(td::real8)
    , _rvec1(td::real8)
    , _rvec2(td::real8)
    , _rvec3(td::real8)
    , _tvec1(td::real8)
    , _tvec2(td::real8)
    , _tvec3(td::real8)
    , _distlbl("Distortion Coefficients:")
    {
        cameraMatrix = cam;
        dist = dis;
        r = rr;
        t = tt;
        _browse.setType(gui::Button::Type::Default);

        gc.appendRow(_autolbl, 2, td::HAlignment::Center);
        gc.appendRow(_putlbl);
        gc.appendRow(_put);
        gc.appendCol(_browse);
        gc.appendRow(_poljelbl);
        gc.appendRow(_poljex);
        gc.appendCol(_poljey);
        gc.appendRow(_plocalbl);
        gc.appendRow(_plocax);
        gc.appendCol(_plocay);
        gc.appendRow(_manuallbl, 2, td::HAlignment::Center);
        gc.appendRow(_fxlbl, td::HAlignment::Right);
        gc.appendCol(_fx);
        gc.appendRow(_fylbl, td::HAlignment::Right);
        gc.appendCol(_fy);
        gc.appendRow(_cxlbl, td::HAlignment::Right);
        gc.appendCol(_cx);
        gc.appendRow(_cylbl, td::HAlignment::Right);
        gc.appendCol(_cy);


        _hlbtn.append(_auto);
        _hlbtn.append(_import);
        _hlbtn.append(_export);
        _hlbtn.appendSpacer();
        _hlbtn.append(_manual);

        _hldist.append(_distlbl);
        _hldist.append(_dist1);
        _hldist.append(_dist2);
        _hldist.append(_dist3);
        _hldist.append(_dist4);
        _hldist.append(_dist5);

        _hlr.append(_rveclbl);
        _hlr.append(_rvec1);
        _hlr.append(_rvec2);
        _hlr.append(_rvec3);

        _hlt.append(_tveclbl);
        _hlt.append(_tvec1);
        _hlt.append(_tvec2);
        _hlt.append(_tvec3);

        _vl.append(_gl);
        _vl.append(_hldist);
        _vl.append(_hlr);
        _vl.append(_hlt);
        _vl.appendSpacer();
        _vl.append(_hlbtn);
        
        setLayout(&_vl);

        //cv::Mat image;
        //image = cv::imread("apple-1.bmp", cv::IMREAD_COLOR_RGB);
        //cv::imshow("prozor", image);
        //cv::waitKey(0);
    }

    void Export(std::string path) {
        xml::Writer w(path);
        w.startDocument();
        w.startNode("Params");

        double fx = _fx.getValue().r8Val();
        double fy = _fy.getValue().r8Val();
        double cx = _cx.getValue().r8Val();
        double cy = _cy.getValue().r8Val();
        double dist1 = _dist1.getValue().r8Val();
        double dist2 = _dist2.getValue().r8Val();
        double dist3 = _dist3.getValue().r8Val();
        double dist4 = _dist4.getValue().r8Val();
        double dist5 = _dist5.getValue().r8Val();
        double rvec1 = _rvec1.getValue().r8Val();
        double rvec2 = _rvec2.getValue().r8Val();
        double rvec3 = _rvec3.getValue().r8Val();
        double tvec1 = _tvec1.getValue().r8Val();
        double tvec2 = _tvec2.getValue().r8Val();
        double tvec3 = _tvec3.getValue().r8Val();

        w.startNode("param");
        w.attribute("id", "f");
        w.attribute("fx", fx);
        w.attribute("fy", fy);
        w.endNode();

        w.startNode("param");
        w.attribute("id", "c");
        w.attribute("cx", cx);
        w.attribute("cy", cy);
        w.endNode();

        w.startNode("param");
        w.attribute("id", "dist");
        w.attribute("dist1", dist1);
        w.attribute("dist2", dist2);
        w.attribute("dist3", dist3);
        w.attribute("dist4", dist4);
        w.attribute("dist5", dist5);
        w.endNode();

        w.startNode("param");
        w.attribute("id", "rvec");
        w.attribute("rvec1", rvec1);
        w.attribute("rvec2", rvec2);
        w.attribute("rvec3", rvec3);
        w.endNode();

        w.startNode("param");
        w.attribute("id", "tvec");
        w.attribute("tvec1", tvec1);
        w.attribute("tvec2", tvec2);
        w.attribute("tvec3", tvec3);
        w.endNode();
        w.endDocument();
    }

    void Import(std::string path) {
        xml::FileParser parser;
        if (!parser.parseFile((td::String)path)) {
            return;
        }
        auto root = parser.getRootNode();
        if (root->getName().cCompare("Params") != 0) return;
        auto param = root.getChildNode("param");
        while (param.isOk()) {
            td::String id;
            param.getAttribValue("id", id);
            if (id.cCompare("f")==0) {
                double fx, fy;
                param.getAttribValue("fx", fx);
                param.getAttribValue("fy", fy);
                _fx.setValue(fx);
                _fy.setValue(fy);
            }
            if (id.cCompare("c") == 0) {
                double cx, cy;
                param.getAttribValue("cx", cx);
                param.getAttribValue("cy", cy);
                _cx.setValue(cx);
                _cy.setValue(cy);
            }
            if (id.cCompare("dist") == 0) {
                double dist1, dist2, dist3, dist4, dist5;
                param.getAttribValue("dist1", dist1);
                param.getAttribValue("dist2", dist2);
                param.getAttribValue("dist3", dist3);
                param.getAttribValue("dist4", dist4);
                param.getAttribValue("dist5", dist5);
                _dist1.setValue(dist1);
                _dist2.setValue(dist2);
                _dist3.setValue(dist3);
                _dist4.setValue(dist4);
                _dist5.setValue(dist5);
            }
            if (id.cCompare("rvec") == 0) {
                double rvec1, rvec2, rvec3;
                param.getAttribValue("rvec1", rvec1);
                param.getAttribValue("rvec2", rvec2);
                param.getAttribValue("rvec3", rvec3);
                _rvec1.setValue(rvec1);
                _rvec2.setValue(rvec2);
                _rvec3.setValue(rvec3);
            }
            if (id.cCompare("tvec") == 0) {
                double tvec1, tvec2, tvec3;
                param.getAttribValue("tvec1", tvec1);
                param.getAttribValue("tvec2", tvec2);
                param.getAttribValue("tvec3", tvec3);
                _tvec1.setValue(tvec1);
                _tvec2.setValue(tvec2);
                _tvec3.setValue(tvec3);
            }
            ++param;
        }

    }

    bool onClick(gui::Button* pBtn) override {
        if (pBtn == &_browse) {
            showSelectFolderDialog();
        }
        if (pBtn == &_auto) {
            kalibracija();
        }
        if (pBtn == &_manual) {
            *dist = cv::Mat::zeros(5, 1, CV_64F);
            *r = cv::Mat::zeros(3, 1, CV_64F);
            *t = cv::Mat::zeros(3, 1, CV_64F);
            double fx = _fx.getValue().r8Val();
            double fy = _fy.getValue().r8Val();
            double cx = _cx.getValue().r8Val();
            double cy = _cy.getValue().r8Val();
            *cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
            cameraMatrix->at<double>(0, 0) = fx;
            cameraMatrix->at<double>(1, 1) = fy;
            cameraMatrix->at<double>(0, 2) = cx;
            cameraMatrix->at<double>(1, 2) = cy;

            double dist1=_dist1.getValue().r8Val();
            double dist2=_dist2.getValue().r8Val();
            double dist3=_dist3.getValue().r8Val();
            double dist4=_dist4.getValue().r8Val();
            double dist5=_dist5.getValue().r8Val();

            dist->at<double>(0, 0)=dist1;
            dist->at<double>(1, 0)=dist2;
            dist->at<double>(2, 0)=dist3;
            dist->at<double>(3, 0)=dist4;
            dist->at<double>(4, 0)=dist5;

            double rvec1 = _rvec1.getValue().r8Val();
            double rvec2 = _rvec2.getValue().r8Val();
            double rvec3 = _rvec3.getValue().r8Val();

            r->at<double>(0, 0) = rvec1;
            r->at<double>(1, 0) = rvec2;
            r->at<double>(2, 0) = rvec3;

            double tvec1 = _tvec1.getValue().r8Val();
            double tvec2 = _tvec2.getValue().r8Val();
            double tvec3 = _tvec3.getValue().r8Val();

            t->at<double>(0, 0) = tvec1;
            t->at<double>(1, 0) = tvec2;
            t->at<double>(2, 0) = tvec3;

        }

        if (pBtn == &_export) {
            gui::SaveFileDialog::show(this, "Save Calibration", { {"XML file", "*.xml"} }, 3);
        }

        if (pBtn == &_import) {
            gui::OpenFileDialog::show(this, "Open Calibration", { {"XML file", "*.xml"} }, 4);
        }
        return false;
    }

    void showSelectFolderDialog() {
        gui::SelectFolderDialog::show(this, "Kalibracija", 2);
    }

    void setPath(td::String str) {
        _put.setValue(str);
    }

    std::vector<cv::Point2f> getCorners(std::string path, cv::Mat& slika) {
        int HEI=_plocax.getValue().i4Val()-1;
        int WID=_plocay.getValue().i4Val()-1;
        std::vector<cv::Point2f> corners;
        slika = cv::imread(path, cv::IMREAD_COLOR);
        cv::Mat gray;
        cv::cvtColor(slika, gray, cv::COLOR_BGR2GRAY);
        cv::findChessboardCorners(gray, cv::Size(HEI, WID), corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);
        cv::cornerSubPix(gray, corners, cv::Size(10, 10), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 3000, 0.000001));
        cv::drawChessboardCorners(slika, cv::Size(HEI, WID), corners, true);
        cv::imshow("slika", slika);
        cv::waitKey(0);
        return corners;
    }
    
    void kalibracija() {
        *r = cv::Mat::zeros(3, 1, CV_64F);
        *t = cv::Mat::zeros(3, 1, CV_64F);
        *cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        td::Variant var;
        _put.getValue(var);
        td::String path = var.strVal();
        std::vector<cv::Mat> slike;
        std::vector<std::vector<cv::Point2f> >corners;
        int i = 0;
        for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
            std::string s = entry.path().string();
            slike.push_back(cv::Mat());
            corners.push_back(getCorners(s, slike[i]));
            i++;
        }
        int HEI = _plocax.getValue().i4Val()-1;
        int WID = _plocay.getValue().i4Val()-1;
        int HEI1 = _poljex.getValue().i4Val();
        int WID1 = _poljey.getValue().i4Val();
        std::vector<cv::Point3f> obj;
        for (int i = 0; i < WID; i++) {
            for (int j = 0; j < HEI; j++) {
                obj.push_back(cv::Point3f(i * 1.0f * WID1, j * 1.0f * HEI1, 0));
            }
        }
        int BR_SLIKA = slike.size();
        std::vector<cv::Mat> rvecs, tvecs;
        *dist = cv::Mat::zeros(5, 1, CV_64F);

        std::vector<std::vector<cv::Point3f> >objects(BR_SLIKA, obj);

        double error = cv::calibrateCamera(objects, corners, slike[0].size(), *cameraMatrix, *dist, rvecs, tvecs);
        for (int i = 0; i < BR_SLIKA; i++) {
            cv::solvePnP(objects[i], corners[i], *cameraMatrix, *dist, rvecs[i], tvecs[i], false, cv::SOLVEPNP_ITERATIVE);
        }

        *r = rvecs[0];
        *t = tvecs[0];

        _fx.setValue(cameraMatrix->at<double>(0, 0));
        _fy.setValue(cameraMatrix->at<double>(1, 1));
        _cx.setValue(cameraMatrix->at<double>(0, 2));
        _cy.setValue(cameraMatrix->at<double>(1, 2));

        _dist1.setValue(dist->at<double>(0, 0));
        _dist2.setValue(dist->at<double>(1, 0));
        _dist3.setValue(dist->at<double>(2, 0));
        _dist4.setValue(dist->at<double>(3, 0));
        _dist5.setValue(dist->at<double>(4, 0));

        _rvec1.setValue(r->at<double>(0, 0));
        _rvec2.setValue(r->at<double>(1, 0));
        _rvec3.setValue(r->at<double>(2, 0));

        _tvec1.setValue(t->at<double>(0, 0));
        _tvec2.setValue(t->at<double>(1, 0));
        _tvec3.setValue(t->at<double>(2, 0));

        cv::Mat R;
        cv::Rodrigues(rvecs[0], R);
    }
    bool onClick(gui::FileDialog* pFD, td::UINT4 dlgID) override
    {
        if (dlgID == 2)
        {
            td::String pathp = pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                _put.setValue(pathp);
            }
            return true;
        }

        if (dlgID == 3)
        {
            td::String pathp = pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                Export(path);
            }
            return true;
        }

        if (dlgID == 4)
        {
            td::String pathp = pFD->getFileName();
            std::string path = pathp.c_str();
            if (path != "") {
                Import(path);
            }
            return true;
        }
        return false;
    }
};
