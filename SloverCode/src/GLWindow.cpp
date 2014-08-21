#include "GLWindow.h"
#include <iostream>
#include "ngl/Camera.h"
#include "ngl/Light.h"
#include "ngl/Matrix.h"
#include "ngl/Transformation.h"
#include "ngl/TransformStack.h"
#include "ngl/Material.h"
#include "ngl/NGLInit.h"
#include "ngl/VBOPrimitives.h"
#include "ngl/ShaderManager.h"
#include "ngl/Random.h"
#include "ngl/Util.h"
#include "SimulationParameters.h"
#include <QString>



//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(
                   QWidget *_parent
                  ) :
                    QGLWidget(_parent)
{

    SimulationParameters::initialiseCameraParameters(m_eye,m_look,m_upVector);
    SimulationParameters::initialiseExportParameters(m_exportEnabled,m_exportPath,m_exportFrameInterval);
    // set this widget to have the initial keyboard focus
    setFocus();
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    this->resize(_parent->size());
    m_rotate=true;
    m_FluidTimer = startTimer(0);
    m_fpsTimer =startTimer(0);
    m_fps=0;
    m_frames=0;
    m_elapsedSimulationTime = 0.0;

    m_elapsedTime = 0.0;

    m_timer.start();
    m_iterationTimer.start();

    m_animate = true;
    m_iterationProcessed = false;

    m_mouseMove = false;

    //create the integration
    m_integrate = new Integrate();

    //create the environment
    m_environmentAndRigidBody = new EnvironmentAndRigidBody();

    //create the fluid solver
    m_sphSolver = new SPHSolver();

    m_render = new Render();

    if (m_exportEnabled)
    {
        m_export = new Export(m_exportPath);
    }
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);

   // Now we will create a basic Camera from the graphics library
   // This is a static camera so it only needs to be set once
   // First create Values for the camera position

   ngl::NGLInit *Init = ngl::NGLInit::instance();
   Init->initGlew();
   m_cam= new ngl::Camera(m_eye,m_look,m_upVector,ngl::PERSPECTIVE);
   // set the shape using FOV 45 Aspect Ratio based on Width and Height
   // The final two are near and far clipping planes of 0.5 and 10
   m_cam->setShape(50,1.0,0.5,150,ngl::PERSPECTIVE);
   m_render->initialiseRender(m_environmentAndRigidBody);


   glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL(
                        int _w,
                        int _h
                       )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,(float)_w/_h,0.001,500,ngl::PERSPECTIVE);
  m_render->updateProjection(m_cam);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // build our transform stack
    ngl::Transformation trans;
    // set the mouse rotation
    trans.setRotation(m_spinXFace,m_spinYFace,0);
    // set this in the TX stack
    m_transformStack.setGlobal(trans);

    m_render->draw(m_environmentAndRigidBody,m_sphSolver,m_transformStack,m_cam);

    if (m_mouseMove == false)
    {
        m_iterationProcessed = true;
    }

    glUseProgramObjectARB(0);
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    glColor3f(0.7,0.7,0.7);

    QString text =QString("Total number of fluid particles : %1").arg(m_sphSolver->getFluidParticleList().size());
    renderText(500,20,text,font);

    text =QString("Integration Method used : %1").arg(m_integrate->getIntegrateType().c_str());
    renderText(500,40,text,font);

    text =QString("Timestep value : %1").arg(m_integrate->getTimestep());
    renderText(500,60,text,font);

    text =QString("Iterations performed per second : %1").arg(m_fps);
    renderText(15,20,text,font);

    text =QString("Elapsed time inside the simulation model : %1  seconds").arg(floor(m_elapsedSimulationTime));
    renderText(15,40,text,font);

    float tmp = m_iterationTimer.elapsed();
    tmp = tmp /  1000;
    text =QString("Time taken to process a single Iteration : %1  seconds").arg(tmp);
    renderText(15,60,text,font);
    m_iterationTimer.restart();

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    m_spinYFace = ( m_spinYFace + (_event->x() - m_origX) ) % 360 ;
    m_spinXFace = ( m_spinXFace + (_event->y() - m_origY) ) % 360 ;
    m_origX = _event->x();
    m_origY = _event->y();
  }
  // re-draw GL
  m_mouseMove = true;
  updateGL();
  m_mouseMove = false;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
}

void GLWindow::timerEvent(
                          QTimerEvent *_event
                         )
{
        if(_event->timerId() == m_FluidTimer)
        {
                if (m_animate ==true)
                {                                        
                    if(m_iterationProcessed)
                    {
                        m_iterationProcessed = false;
                        m_elapsedTime = m_elapsedTime + m_integrate->getTimestep();
                        m_elapsedSimulationTime += m_integrate->getTimestep();
                        ++m_frames;
                        m_sphSolver->updateFluid(m_environmentAndRigidBody,m_integrate);
                        if (m_exportEnabled)
                        {
                            if (m_elapsedTime >= m_exportFrameInterval)
                            {
                            m_export->exportDataOut(m_sphSolver->getFluidNameList(),m_sphSolver->getFluidParticlePositionList(),m_environmentAndRigidBody);
                            m_elapsedTime = 0;
                            }
                        }
                        updateGL();
                        return;
                    }
                }

        }
        if(_event->timerId() == m_fpsTimer)
          {
            if( m_timer.elapsed() > 1000.0)
            {
              m_fps=m_frames;
              m_frames=0;
              m_timer.restart();
            }
           }
}


//This is a dtor for openGL Window.
// Used for memory management.
GLWindow::~GLWindow()
{
    delete m_sphSolver;
    delete m_environmentAndRigidBody;
    delete m_integrate;
}






