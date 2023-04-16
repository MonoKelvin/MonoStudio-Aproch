#include "stdafx.h"
#include "FUIElement.h"

APROCH_NAMESPACE_BEGIN

FUIElement::FUIElement(FUICanvas *canvas)
//: QOpenGLWidget(canvas)
{
}

FUIElement::~FUIElement()
{
}

void FUIElement::initializeGL()
{
    initializeOpenGLFunctions();
}

void FUIElement::resizeGL(int w, int h)
{
}

void FUIElement::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

APROCH_NAMESPACE_END
