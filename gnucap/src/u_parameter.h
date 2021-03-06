/*$Id: u_parameter.h,v 26.125 2009/10/15 20:58:21 al Exp $ -*- C++ -*-
 * Copyright (C) 2005 Albert Davis
 * Author: Albert Davis <aldavis@gnu.org>
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *------------------------------------------------------------------
 * A class for parameterized values
 * Used for spice compatible .param statements
 * and passing arguments to models and subcircuits
 */
//testing=script 2006.07.14
#ifndef U_PARAMETER_H
#define U_PARAMETER_H
#include "globals.h"
#include "m_expression.h"
#include "u_opt.h"
#include "e_cardlist.h"
/*--------------------------------------------------------------------------*/
class LANGUAGE;
/*--------------------------------------------------------------------------*/
template <class T>
class PARAMETER {
private:
  mutable T _v;
  std::string _s;
public:
  explicit PARAMETER() :_v(NOT_INPUT), _s() {}
  PARAMETER(const PARAMETER<double>& p) :_v(p._v), _s(p._s) {}
  explicit PARAMETER(T v) :_v(v), _s() {}
  //explicit PARAMETER(T v, const std::string& s) :_v(v), _s(s) {untested();}
  ~PARAMETER() {}
  
  bool	has_hard_value()const {return (_s != "");}
  bool	has_good_value()const {return (_v != NOT_INPUT);}
  //bool has_soft_value()const {untested(); return (has_good_value() && !has_hard_value());}

  operator T()const {return _v;}
  T	e_val(const T& def, const CARD_LIST* scope)const;
  void	parse(CS& cmd);

  std::string string()const {
    if (_s == "#") {
      return to_string(_v);
    }else if (_s == "") {
      return "NA(" + to_string(_v) + ")";
    }else{
      return _s;
    }
  }
  void	print(OMSTREAM& o)const		{o << string();}
  void	set_default(const T& v)		{_v = v; _s = "";}
  void	operator=(const PARAMETER& p)	{_v = p._v; _s = p._s;}
  void	operator=(const T& v)		{_v = v; _s = "#";}
  //void	operator=(const std::string& s)	{untested();_s = s;}

  void	operator=(const std::string& s)	{
    if (strchr("'\"{", s[0])) {
      CS cmd(CS::_STRING, s);
      _s = cmd.ctos("", "'\"{", "'\"}");
    }else if (s == "NA") {
      _s = "";
    }else{
      _s = s;
    }
  }
  bool  operator==(const PARAMETER& p)const {
    return (_v == p._v  &&  _s == p._s);
  }
  bool  operator==(const T& v)const {
    if (v != NOT_INPUT) {
      return _v == v;
    }else{
      return (_v == NOT_INPUT || !has_hard_value());
    }
  }
  //bool	operator!=(const PARAMETER& p)const {untested();
  //  return !(*this == p);
  //}
  //bool	operator!=(const T& v)const {untested();
  //  return !(*this == v);
  //}
  T*	pointer_hack()	 {return &_v;}
private:
  T lookup_solve(const T& def, const CARD_LIST* scope)const;
};
/*--------------------------------------------------------------------------*/
/* non-class interface, so non-paramaters can have same syntax */
/* It is needed by the model compiler */
#if 0
inline bool operator==(const PARAMETER<int>& p, double v)
{untested();
  if (v != NOT_INPUT) {untested();
    return p.operator==(static_cast<int>(v));
  }else{untested();
    return (!(p.has_value()));
  }
}
#endif

template <class T>
bool has_hard_value(const PARAMETER<T>& p)
{
  return p.has_hard_value();
}

#if 0
inline bool has_hard_value(double x)
{untested();
  return (x != NOT_INPUT);
}
inline bool has_good_value(double x)
{untested();
  return (x != NOT_INPUT);
}
#endif

template <class T>
bool has_good_value(const PARAMETER<T>& p)
{
  return p.has_good_value();
}

#if 0
template <class T>
bool has_soft_value(const PARAMETER<T>& p)
{untested();
  return p.has_soft_value();
}
#endif

