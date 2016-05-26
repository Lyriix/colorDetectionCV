

#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/gl.h>
#include <GL/glew.h>

#include "../../lib/3d/mat3.hpp"
#include "../../lib/3d/vec3.hpp"
#include "../../lib/mesh/mesh.hpp"
#include "../../lib/opengl/mesh_opengl.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include <vector>
#include <opencv2/opencv.hpp>
#include <list>

#include "multipletrackingObject.hpp"


class myWidgetGL;

class scene
{
public:

    scene();



    /** \brief Method called only once at the beginning (load off files ...) */
    void load_scene();

    /** \brief Method called at every frame */
    void draw_scene();

    /** Set the pointer to the parent Widget */
    void set_widget(myWidgetGL* widget_param);



private:

    /** Load a texture from a given file and returns its id */
    GLuint load_texture_file(std::string const& filename);

    /** Load common data such as shader and files */
    void load_common_data();

    /** Load the recorder from the webcam */
    void start_webcam();
    /** Generate an openglGL texture from the OpenCV image */
    GLuint generate_texture_webcam(cv::Mat const& im);

    /** Load a shader and send the camera matrices */
    void prepare_shader(GLuint shader_id);
    /** Perform some image analysis */
    void analyse_image(cv::Mat & im);





    /** Access to the parent object */
    myWidgetGL* pwidget;

    /** Default id for the texture (white texture) */
    GLuint texture_default;

    /** The id of the shader do draw meshes */
    GLuint shader_mesh;

    /** The shader dealing with the webcam in background */
    GLuint shader_webcam;




    /** Mesh for the webcam in background */
    cpe::mesh mesh_camera;
    cpe::mesh_opengl mesh_camera_opengl;



    /** OpenCV webcam capture interface */
    cv::VideoCapture capture;

    /** Time parameter */
    int time_advance;

    trackingObject obj;


};

#endif
