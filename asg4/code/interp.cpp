// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

#include <memory>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"border" , &interpreter::do_border },
   {"moveby" , &interpreter::do_moveby },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"diamond"  , &interpreter::make_diamond  },
   {"triangle" , &interpreter::make_triangle },
   {"equilateral" , &interpreter::make_equilateral},
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   rgbcolor rgb {begin[0]};
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   object obj(rgb, itor->second, where);
   window::push_back(obj);
}

void interpreter::do_border (param begin, param end){
   DEBUGF ('f', range (begin, end));
   string color = *begin++;
   string::size_type sz;
   int thickness = stof(*begin, &sz);
   window::setthickness(thickness);
   window::setcolor(color); 
}

void interpreter::do_moveby (param begin, param end){
   DEBUGF ('f', range (begin, end));
   string::size_type sz;
   float value = stof(*begin++,&sz);
   window::set_moveby(value); 
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   auto font = fontcode.find(*begin)->second;
   string finishedString;
   ++begin;
   while (begin != end)
   {
      finishedString += *begin + " ";
      ++begin;
   }
   return make_shared<text> (font, finishedString);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string::size_type sz;
   string::size_type sz2;
 
   float width = stof(*begin++, &sz);
   float height= stof(*begin, &sz2);
   return make_shared<ellipse> (width, height);
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string::size_type sz;
   float diam = stof(*begin++, &sz);
   return make_shared<circle> (diam);
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   vertex_list v; 

   string::size_type sz; 
   string::size_type sz2;

   while(begin != end){
      float x = stof(*begin++,&sz);
      if(begin != end){ 
         float y  = stof(*begin++,&sz2);
         vertex vert({x, y});
         v.push_back(vert); 
      } 
   } 
   vertex_list avg; 
   float x_hat = 0; 
   float y_hat = 0;
   int count = 0;
   auto beg = v.begin();
   auto en = v.end();
   while(beg != en) {
      x_hat = x_hat + beg->xpos; 
      y_hat = y_hat + beg->ypos; 
      count++;  
      ++beg;        
   }  
   vertex new_pos({x_hat/count,y_hat/count}) ;
   beg = v.begin();
   en =   v.end();
   while(beg != en){ 
      float new_x = beg->xpos - new_pos.xpos;
      float new_y = beg->ypos - new_pos.ypos;
      vertex center({new_x,new_y});
      avg.push_back(center);
      ++beg;
   }   

   return make_shared<polygon> (avg); 
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string:: size_type sz;
   string:: size_type sz2;
   float width = stof(*begin++, &sz);
   float height = stof(*begin, &sz);
   return make_shared<rectangle> (width, height);
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string:: size_type sz;
   float width = stof(*begin++, &sz);
   return make_shared<square> (width);
}

shape_ptr interpreter::make_diamond (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string:: size_type sz;
   string:: size_type sz2;
   float width = stof(*begin++, &sz);
   float height = stof(*begin, &sz2);
   return make_shared<diamond> (width, height);
}

shape_ptr interpreter::make_triangle (param begin, param end){
   DEBUGF ('f', range (begin, end));
   vertex_list v;

   string::size_type sz;
   string::size_type sz2;

   while(begin != end){
      float x = stof(*begin++,&sz);
      if(begin != end){
         float y  = stof(*begin++,&sz2);
         vertex vert({x, y});
         v.push_back(vert);
      }
   }
   vertex_list avg;
   float x_hat = 0;
   float y_hat = 0;
   int count = 0;
   auto beg = v.begin();
   auto en = v.end();
   while(beg != en) {
      x_hat = x_hat + beg->xpos;
      y_hat = y_hat + beg->ypos;
      count++;
      ++beg;
   }
   vertex new_pos({x_hat/count,y_hat/count}) ;
   beg = v.begin();
   en =   v.end();
   while(beg != en){
      float new_x = beg->xpos - new_pos.xpos;
      float new_y = beg->ypos - new_pos.ypos;
      vertex center({new_x,new_y});
      avg.push_back(center);
      ++beg;
   }

   return make_shared<triangle> (avg);
}

shape_ptr interpreter::make_equilateral (param begin, param end){
   string:: size_type sz;
   float width = stof(*begin++, &sz);
   return make_shared<equilateral> (width);
}
