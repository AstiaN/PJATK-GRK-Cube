#include "window.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>

#include "glerror.h"

const char* kVertexShader="SimpleShader.vertex.glsl";
const char* kFragmentShader="SimpleShader.fragment.glsl";


Window::Window(const char * title, int width, int height){
    strcpy(title_, title);
    width_ = width;
    height_ = height;
    last_time_ = 0;
    cube_.SetInitAngle(15);
    cube_.SetVelocity(45);
}

void Window::Initialize(int argc, char * argv[], int major_gl_version, int minor_gl_version){

    InitGlutOrDie(argc, argv, major_gl_version, minor_gl_version);
    InitGlewOrDie();


    std::cout << "OpenGL initialized: OpenGL version: " << glGetString(GL_VERSION) << " GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    InitModels();
    InitPrograms();

    glUseProgram(program_);
    view_matrix_.Translate(0, 0, -2);
    program_.SetViewMatrix(view_matrix_);

    projection_matrix_ = Mat4::CreateProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
    program_.SetProjectionMatrix(projection_matrix_);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.8f, 0.9f, 0.5f, 0.0f);

}

void Window::InitGlutOrDie(int argc, char * argv[], int major_gl_version, int minor_gl_version){
    glutInit(&argc, argv);

    glutInitContextVersion(major_gl_version, minor_gl_version);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#ifdef DEBUG
    glutInitContextFlags(GLUT_DEBUG);
#endif

    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    glutInitWindowSize(width_, height_);

    glutInitDisplayMode(GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);

    int window_handle = glutCreateWindow(title_);

    if( window_handle < 1) {
        std::cerr << "ERROR: Could not create a new rendering window" << std::endl;
        exit(EXIT_FAILURE);
    }


}

void Window::InitGlewOrDie(){
    GLenum glew_init_result;
    glewExperimental = GL_TRUE;
    glew_init_result = glewInit();

    if (GLEW_OK != glew_init_result) {
        std::cerr << "Glew ERROR: " << glewGetErrorString(glew_init_result) << std::endl;
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    if(glDebugMessageCallback){
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC) OpenglCallbackFunction, NULL);
        GLuint unused_ids = 0;
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            &unused_ids,
            GL_FALSE);
    }
    else
        std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

}

void Window::InitModels(){
    cube_.Initialize();
}

void Window::InitPrograms(){
    program_.Initialize(kVertexShader, kFragmentShader);
}

void Window::Resize(int new_width, int new_height){
    width_ = new_width;
    height_ = new_height;
    projection_matrix_ = Mat4::CreateProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
    glUseProgram(program_);
    program_.SetProjectionMatrix(projection_matrix_);
    glViewport(0, 0, width_, height_);
    glutPostRedisplay();
}

void Window::KeyPressed(unsigned char key, int /*x_coord*/, int /*y_coord*/){
    switch (key){
    case 27:
      glutLeaveMainLoop();
    break;
    case ' ':
      cube_.ToggleAnimated();
    break;
    default:
    break;
    }
}

void Window::SpecialKeyPressed(int key, int x_coord, int y_coord){
    switch (key){
    case GLUT_KEY_LEFT:
      cube_.MoveY(-5);
    break;
    case GLUT_KEY_RIGHT:
      cube_.MoveY(5);
    break;
    case GLUT_KEY_DOWN:
      cube_.MoveX(5);
    break;
    case GLUT_KEY_UP:
      cube_.MoveX(-5);
    break;
    case GLUT_KEY_PAGE_DOWN:
      view_matrix_.Translate(0, 0, -0.1);
      glUseProgram(program_);
      program_.SetViewMatrix(view_matrix_);
    break;
    case GLUT_KEY_PAGE_UP:
      view_matrix_.Translate(0, 0, 0.1);
      glUseProgram(program_);
      program_.SetViewMatrix(view_matrix_);
    break;
    case GLUT_KEY_HOME:

    break;
    case GLUT_KEY_END:

    break;
      default:
      break;
    }
}


void Window::Render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    clock_t now = clock();
    if (last_time_ == 0) last_time_ = now;
    //cube_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
    last_time_ = now;

    cube_.Draw(program_);

    glutSwapBuffers();
    glutPostRedisplay();

}

