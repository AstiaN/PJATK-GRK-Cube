#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>

#include "program.h"
#include "matma.h"

class Cube{
 public:
    Cube(float init_velocity=10, float init_angle=0);
    ~Cube();
    void SetInitAngle(float angle){angle_= angle;}
    void SetVelocity(float velocity){velocity_= velocity;}
    void Initialize();
    void Draw(const Program & program) const;
    void Move(float delta_t);
    void MoveY(float delta_y);
    void MoveX(float delta_x);
    void SpeedUp();
    void SlowDown();
    void ToggleAnimated();
 private:
    GLuint vao_;
    GLuint vertex_buffer_;
    GLuint index_buffer_;
    Mat4 model_matrix_;

    float angle_;
    float velocity_;
    bool animated_;

};

#endif // CUBE_H
