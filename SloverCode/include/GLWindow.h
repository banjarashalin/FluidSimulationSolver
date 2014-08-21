#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/TransformStack.h>
#include <ngl/Vector.h>
#include <ngl/BBox.h>
#include "SPHSolver.h"
#include "EnvironmentAndRigidBody.h"
#include "Integrate.h"
#include "Render.h"
#include "Export.h"

// must be included after our stuff becuase GLEW needs to be first
#include <QtOpenGL>

/// @file GLWindow.h
/// @brief  basic Qt GL window class for visualization of simulation data. Adapted from Jon Macey's lecture notes.
/// @author Shalin K Banjara
/// @version 1.0
/// @date 23.10.11
/// Revision History :
/// Initial Version 23.10.11
/// @class GLWindow
/// @brief management of entities required of simulation and visualization of fluids


class GLWindow : public QGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
public :

    /// @brief ctor
    /// @param [in] _parent the parent window to create the GL context in
    GLWindow(
                QWidget *_parent
            );

    /// @brief dtor
        ~GLWindow();

 public slots :

protected:

  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL();

  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  void resizeGL(
                const int _w,
                const int _h
               );

  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  // be re-drawn
  void paintGL();

private :
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  void mouseMoveEvent (
                       QMouseEvent * _event
                      );

  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  void mousePressEvent (
                        QMouseEvent *_event
                       );

  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  void mouseReleaseEvent (
                          QMouseEvent *_event
                         );

  /// @brief the timer event triggered from the timers
  /// @param _even the event of the timer triggered by Qt
  void timerEvent (
                    QTimerEvent *_event
                  );


    /// @brief used to store the x rotation mouse value
    int m_spinXFace;

    /// @brief used to store the y rotation mouse value
    int m_spinYFace;

    /// @brief flag to indicate if the mouse button is pressed when dragging
    bool m_rotate;

    /// @brief the previous x mouse value
    int m_origX;

    /// @brief the previous y mouse value
    int m_origY;

    /// @brief timer check if the iteration process is over
    int m_FluidTimer;

    /// @brief timer to to calculate frames per second
    int m_fpsTimer;

    /// @brief number iterations performed per second
    int m_fps;

    /// @brief number iterations performed within one second
    int m_frames;

    /// @brief elapsed time after the last simulation data was written
    float m_elapsedTime;

    /// @brief elapsed time inside the simulation model
    float m_elapsedSimulationTime;

    /// @brief flag to indicate if animation is active or not
    bool m_animate;

    /// @brief flag to check if a iteration is completed
    bool m_iterationProcessed;

    /// @brief eye of the camera
    ngl::Vector m_eye;

    /// @brief look/traget of the camera
    ngl::Vector m_look;

    /// @brief up vector for the camera
    ngl::Vector m_upVector;

    /// @brief timer to check the iterations performed per second
    QTime m_timer;

    /// @brief timer to check the iterations performed per second
    QTime m_iterationTimer;

    /// @brief flag to check exporting of simulation data
    bool m_exportEnabled;

    /// @brief path at which the simulation data is to be written on to the disk
    std::string m_exportPath;

    /// @brief time interval between two frames for writting simulation data
    float m_exportFrameInterval;

    /// @brief flag to check if a mouse move event in OpenGL Window has occured
    bool m_mouseMove;

    /// @brief our transform stack for drawing the teapots
    ngl::TransformStack m_transformStack;

    /// @brief pointer to ngl/Camera class
    ngl::Camera *m_cam;

    /// @brief pointer to SPHSolver class
    SPHSolver* m_sphSolver;

    /// @brief pointer to EnviornmentAndRigidBody class
    EnvironmentAndRigidBody* m_environmentAndRigidBody;

    /// @brief pointer to Integrate class
    Integrate* m_integrate;

    /// @brief pointer to Render class
    Render* m_render;

    /// @brief pointer to Export class
    Export* m_export;
};

#endif
