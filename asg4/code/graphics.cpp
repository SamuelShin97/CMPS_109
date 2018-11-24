// $Id: graphics.cpp,v 1.3 2016-07-20 21:33:16-07 - - $

#include <iostream>
using namespace std;

#include <GL/freeglut.h>

#include "graphics.h"
#include "util.h"

int window::width = 640; // in pixels
int window::height = 480; // in pixels
int window::thickness = 4;
string window::color = "red";
int window::moveby = 4;
vector<object> window::objects;
size_t window::selected_obj = 0;
mouse window::mus;

// Executed when window system signals to shut down.
void window::close() {
   DEBUGF ('g', sys_info::execname() << ": exit ("
           << sys_info::exit_status() << ")");
   exit (sys_info::exit_status());
}

// Executed when mouse enters or leaves window.
void window::entry (int mouse_entered) {
   DEBUGF ('g', "mouse_entered=" << mouse_entered);
   window::mus.entered = mouse_entered;
   if (window::mus.entered == GLUT_ENTERED) {
      DEBUGF ('g', sys_info::execname() << ": width=" << window::width
           << ", height=" << window::height);
      window::display();
   }
   glutPostRedisplay();
}

bool first_obj = true;
// Called to display the objects in the window.
void window::display() {
   glClear (GL_COLOR_BUFFER_BIT);
   if (first_obj == true)
   {
      window::objects[0].set_border(true);
      first_obj = false;
   }
   int label = 0;
   for (auto& object: window::objects){
      if (object.get_border() == true)
      {
         object.draw_border(window::thickness, window::color);
      }
      object.draw();
      if (object.get_border() == true)
      {
         object.draw_label(label);
      }
      ++label;
   }
   mus.draw();
   glutSwapBuffers();
}

// Called when window is opened and when resized.
void window::reshape (int width, int height) {
   DEBUGF ('g', "width=" << width << ", height=" << height);
   window::width = width;
   window::height = height;
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (0, window::width, 0, window::height);
   glMatrixMode (GL_MODELVIEW);
   glViewport (0, 0, window::width, window::height);
   glClearColor (0.25, 0.25, 0.25, 1.0);
   glutPostRedisplay();
}



// Executed when a regular keyboard key is pressed.
void window::keyboard (GLubyte key, int x, int y) {
   enum {BS = 8, TAB = 9, ESC = 27, SPACE = 32, DEL = 127};
   DEBUGF ('g', "key=" << unsigned (key) << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case 'Q': case 'q': case ESC:
         window::close();
         break;
      case 'H': case 'h':
         //move_selected_object (
         window::objects[selected_obj].move(-moveby,0);
         if(window::objects[selected_obj].get_center().xpos < 0)
         {
          window::objects[selected_obj].set_center(window::width,
                   window::objects[selected_obj].get_center().ypos);
         }
         break;
      case 'J': case 'j':
         //move_selected_object (
         window::objects[selected_obj].move(0,-moveby);
         if(window::objects[selected_obj].get_center().ypos < 0)
         {
            window::objects[selected_obj].set_center(
                  window::objects[selected_obj].get_center().xpos,
                  window::height);
         }
         break;
      case 'K': case 'k':
         //move_selected_object (
         window::objects[selected_obj].move(0,moveby);
         if(window::objects[selected_obj].get_center().ypos > 
                  window::height)
         {
          window::objects[selected_obj].set_center(
                 window::objects[selected_obj].get_center().xpos,0);
         }
         break;
      case 'L': case 'l':
         //move_selected_object (
         window::objects[selected_obj].move(moveby,0);
         if(window::objects[selected_obj].get_center().xpos >
                 window::width)
         {
          window::objects[selected_obj].set_center(0,
               window::objects[selected_obj].get_center().ypos);
         }
         break;
      case 'N': case 'n': case SPACE: case TAB:
         for (unsigned int i = 0; i < window::objects.size(); ++i)
         {
            //sets all border fields to false
            objects[i].set_border(false);
         } 
         if(selected_obj == window::objects.size() - 1)
         {
            selected_obj = 0;
         }
         else{
           ++selected_obj;
         } 
         objects[selected_obj].set_border(true);
         break;
      case 'P': case 'p': case BS:
         for (unsigned int i = 0; i < window::objects.size(); ++i)
         {
            //sets all border fields to false
            objects[i].set_border(false);
         }
         if(selected_obj == 0)
         {
            selected_obj = window::objects.size()-1;
         }
         else{
            --selected_obj;
         }
         objects[selected_obj].set_border(true);
         break;
      case '0'...'9':
         for (unsigned int i = 0; i < window::objects.size(); ++i)
         {
            //sets all border fields to false
            objects[i].set_border(false);
         }

         if (key == '0' && 0 <= window::objects.size())
         {
            selected_obj = 0;
         }
         else if (key == '1' && 1 <= window::objects.size())
         {
            selected_obj = 1;
         }
         else if (key == '2' && 2 <= window::objects.size())
         {
            selected_obj = 2;
         }
         else if (key == '3' && 3 <= window::objects.size())
         {
            selected_obj = 3;
         }
         else if (key == '4' && 4 <= window::objects.size())
         {
            selected_obj = 4;
         }
         else if (key == '5' && 5 <= window::objects.size())
         {
            selected_obj = 5;
         }
         else if (key == '6' && 6 <= window::objects.size())
         {
            selected_obj = 6;
         }
         else if (key == '7' && 7 <= window::objects.size())
         {
            selected_obj = 7;
         }
         else if (key == '8' && 8 <= window::objects.size())
         {
            selected_obj = 8;
         }
         else if (key == '9' && 9 <= window::objects.size())
         {
            selected_obj = 9;
         }
         objects[selected_obj].set_border(true);
         break;
      default:
         cerr << unsigned (key) << ": invalid keystroke" << endl;
         break;
   }
   glutPostRedisplay();
}


