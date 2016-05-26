
#include <GL/glew.h>

#include "scene.hpp"
#include "../../lib/common/error_handling.hpp"
#include "../../lib/opengl/glutils.hpp"
#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"
#include "../interface/myWidgetGL.hpp"
#include <cmath>

#include <string>
#include <sstream>
#include "../../lib/mesh/mesh_io.hpp"


using namespace cpe;

void scene::load_scene()
{
    load_common_data();
    start_webcam();

    //*****************************************//
    // Background                              //
    //*****************************************//

    mesh_camera.add_vertex( { -1.0f , -1.0f , 0.5f } );
    mesh_camera.add_vertex( { -1.0f , +1.0f , 0.5f } );
    mesh_camera.add_vertex( { +1.0f , +1.0f , 0.5f } );
    mesh_camera.add_vertex( { +1.0f , -1.0f , 0.5f } );

    mesh_camera.add_texture_coord( { 1.0f , 1.0f } );
    mesh_camera.add_texture_coord( { 1.0f , 0.0f } );
    mesh_camera.add_texture_coord( { 0.0f , 0.0f } );
    mesh_camera.add_texture_coord( { 0.0f , 1.0f } );

    mesh_camera.add_triangle_index( { 0,2,1 } );
    mesh_camera.add_triangle_index( { 0,3,2 } );

    mesh_camera.fill_empty_field_by_default();
    mesh_camera_opengl.fill_vbo(mesh_camera);

}

void scene::draw_scene()
{
    // Get webcam image
    cv::Mat frame;
    capture >> frame;
    analyse_image(frame);
    GLuint texture_webcam = generate_texture_webcam(frame);

    // draw webcam image as a background
    glUseProgram(shader_webcam);                               PRINT_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D,texture_webcam);               PRINT_OPENGL_ERROR();
    glDepthMask(GL_FALSE);                                     PRINT_OPENGL_ERROR();
    mesh_camera_opengl.draw();
    glDepthMask(GL_TRUE);                                      PRINT_OPENGL_ERROR();



    // Delete the texture on the GPU
    glDeleteTextures(1, &texture_webcam);PRINT_OPENGL_ERROR();
    //trackingObject::multipleTracking();
    obj.multipleTracking(frame);
}



void scene::start_webcam()
{
    capture=cv::VideoCapture(0);
    if(!capture.isOpened())
    {
        std::cerr<<"Failed to open Camera"<<std::endl;
        exit(1);
    }
}

void scene::load_common_data()
{
    texture_default=load_texture_file("data/white.jpg");

    shader_mesh = read_shader("shaders/shader_mesh.vert",
                              "shaders/shader_mesh.frag"); PRINT_OPENGL_ERROR();



    shader_webcam = read_shader("shaders/shader_webcam.vert",
                              "shaders/shader_webcam.frag"); PRINT_OPENGL_ERROR();
}



void scene::analyse_image(cv::Mat & im)
{

}

void scene::prepare_shader(GLuint const shader_id)
{
    //Setup uniform parameters
    glUseProgram(shader_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();
}



GLuint scene::generate_texture_webcam(cv::Mat const& im)
{
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);  PRINT_OPENGL_ERROR();

    GLenum in_color = GL_BGR;
    if (im.channels() == 1)
        in_color = GL_LUMINANCE;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); PRINT_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); PRINT_OPENGL_ERROR();

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im.cols,im.rows,0,in_color,GL_UNSIGNED_BYTE,im.ptr()); PRINT_OPENGL_ERROR();

    return texture_id;
}

scene::scene()
    :shader_mesh(0),time_advance(0)
{}


GLuint scene::load_texture_file(std::string const& filename)
{
    return pwidget->load_texture_file(filename);
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}


