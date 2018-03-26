#include "cube.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "model.h"

Cube::Cube(float init_velocity, float init_angle){
    velocity_ = init_velocity;
    angle_ = init_angle;
    animated_ = true;
}


void Cube::Move(float delta_t){
    if (!animated_) return;
    angle_ += delta_t * velocity_;
    if(angle_>360) angle_ -= 360;
    if(angle_<-360) angle_ += 360;
    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutX(angle_);
    model_matrix_.RotateAboutY(angle_);
}

void Cube::MoveY(float delta_y){
    model_matrix_.RotateAboutY(delta_y);
}

void Cube::MoveX(float delta_x){
    model_matrix_.RotateAboutX(delta_x);
}

void Cube::SpeedUp(){
    velocity_ *= 1.09544511501;
}

void Cube::SlowDown(){
    velocity_ /= 1.09544511501;
}

void Cube::ToggleAnimated(){
    animated_ = ! animated_;
}

void Cube::Initialize(){
    const ColorVertex kVertices[12] =
    {
        { { -.5f, -.5f,  .5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	    { { -.5f,  .0f,  .5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	    { {  .5f,  .0f,  .5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	    { {  .5f, -.5f,  .5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	    { { -.5f, -.5f, -.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	    { { -.5f,  .0f, -.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	    { {  .5f,  .0f, -.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	    { {  .5f, -.5f, -.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	    { { -.5f,  .0f,  .0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	    { {  .5f,  .0f,  .0f, 1.0f }, { 0.0f, 0.5f, 1.0f, 1.0f } },
	    { {  .0f,  .5f, -.5f, 1.0f }, { 0.5f, 0.5f, 1.0f, 1.0f } },
	    { {  .0f, -.5f,  .5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
    };

    const GLuint kIndices[63] =
    {
		0,1,11, 11,3,2,
	    0,4,3,   3,4,7,
	    0,1,8,   8,4,5,
	    0,4,8,   2,3,9,
	    6,7,9,   3,9,7,
	    6,7,9,   3,9,7,
	    4,5,6,   4,6,7,
	    5,6,10,  5,8,10,
	    10,9,6,  10,8,9,
	    8,9,11,  1,8,11,
	    11,2,9,
    };


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]),
                          (GLvoid*) sizeof(kVertices[0].position));
    glEnableVertexAttribArray(1);


    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Cube::~Cube(){
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &index_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
}

void Cube::Draw(const Program &program) const{

    glUseProgram(program);
    glBindVertexArray(vao_);

    program.SetModelMatrix(model_matrix_);

    glDrawElements(GL_TRIANGLES, 63, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);


}
