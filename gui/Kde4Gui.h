// 
//   Copyright (C) 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef GNASH_KDE4GUI_H
#define GNASH_KDE4GUI_H

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include "gui.h"

#include <QX11EmbedWidget>

#ifdef RENDERER_OPENGL
# error "OGL not supported yet for KDE4!"
#elif defined(RENDERER_CAIRO)
# error "Cairo not supported yet for KDE4!"
#elif defined(RENDERER_AGG)
# include "Kde4GlueAgg.h"
#endif


class QMainWindow;
class QMenuBar;
class QMenu;
class QRect;

namespace gnash {
    class Kde4Gui;
}

namespace gnash
{

class DrawingWidget : public QX11EmbedWidget
{
    Q_OBJECT

public:
    DrawingWidget(Kde4Gui& gui)
        :
        QX11EmbedWidget(),
        _gui(gui)
    {}

    ~DrawingWidget() {}

public slots:

    void play();
    void pause();
    void stop();
    void restart();
    void refresh();
    void fullscreen(bool isFull);

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void resizeEvent(QResizeEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:

    Kde4Gui& _gui;

};


class DSOEXPORT Kde4Gui :  public Gui
{

public:
    Kde4Gui(unsigned long xid, float scale, bool loop, unsigned int depth);
    virtual ~Kde4Gui();
    virtual bool init(int argc, char **argv[]);
    virtual bool createWindow(const char* windowtitle, int width, int height);
    virtual bool run();
    virtual void renderBuffer();
    virtual void setInterval(unsigned int interval);
    virtual void setTimeout(unsigned int timeout);
    virtual void handleKeyEvent(QKeyEvent *event, bool down);
    virtual void setCursor(gnash_cursor_type newcursor);
    virtual void setFullscreen();
    virtual void unsetFullscreen();
    void setInvalidatedRegions(const InvalidatedRanges& ranges);
    void resize(int width, int height);
    void quit();

    bool want_multiple_regions() { return true; }

    void renderWidget(const QRect& updateRect);

    void popupMenu(const QPoint& point);

private:

    typedef std::vector<geometry::Range2d<int> > DrawBounds; 
    typedef std::map<int, gnash::key::code> KeyMap;

    void setupActions();
    void setupMenus();
    void createMainMenu();

    /// Set up the map of Qt to Gnash keys.
    void setupKeyMap();

    DrawBounds::value_type _validbounds;
    DrawBounds _drawbounds;
 
    /// The main application, which should destroy everything
    /// left on closing.
    std::auto_ptr<QApplication>  _application;
    
    /// The widget for rendering and handling user events.
    //
    /// Ownership is transferred to the main window, which
    /// takes care of deletion.
    DrawingWidget* _drawingWidget;
    
    /// Takes care of painting onto the widget.
    Kde4AggGlue _glue;
    
    /// The main application window.
    std::auto_ptr<QMainWindow> _window;

    /// A map for Qt::Key values that don't easily
    /// map onto Gnash ones.
    KeyMap _keyMap;

    gnash::key::code qtToGnashKey(QKeyEvent *event);

    /// QActions and QMenus should be attached to the
    /// QMainWindow so that they are destroyed with it.
    /// Actions may be shared between menus and/or
    /// other uses.

    // File Menu
    QMenu* fileMenu;
    QAction* quitAction;
    
    // Movie Control Menu;
    QMenu* movieControlMenu;
    QAction* playAction;
    QAction* pauseAction;
    QAction* stopAction;
    QAction* restartAction;
    
    // View Menu
    QMenu* viewMenu;
    QAction* refreshAction;
    QAction* fullscreenAction;

};

}

#endif