template <class T>
bool has_nz_value(const T& p)
{
  return (has_good_value(p) && p != 0);
}

template <class T>
void set_default(PARAMETER<T>* p, const T& v)
{
  assert(p);
  p->set_default(v);
}

template <class T>
void set_default(T* p, const T& v)
{
  assert(p);
  *p = v;
}

template <class T>
void e_val(PARAMETER<T>* p, const PARAMETER<T>& def, const CARD_LIST* scope)
{
  assert(p);
  p->e_val(def, scope);
}

template <class T>
void e_val(PARAMETER<T>* p, const T& def, const CARD_LIST* scope)
{
  assert(p);
  p->e_val(def, scope);
}

#if 0
template <class T>
void e_val(T* p, const T& def, const CARD_LIST*)
{untested();
  assert(p);
  if (*p == NOT_INPUT) {untested();
    *p = def;
  }else{untested();
  }
}
#endif
/*--------------------------------------------------------------------------*/
class INTERFACE PARAM_LIST {
private:
  mutable std::map<const std::string, PARAMETER<double> > _pl;
  std::map<const std::string, DISPATCHER<FUNCTION>::INSTALL *> _fl; // function intallers list
    // not going to use installers directly - just provide destruction mechanism (to be tested)
  std::map<const std::string, std::vector<std::string> > _fal;       // function argument list
  PARAM_LIST* _try_again; // if you don't find it, also look here
  CARD_LIST*  _upper_level; // _upper_level->params() is "second" _try_again - but _params is mutable at CARD_LIST (mutable PARAM_LIST* _params) so can not refer them directly
public:
  PARAM_LIST* get_try_again() {return _try_again;};
  PARAM_LIST* get_try_again()const {return _try_again;};

  CARD_LIST*  get_upper_level() {return _upper_level;};
  CARD_LIST*  get_upper_level()const {return _upper_level;};
  PARAM_LIST* get_upper_level_params() {return _upper_level?_upper_level->params():NULL;};
  PARAM_LIST* get_upper_level_params() const {return _upper_level?_upper_level->params():NULL;};
  
public:
  typedef std::map<const std::string, PARAMETER<double> >::const_iterator
		const_iterator;
  typedef std::map<const std::string, PARAMETER<double> >::iterator
		iterator;
  explicit PARAM_LIST() :_try_again(NULL),_upper_level(NULL) {}
  explicit PARAM_LIST(const PARAM_LIST& p) 
				:_pl(p._pl), _try_again(p._try_again), _fl(p._fl)/*?*/, _fal(p._fal), _upper_level(p._upper_level) {}    //[todo] GS fix copy constructor w.r.t. _fl
                                                                        // used in COMMON_SUBCKT:: copy constructor
  //explicit PARAM_LIST(PARAM_LIST* ta) :_try_again(ta) {untested();}
  ~PARAM_LIST() {}                                                      //[todo] GS fix destructor w.r.t. _fl
  void	parse(CS& cmd);
  void	print(OMSTREAM&, LANGUAGE*)const;
  void  deep_print(OMSTREAM& o, LANGUAGE*, int i=0) const;
  
  size_t size()const {return _pl.size();}
  bool	 is_empty()const {return _pl.empty();}
  bool	 is_printable(int)const;
  std::string name(int)const;
  std::string value(int)const;

  void	eval_copy(PARAM_LIST&, const CARD_LIST*);
  bool  operator==(const PARAM_LIST& p)const {return _pl == p._pl;}
  const PARAMETER<double>& deep_lookup(std::string)const;
  const PARAMETER<double>& operator[](std::string i)const {return deep_lookup(i);}
  void set(std::string, const std::string&);
  void set_try_again(PARAM_LIST* t) {_try_again = t;}
  void set_upper_level(CARD_LIST* t) {_upper_level = t;}

