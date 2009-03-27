// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef T_STRUCT_H
#define T_STRUCT_H

#include <algorithm>
#include <vector>
#include <string>

#include "t_type.h"
#include "t_field.h"

// Forward declare that puppy
class t_program;

/**
 * A struct is a container for a set of member fields that has a name. Structs
 * are also used to implement exception types.
 *
 */
class t_struct : public t_type {
 public:
  t_struct(t_program* program) :
    t_type(program),
    sorted_(false),
    is_xception_(false),
    xsd_all_(false) {}

  t_struct(t_program* program, const std::string& name) :
    t_type(program, name),
    sorted_(false),
    is_xception_(false),
    xsd_all_(false) {}

  void set_name(const std::string& name) {
    name_ = name;
  }

  void set_xception(bool is_xception) {
    is_xception_ = is_xception;
  }

  void set_xsd_all(bool xsd_all) {
    xsd_all_ = xsd_all;
  }

  bool get_xsd_all() const {
    return xsd_all_;
  }

  void append(t_field* elem) {
    members_.push_back(elem);
    sorted_ = false;
  }

  const std::vector<t_field*>& get_members() {
    sort_members();
    return members_;
  }

  bool is_struct() const {
    return !is_xception_;
  }

  bool is_xception() const {
    return is_xception_;
  }

  virtual std::string get_fingerprint_material() const {
    sort_members();
    std::string rv = "{";
    std::vector<t_field*>::const_iterator m_iter;
    for (m_iter = members_.begin(); m_iter != members_.end(); ++m_iter) {
      rv += (*m_iter)->get_fingerprint_material();
      rv += ";";
    }
    rv += "}";
    return rv;
  }

  virtual void generate_fingerprint() {
    sort_members();
    t_type::generate_fingerprint();
    std::vector<t_field*>::const_iterator m_iter;
    for (m_iter = members_.begin(); m_iter != members_.end(); ++m_iter) {
      (*m_iter)->get_type()->generate_fingerprint();
    }
  }

  bool validate_field(t_field* field) {
    sort_members();
    int key = field->get_key();
    std::vector<t_field*>::const_iterator m_iter;
    for (m_iter = members_.begin(); m_iter != members_.end(); ++m_iter) {
      if ((*m_iter)->get_key() == key) {
        return false;
      }
    }
    return true;
  }

 private:

  std::vector<t_field*> members_;
  bool sorted_;

  /**
   * Fields should be sorted by tag order. We store if fields sorted 
   * to avoid sort on every call of get_members or append routines.
   */
  void sort_members() {
    if (!sorted_) {
      std::sort(members_.begin(), members_.end(), t_field::key_compare());
      sorted_ = true;
    }
  }

  bool is_xception_;

  bool xsd_all_;
};

#endif
