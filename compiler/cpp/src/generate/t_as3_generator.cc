#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <sys/stat.h>

#include "platform.h"
#include "t_oop_generator.h"
using namespace std;

/**
 * Actionscript 3 code generator.
 */
class t_as3_generator : public t_oop_generator {
public:
  t_as3_generator(
    t_program *program,
    const std::map<std::string, std::string>& parsed_options,
    const std::string& option_string)
    : t_oop_generator(program)
  {
  }

  /**
   * Init and close methods
   */

  void init_generator();
  void close_generator();

  void generate_consts(std::vector<t_const*> consts);

  /**
   * Program-level generation functions
   */

  void generate_typedef (t_typedef*  ttypedef);
  void generate_enum    (t_enum*     tenum);
  void generate_struct  (t_struct*   tstruct);
  void generate_xception(t_struct*   txception);
  void generate_service (t_service*  tservice);  

private:
  std::ofstream f_out_;

  std::string package_dir_;
  std::string package_name_;

  void generate_imports(ofstream &ostream);
  void generate_as3_struct(t_struct* tstruct, bool is_exception);

  string type_name(t_type* ttype, bool in_container = false, bool in_init = false);
  std::string base_type_name(t_base_type* tbase, bool in_container=false);

  std::string as3_package();
};


void t_as3_generator::init_generator() {
  // Make output directory
  MKDIR(get_out_dir().c_str());
  package_name_ = program_->get_namespace("as3");

  string dir = package_name_;
  string subdir = get_out_dir();
  string::size_type loc;
  while ((loc = dir.find(".")) != string::npos) {
    subdir = subdir + "/" + dir.substr(0, loc);
    MKDIR(subdir.c_str());
    dir = dir.substr(loc+1);
  }
  if (dir.size() > 0) {
    subdir = subdir + "/" + dir;
    MKDIR(subdir.c_str());
  }

  package_dir_ = subdir;  

  string outfile = package_dir_ + "/" + program_->get_name();
  f_out_.open(outfile.c_str());

  f_out_ <<
    autogen_comment() <<
    as3_package();
  indent_up();
  generate_imports(f_out_);

}

void t_as3_generator::close_generator() {
  // Close package
  indent_down();
  indent(f_out_) << "}" << endl;
  f_out_.close();
}


/**
 * Packages the generated file
 *
 * @return String of the package, i.e. "package com.facebook.thriftdemo;"
 */
string t_as3_generator::as3_package() {
  if (!package_name_.empty()) {
    return string("package ") + package_name_ + " {\n\n";
  }
  return "";
}

/**
 * Prints standard as3 imports
 *
 * @return List of imports for As3 types that are used in here
 */
void t_as3_generator::generate_imports(ofstream &ostream) {
  indent(ostream) << "import flash.net.Responder;" << endl;
}


/**
 * Generates a typedef. This is not done in As3, since it does
 * not support arbitrary name replacements, and it'd be a wacky waste
 * of overhead to make wrapper classes.
 *
 * @param ttypedef The type definition
 */
void t_as3_generator::generate_typedef(t_typedef* ttypedef) {}

/**
 * Enums are a class with a set of static constants.
 *
 * @param tenum The enumeration
 */
void t_as3_generator::generate_enum(t_enum* tenum) {
  // Make output file
  string f_enum_name = package_dir_+"/"+(tenum->get_name())+".as3";
  ofstream f_enum;
  f_enum.open(f_enum_name.c_str());

  // Comment and package it
  f_enum <<
    autogen_comment() <<
    as3_package() << endl;

  f_enum <<
    "public class " << tenum->get_name() << " ";
  scope_up(f_enum);

  vector<t_enum_value*> constants = tenum->get_constants();
  vector<t_enum_value*>::iterator c_iter;
  int value = -1;
  for (c_iter = constants.begin(); c_iter != constants.end(); ++c_iter) {
    if ((*c_iter)->has_value()) {
      value = (*c_iter)->get_value();
    } else {
      ++value;
    }

    indent(f_enum) <<
      "public static final int " << (*c_iter)->get_name() <<
      " = " << value << ";" << endl;
  }

  scope_down(f_enum);
  f_enum.close();
}

/**
 * Generates a class that holds all the constants.
 */
void t_as3_generator::generate_consts(std::vector<t_const*> consts) {
  if (consts.empty()) {
    return;
  }

}


/**
 * Generates a struct definition for a thrift data type. This is a class
 * with data members, read(), write(), and an inner Isset class.
 *
 * @param tstruct The struct definition
 */
void t_as3_generator::generate_struct(t_struct* tstruct) {
  generate_as3_struct(tstruct, false);
}

/**
 * Exceptions are structs, but they inherit from Exception
 *
 * @param tstruct The struct definition
 */
void t_as3_generator::generate_xception(t_struct* txception) {
  generate_as3_struct(txception, true);
}


/**
 * As3 struct definition.
 *
 * @param tstruct The struct definition
 */
void t_as3_generator::generate_as3_struct(t_struct* tstruct,
                                            bool is_exception) {

}

/**
 * Generates a thrift service. In C++, this comprises an entirely separate
 * header and source file. The header file defines the methods and includes
 * the data types defined in the main header file, and the implementation
 * file contains implementations of the basic printer and default interfaces.
 *
 * @param tservice The service definition
 */
