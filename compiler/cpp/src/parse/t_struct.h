// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef T_STRUCT_H
#define T_STRUCT_H

#include <algorithm>
#include <vector>
#include <utility>
#include <string>

#include "t_type.h"
#include "t_field.h"

// Forward declare that puppy
class t_program;

typedef std::vector<t_field*> members_type;
/**
 * A struct is a container for a set of member fields that has a name. Structs
 * are also used to implement exception types.
 *
 */
class t_struct : public t_type {
 public:
  t_struct(t_program* program) :
    t_type(program),
    is_xception_(false),
    xsd_all_(false) {}

  t_struct(t_program* program, const std::string& name) :
    t_type(program, name),
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

  bool append(t_field* elem) {
    typedef members_type::iterator iter_type;
    std::pair<iter_type, iter_type> bounds = std::equal_range(
            members_.begin(), members_.end(), elem, t_field::key_compare()
        );
    if (bounds.first != bounds.second)
        return false;
    members_.insert(bounds.second, elem);
    return true;
  }

  const members_type& get_members() {
    return members_;
  }

  bool is_struct() const {
    return !is_xception_;
  }

  bool is_xception() const {
    return is_xception_;
  }

  virtual std::string get_fingerprint_material() const {
    std::string rv = "{";
    members_type::const_iterator m_iter;
    for (m_iter = members_.begin(); m_iter != members_.end(); ++m_iter) {
      rv += (*m_iter)->get_fingerprint_material();
      rv += ";";
    }
    rv += "}";
    return rv;
  }

  virtual void generate_fingerprint() {
    t_type::generate_fingerprint();
    members_type::const_iterator m_iter;
    for (m_iter = members_.begin(); m_iter != members_.end(); ++m_iter) {
      (*m_iter)->get_type()->generate_fingerprint();
    }
  }

 private:

  members_type members_;

  /**
   * Fields should be sorted by tag order. We store if fields sorted 
   * to avoid sort on every call of get_members or append routines.
   */
  void sort_members() {
    std::sort(members_.begin(), members_.end(), t_field::key_compare());
  }

  bool is_xception_;

  bool xsd_all_;
};

#endif
