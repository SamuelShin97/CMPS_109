// $Id: graphics.h,v 1.1 2015-07-16 16:47:51-07 - - $

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      shared_ptr<shape> pshape;
      vertex center;
      rgbcolor color;
      bool border;
   public:
      // Default copiers, movers, dtor all OK.
      object(rgbcolor& c, const shared_ptr<shape>& ptr, vertex& v)
      {color = c; pshape = ptr; center = v;}
      vertex get_center(){return center; }
      void set_center(float xpos, float ypos){ 
         center.xpos = xpos; center.ypos = ypos;
      }  
      void draw() { pshape->draw (center, color); }
      void draw_border(int thickness, string c) {
         pshape->draw_border(center, thickness, c);
      }
      void draw_label(int label) { 
         pshape->draw_label(center, label);
      }
      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
      void set_border(bool set) {border = set;}
      bool get_border() {return border;}
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};


class window {
      friend class mouse;
   private:
      static int width;         // in pixels
      static int height;        // in pixels
      static int thickness;
      static string color;
      static int moveby;
      static vector<object> objects;
      static size_t selected_obj;
      static mouse mus;
      
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
   public:
      static void push_back (const object& obj) {
                  objects.push_back (obj); }
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static void setthickness (int thickness_) 
      {thickness = thickness_;}
      static void setcolor (string color_) {color = color_;}
      static void set_moveby(float value_) {moveby = value_;}
      static vector<object> get_objects() {return objects;}
      static void main();
};

#endif