void t_as3_generator::generate_service(t_service* tservice) {
  // Make output file

  indent(f_out_) <<
    "public class " << service_name_ << " {" << endl;
  indent_up();

  const vector<t_function *> functions = tservice->get_functions();
  for (vector<t_function *>::const_iterator f_iter = functions.begin();
       f_iter != functions.end();
       ++f_iter) {
    const t_function *func = *f_iter;

    indent(f_out_) << "public function " << func->get_name() << "(";

    const vector<t_field *> &arg_fields = func->get_arglist()->get_members();
    bool first = true;
    for (vector<t_field *>::const_iterator a_iter = arg_fields.begin();
         a_iter != arg_fields.end();
         ++a_iter)
    {
      // comma separate args
      if (!first) {
        f_out_ << ", ";
      }
      first = false;

      const t_field *arg = *a_iter;
      t_type *type = get_true_type(arg->get_type());
      f_out_ << arg->get_name() << ":" << type_name(type);
    }

    // Add the onSuccess handler argument
    if (!first) f_out_ << ", ";
    f_out_ << "onSuccess:Function";

    t_type *ret_type = get_true_type(func->get_returntype());
    f_out_ << ") : " << type_name(ret_type) << " {" << endl;
    indent_up();

    indent(f_out_) << "var resp:Responder = new Responder(" << endl;
    indent(f_out_) << "  function(result:Object) : void { " << endl;
    indent(f_out_) << "    onSuccess(new " << type_name(ret_type) << "(result));" << endl;
    indent(f_out_) << "  }" << endl;
    indent(f_out_) << ");" << endl;
    indent(f_out_) << "this.getConnection().call('" <<
      tservice->get_name() << "." << func->get_name() << "'," << 
      "resp, {" << endl;
    indent_up();

    // Make hash of args
    first = true;
    for (vector<t_field *>::const_iterator a_iter = arg_fields.begin();
         a_iter != arg_fields.end();
         ++a_iter)
    {
      // comma separate args
      if (!first) {
        f_out_ << ", " << endl;
      }
      first = false;

      const t_field *arg = *a_iter;
      indent(f_out_) << arg->get_name() << ":" << arg->get_name();
    }
    indent_down();
    indent(f_out_) << endl;
    indent(f_out_) << "});" << endl;

    indent_down();
    indent(f_out_) << "} // end " << func->get_name() << endl << endl;;
  }

  indent_down();
  indent(f_out_) <<
    "}" << endl;
}


/**
 * Returns a As3 type name
 *
 * @param ttype The type
 * @param container Is the type going inside a container?
 * @return As3 type name, i.e. HashMap<Key,Value>
 */
string t_as3_generator::type_name(t_type* ttype, bool in_container, bool in_init) {
  // In As3 typedefs are just resolved to their real type
  ttype = get_true_type(ttype);
  string prefix;

  if (ttype->is_base_type()) {
    return base_type_name((t_base_type*)ttype, in_container);
  } else if (ttype->is_enum()) {
    return (in_container ? "Integer" : "int");
  } else if (ttype->is_map()) {
    t_map* tmap = (t_map*) ttype;
    if (in_init) {
      prefix = "HashMap";
    } else {
      prefix = "Map";
    }
    return prefix + "<" +
      type_name(tmap->get_key_type(), true) + "," +
      type_name(tmap->get_val_type(), true) + ">";
  } else if (ttype->is_set()) {
    t_set* tset = (t_set*) ttype;
    if (in_init) {
      prefix = "HashSet<";
    } else {
      prefix = "Set<";
    }
    return prefix + type_name(tset->get_elem_type(), true) + ">";
  } else if (ttype->is_list()) {
    t_list* tlist = (t_list*) ttype;
    if (in_init) {
      prefix = "ArrayList<";
    } else {
      prefix = "List<";
    }
    return prefix + type_name(tlist->get_elem_type(), true) + ">";
  }

  // Check for namespacing
  t_program* program = ttype->get_program();
  if (program != NULL && program != program_) {
    string package = program->get_namespace("as3");
    if (!package.empty()) {
      return package + "." + ttype->get_name();
    }
  }

  return ttype->get_name();
}

/**
 * Returns the C++ type that corresponds to the thrift type.
 *
 * @param tbase The base type
 * @param container Is it going in a As3 container?
 */
string t_as3_generator::base_type_name(t_base_type* type,
                                        bool in_container) {
  t_base_type::t_base tbase = type->get_base();

  switch (tbase) {
  case t_base_type::TYPE_VOID:
    return "void";
  case t_base_type::TYPE_STRING:
    if (type->is_binary()) {
      return "byte[]";
    } else {
      return "String";
    }
  case t_base_type::TYPE_BOOL:
    return (in_container ? "Boolean" : "boolean");
  case t_base_type::TYPE_BYTE:
    return (in_container ? "Byte" : "byte");
  case t_base_type::TYPE_I16:
    return (in_container ? "Short" : "short");
  case t_base_type::TYPE_I32:
    return (in_container ? "Integer" : "int");
  case t_base_type::TYPE_I64:
    return (in_container ? "Long" : "long");
  case t_base_type::TYPE_DOUBLE:
    return (in_container ? "Double" : "double");
  default:
    throw "compiler error: no C++ name for base type " + t_base_type::t_base_name(tbase);
  }
}


THRIFT_REGISTER_GENERATOR(as3, "As3", "");

