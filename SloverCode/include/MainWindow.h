#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GLWindow.h"

/// @file MainWindow.h
/// @brief represents the main gui application window, modified from JMacey's demo
/// @author Shalin Banjara
/// @version 1.0
/// @date 04.11.11
/// Revision History :
/// Initial Version 04.11.11
/// @class MainWindow
/// @brief contains all the UI extended from MainWindow.ui and our GLWindow

/// @namespace Ui the Ui namespace created from the MainWindow class
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// @brief ctor
    /// @param[in] parent the main application window for the GUI
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    /// @brief the handle to the main GUI window, used for creating all the UI and the gl window
    Ui::MainWindow *m_ui;

    /// @brief openGL widget
    GLWindow *m_gl;

};

#endif // MAINWINDOW_H