  iterator begin() {return _pl.begin();}
  iterator end() {return _pl.end();}
private:
  mutable int _index;
  mutable const_iterator _previous;
//GS
public:
  void addParameter(std::string s, PARAMETER<double> val){_pl[s] = val;} 
};
/*--------------------------------------------------------------------------*/
template <>
inline bool PARAMETER<bool>::lookup_solve(const bool&, const CARD_LIST*)const
{
  CS cmd(CS::_STRING, _s);
  return cmd.ctob();
}
/*--------------------------------------------------------------------------*/
std::string s_char_subst(std::string s, char a, char b);  // substitutes all occurances of a to b in s

template <class T>
inline T PARAMETER<T>::lookup_solve(const T& def, const CARD_LIST* scope)const
{
  CS cmd(CS::_STRING, s_char_subst(_s,'\'',' ') );   // GS - remove quotes from _s to handle expr like cos('a+b')
  Expression e(cmd);
  Expression reduced(e, scope);
  T v = T(reduced.eval());
  if (v != NOT_INPUT) {
    return v;
  }else{
    const PARAM_LIST* pl = scope->params();
    return T(pl->deep_lookup(_s).e_val(def, scope));
  } 
}
/*--------------------------------------------------------------------------*/
#if 0
template <class T>
inline T PARAMETER<T>::lookup_solve(const T& def, const CARD_LIST* scope)const
{
  const PARAM_LIST* pl = scope->params();
  return T(pl->deep_lookup(_s).e_val(def, scope));
}
#endif
/*--------------------------------------------------------------------------*/
template <class T>
T PARAMETER<T>::e_val(const T& def, const CARD_LIST* scope)const
{
  assert(scope);

  static int recursion=0;
  static const std::string* first_name = NULL;
  if (recursion == 0) {
    first_name = &_s;
  }else{
  }
  assert(first_name);
  
  ++recursion;
  if (_s == "") {
    // blank string means to use default value
    _v = def;
    if (recursion > 1) {
      error(bWARNING, "parameter " + *first_name + " has no value, default used\n");
    }else{
    }
  }else if (_s != "#") {
    // anything else means look up the value
    if (recursion <= OPT::recursion) {
      _v = lookup_solve(def, scope);
      if (_v == NOT_INPUT) {untested();itested();
	error(bDANGER, "parameter " + *first_name + " has no value\n");
      }else{
      }
    }else{untested();
      _v = def;
      error(bDANGER, "parameter " + *first_name + " recursion too deep\n");
    }
  }else{
    // start with # means we have a final value
  }
  --recursion;
  return _v;
}
/*--------------------------------------------------------------------------*/
template <>
inline void PARAMETER<bool>::parse(CS& cmd) 
{
  bool new_val;
  cmd >> new_val;
  if (cmd) {
    _v = new_val;
    _s = "#";
  }else{untested();
    std::string name;
    //cmd >> name;
    name = cmd.ctos(",=();", "'{\"", "'}\"");
    if (cmd) {untested();
      if (name == "NA") {untested();
	_s = "";
      }else{untested();
	_s = name;
      }
    }else{untested();
    }
  }
  if (!cmd) {untested();
    _v = true;
    _s = "#";
  }else{
  }
}
/*--------------------------------------------------------------------------*/
template <class T>
inline void PARAMETER<T>::parse(CS& cmd) 
{
  T new_val;
  cmd >> new_val;
  if (cmd) {
    _v = new_val;
    _s = "#";
  }else{
    std::string name;
    //cmd >> name;
    name = cmd.ctos(",=();", "'{\"", "'}\"");
    if (cmd) {
      if (cmd.match1('(')) {
	_s = name + '(' + cmd.ctos("", "(", ")") + ')';
      }else{
	_s = name;
      }
      if (name == "NA") {
        _s = "";
      }else{
      }
    }else{
    }
  }
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
INTERFACE bool Get(CS& cmd, const std::string& key, PARAMETER<bool>* val);
INTERFACE bool Get(CS& cmd, const std::string& key, PARAMETER<int>* val);
INTERFACE bool GetToken(CS& cmd, PARAMETER<int>* val);
INTERFACE bool GetToken(CS& cmd, PARAMETER<double>* val);
/*--------------------------------------------------------------------------*/
template <class T>
inline OMSTREAM& operator<<(OMSTREAM& o, const PARAMETER<T> p)
{
  p.print(o);
  return o;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