// Executed when a special function key is pressed.
void window::special (int key, int x, int y) {
   DEBUGF ('g', "key=" << key << ", x=" << x << ", y=" << y);
   window::mus.set (x, y);
   switch (key) {
      case GLUT_KEY_LEFT: keyboard('h', x, y); break;
      case GLUT_KEY_DOWN: keyboard('j', x, y); break;
      case GLUT_KEY_UP:   keyboard('k', x, y); break;
      case GLUT_KEY_RIGHT:keyboard('l', x, y); break;
      case GLUT_KEY_F1: keyboard('1', x, y); break;
      case GLUT_KEY_F2: keyboard('2', x, y); break;
      case GLUT_KEY_F3: keyboard('3', x, y); break;
      case GLUT_KEY_F4: keyboard('4', x, y); break;
      case GLUT_KEY_F5: keyboard('5', x, y); break;
      case GLUT_KEY_F6: keyboard('6', x, y); break;
      case GLUT_KEY_F7: keyboard('7', x, y); break;
      case GLUT_KEY_F8: keyboard('8', x, y); break;
      case GLUT_KEY_F9: keyboard('9', x, y); break;
      case GLUT_KEY_F10: keyboard('9', x, y); 
                         if (window::objects.size() >= 10) {
                            keyboard('n', x, y);
                         }  
                         break;
      case GLUT_KEY_F11: keyboard('9', x, y);
                         if (window::objects.size() >= 11) {
                            keyboard('n', x, y);
                            keyboard('n', x, y);
                         }
                         break;
      case GLUT_KEY_F12: keyboard('9', x, y);
                         if (window::objects.size() >= 12) {
                            keyboard('n', x, y);
                            keyboard('n', x, y);
                            keyboard('n', x, y);
                         }
                         break;
      default:
         cerr << unsigned (key) << ": invalid function key" << endl;
         break;
   }
   glutPostRedisplay();
}


void window::motion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::passivemotion (int x, int y) {
   DEBUGF ('g', "x=" << x << ", y=" << y);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::mousefn (int button, int state, int x, int y) {
   DEBUGF ('g', "button=" << button << ", state=" << state
           << ", x=" << x << ", y=" << y);
   window::mus.state (button, state);
   window::mus.set (x, y);
   glutPostRedisplay();
}

void window::main () {
   static int argc = 0;
   glutInit (&argc, nullptr);
   glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize (window::width, window::height);
   glutInitWindowPosition (128, 128);
   glutCreateWindow (sys_info::execname().c_str());
   glutCloseFunc (window::close);
   glutEntryFunc (window::entry);
   glutDisplayFunc (window::display);
   glutReshapeFunc (window::reshape);
   glutKeyboardFunc (window::keyboard);
   glutSpecialFunc (window::special);
   glutMotionFunc (window::motion);
   glutPassiveMotionFunc (window::passivemotion);
   glutMouseFunc (window::mousefn);
   DEBUGF ('g', "Calling glutMainLoop()");
   glutMainLoop();
}


void mouse::state (int button, int state) {
   switch (button) {
      case GLUT_LEFT_BUTTON: left_state = state; break;
      case GLUT_MIDDLE_BUTTON: middle_state = state; break;
      case GLUT_RIGHT_BUTTON: right_state = state; break;
   }
}

void mouse::draw() {
   static rgbcolor color ("green");
   ostringstream text;
   text << "(" << xpos << "," << window::height - ypos << ")";
   if (left_state == GLUT_DOWN) text << "L"; 
   if (middle_state == GLUT_DOWN) text << "M"; 
   if (right_state == GLUT_DOWN) text << "R"; 
   if (entered == GLUT_ENTERED) {
      void* font = GLUT_BITMAP_HELVETICA_18;
      glColor3ubv (color.ubvec);
      glRasterPos2i (10, 10);
      auto ubytes = reinterpret_cast<const GLubyte*>
                    (text.str().c_str());
      glutBitmapString (font, ubytes);
   }
}


