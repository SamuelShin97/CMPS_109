// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height):
            polygon({}) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
   vertex v ({width/2, height/2});
   vertices.push_back(v);
   v = {width/2,-height/2};
   vertices.push_back(v);
   v = {-width/2, -height/2};
   vertices.push_back(v);
   v = {-width/2,height/2};
   vertices.push_back(v);
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (GLfloat width, GLfloat height): polygon({}){
   vertex v({0,height/2});
   vertices.push_back(v);
   v = {width/2, 0};
   vertices.push_back(v);
   v ={0,-height/2};
   vertices.push_back(v);
   v = {-width/2,0}; 
   vertices.push_back(v); 
}

triangle::triangle (const vertex_list& vertices): polygon(vertices) {

}

equilateral::equilateral (GLfloat width): triangle({}){
   vertex v({-width/2, -width/2});
   vertices.push_back(v);
   v = {0, width/2};
   vertices.push_back(v);
   v ={width/2, -width/2};
   vertices.push_back(v);

}

void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   string text = textdata;
   auto font = glut_bitmap_font; 
   glColor3ubv(color.ubvec);
   glRasterPos2f(center.xpos,center.ypos);
   for (auto ch: text) glutBitmapCharacter(font, ch);
}

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin(GL_POLYGON);
   glColor3ubv(color.ubvec);
   const float delta = 2 * M_PI / 32;
   float width = dimension.xpos / 3;
   float height = dimension.ypos / 3;
   int x = center.xpos;
   int y = center.ypos;
   
   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      float xpos = width * cos (theta) + x;
      float ypos = height * sin (theta) + y;
      glVertex2f (xpos, ypos);
   }
   glEnd();
}

void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin(GL_POLYGON);
   glColor3ubv(color.ubvec);
   auto begin = vertices.begin();
   auto end = vertices.end(); 
   
   while (begin != end)
   {
      glVertex2f (begin->xpos + center.xpos, begin->ypos + center.ypos);
      ++begin;
   }
   glEnd();
}

void text::draw_border (const vertex& center,
   int thickness, string color) const {
   glBegin(GL_POLYGON);
   rgbcolor str_color{color};
   glColor3ubv(str_color.ubvec);

   if (thickness <= 0) //if thickness is 0 or negative
   {
      //do nothing since there must be thickness
   }
   else{
      glVertex2f(center.xpos, center.ypos - 5);
      glVertex2f(center.xpos, center.ypos - 5 - thickness);
      glVertex2f(center.xpos * 10, center.ypos - 5 - thickness);
      glVertex2f(center.xpos * 10, center.ypos - 5);
   }
   glEnd();
}

void ellipse::draw_border (const vertex& center, 
   int thickness, string color) const {
   glBegin(GL_POLYGON);
   rgbcolor str_color{color};
   glColor3ubv(str_color.ubvec);
   const float delta = 2 * M_PI / 32;
   float width = dimension.xpos / 3 + thickness;
   float height = dimension.ypos / 3 + thickness;
   int x = center.xpos;
   int y = center.ypos;

   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      float xpos = width * cos (theta) + x;
      float ypos = height * sin (theta) + y;
      glVertex2f (xpos, ypos);
   }
   glEnd(); 
}

void polygon::draw_border (const vertex& center,
    int thickness, string color) const{
   glBegin(GL_POLYGON);
   rgbcolor str_color{color};
   glColor3ubv(str_color.ubvec);
   auto begin = vertices.begin();
   auto end = vertices.end();

   while (begin != end)
   {
      glVertex2f (begin->xpos + center.xpos + thickness, 
         begin->ypos + center.ypos + thickness);
      ++begin;
   }
   begin = vertices.begin();
   while (begin != end)
   {
      glVertex2f (begin->xpos + center.xpos + thickness,
         begin->ypos + center.ypos - thickness);
      ++begin;
   }
   
   begin = vertices.begin();
   while (begin != end)
   {
      glVertex2f (begin->xpos + center.xpos - thickness,
         begin->ypos + center.ypos - thickness);
      ++begin;
   }
   
   begin = vertices.begin();
   while (begin != end)
   {
      glVertex2f (begin->xpos + center.xpos - thickness,
         begin->ypos + center.ypos + thickness);
      ++begin;
   }
   
   glEnd();
}

void text::draw_label (const vertex& center, int label) const {

}

void ellipse::draw_label (const vertex& center, int label) const {
   string num = to_string(label);  
   auto font = GLUT_BITMAP_8_BY_13;
   rgbcolor col{"black"};
   glColor3ubv(col.ubvec);
   glRasterPos2f(center.xpos,center.ypos);
   for( auto ch: num) glutBitmapCharacter(font,ch);
}

void polygon::draw_label (const vertex& center, int label) const {
   string num = to_string(label);
   auto font = GLUT_BITMAP_8_BY_13;
   rgbcolor col{"black"};
   glColor3ubv(col.ubvec);
   glRasterPos2f(center.xpos,center.ypos);
   for( auto ch: num) glutBitmapCharacter(font,ch);
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

