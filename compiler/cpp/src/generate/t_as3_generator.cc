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

  string generate_makelist(t_list* tlist, string field_name);
  string type_name(t_type* ttype);
  std::string base_type_name(t_base_type* tbase);

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
}

void t_as3_generator::close_generator() {
  // Close package
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
  return "package {\n\n";
}

/**
 * Prints standard as3 imports
 *
 * @return List of imports for AS3 types that are used in here
 */
void t_as3_generator::generate_imports(ofstream &ostream) {
  indent(ostream) << "import flash.net.Responder;" << endl;
  indent(ostream) << "import flash.net.NetConnection;" << endl << endl;
}


/**
 * Generates a typedef. This is not done in AS3, since it does
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
  /*
  // Make output file
  string f_enum_name = package_dir_+"/"+(tenum->get_name())+".as";
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
*/
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
 * AS3 struct definition.
 *
 * @param tstruct The struct definition
 */
void t_as3_generator::generate_as3_struct(t_struct* tstruct,
                                            bool is_exception) {
  // Make output file
  std::ofstream struct_out;
  string outfile = package_dir_ + "/" + tstruct->get_name() + ".as";
  struct_out.open(outfile.c_str());

  struct_out << autogen_comment();

  indent(struct_out) << as3_package();

  indent_up();

  indent(struct_out) << "[Bindable]" << endl;
  indent(struct_out) << "dynamic public class " + tstruct->get_name() + " {" << endl;
  
  indent_up();
 
  const vector<t_field*>& members = tstruct->get_members();
  vector<t_field*>::const_iterator m_iter;
  for(m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
    indent(struct_out) << "public var " + (*m_iter)->get_name()  + ":" + type_name((*m_iter)->get_type()) + ";" << endl;
  }

  struct_out << endl;

  indent(struct_out) << "public function " + tstruct->get_name() + "(remote:Object = null) {" << endl;

  indent_up();
  indent(struct_out) << "if(remote != null) {" << endl;

  indent_up();

  // Create constructor to marshall remote objects.
  for(m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
    indent(struct_out) << (*m_iter)->get_name() << " = ";
    if((*m_iter)->get_type()->is_list()) {
      t_list* tlist = (t_list*) (*m_iter)->get_type();
      struct_out << generate_makelist(tlist, "remote." + (*m_iter)->get_name()) + ";" << endl;
    } else if(!((*m_iter)->get_type()->is_base_type())) {
      struct_out << "new " + type_name((*m_iter)->get_type()) + "(remote." + (*m_iter)->get_name() + ");" << endl;
    } else {
      struct_out << "remote." + (*m_iter)->get_name() + ";" << endl;
    }
  }

  indent_down();
  indent(struct_out) << "}" << endl;

  indent_down();
  indent(struct_out) << "}" << endl;

  indent_down();
  indent(struct_out) << "}" << endl;

  indent_down();
  indent(struct_out) << "}" << endl;

  struct_out.close();
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
  std::ofstream service_out;
  string outfile = package_dir_ + "/" + service_name_ + ".as";
  service_out.open(outfile.c_str());

  service_out << autogen_comment();
  service_out <<  as3_package();

  indent_up();

  generate_imports(service_out);

  indent(service_out) << "public class " << service_name_ << " {" << endl;
  indent_up();

  indent(service_out) << "private var connection:NetConnection;" << endl << endl;

  indent(service_out) << "public function " + service_name_ + "(conn:NetConnection) {" << endl;
  indent_up();

  indent(service_out) << "connection = conn;" << endl;

  indent_down();
  indent(service_out) << "}" << endl << endl;

  const vector<t_function *> functions = tservice->get_functions();
  for (vector<t_function *>::const_iterator f_iter = functions.begin();
       f_iter != functions.end();
       ++f_iter) {
    const t_function *func = *f_iter;

    indent(service_out) << "public function " << func->get_name() << "(";

    const vector<t_field *> &arg_fields = func->get_arglist()->get_members();
    bool first = true;
    for (vector<t_field *>::const_iterator a_iter = arg_fields.begin();
         a_iter != arg_fields.end();
         ++a_iter)
    {
      // comma separate args
      if (!first) {
        service_out << ", ";
      }
      first = false;

      const t_field *arg = *a_iter;
      t_type *type = get_true_type(arg->get_type());
      service_out << arg->get_name() << ":" << type_name(type);
    }

    // Add the onSuccess handler argument
    if (!first)
      service_out << ", ";

    service_out << "onSuccess:Function = null, onError:Function = null";

    t_type *ret_type = get_true_type(func->get_returntype());
    service_out << "):void {" << endl;
    indent_up();

    indent(service_out) << "var resp:Responder = new Responder(" << endl;
    indent_up();

    if(ret_type->is_list()) {
      indent(service_out) << "function(result:Array):void {" << endl;
    } else if(ret_type->is_void()) {
      indent(service_out) << "function():void {" << endl;
    } else {
      indent(service_out) << "function(result:Object):void {" << endl;
    }

    indent_up();
    indent(service_out) << "if(onSuccess != null) {" << endl;
    indent_up();
    indent(service_out) << "onSuccess(";

    if(ret_type->is_list()) {
      t_list* tlist = (t_list*) ret_type;
      service_out << generate_makelist(tlist, "result") + ");" << endl;
    } else if(ret_type->is_void()) {
      service_out << ");" << endl;
    } else {
      service_out << "new " << type_name(ret_type) << "(result));" << endl;
    }

    indent_down();
    indent(service_out) << "}" << endl;
    indent_down();
    indent(service_out) << "}," << endl;

    indent(service_out) << "function(error:Object):void {" << endl;
    indent_up();
    indent(service_out) << "if(onError != null) {" << endl;
    indent_up();
    indent(service_out) << "onError(error);" << endl;
    indent_down();
    indent(service_out) << "}" << endl;
    indent_down();
    indent(service_out) << "});" << endl;
    
    indent_down();
    indent(service_out) << "connection.call('" <<
      tservice->get_name() << "." << func->get_name() << "', " << 
      "resp, {";

    // Make hash of args
    first = true;
    for (vector<t_field *>::const_iterator a_iter = arg_fields.begin();
         a_iter != arg_fields.end();
         ++a_iter)
    {
      // comma separate args
      if (!first) {
        service_out << ", ";
      }
      first = false;

      const t_field *arg = *a_iter;
      service_out << arg->get_name() << ": " << arg->get_name();
    }
    service_out << "});" << endl;

    indent_down();
    indent(service_out) << "} // end " << func->get_name() << endl << endl;;
  }

  indent_down();
  indent(service_out) << "}" << endl;
  indent_down();
  indent(service_out) << "}" << endl;

  service_out.close();
}

string t_as3_generator::generate_makelist(t_list* tlist, string field_name) {
  string ret = "ThriftUtils.makeList(" + type_name(tlist->get_elem_type()) + ", ";
  if(tlist->get_elem_type()->is_list()) {
    ret += generate_makelist((t_list*) tlist->get_elem_type(), field_name);
  } else {
    ret += field_name;
  }

  ret += ")";

  return ret;
}

/**
 * Returns a AS3 type name
 *
 * @param ttype The type
 * @param container Is the type going inside a container?
 * @return AS3 type name, i.e. HashMap<Key,Value>
 */
string t_as3_generator::type_name(t_type* ttype) {
  // In AS3 typedefs are just resolved to their real type
  ttype = get_true_type(ttype);
  string prefix;

  if (ttype->is_base_type()) {
    return base_type_name((t_base_type*)ttype);
  } else if (ttype->is_enum()) {
    return "int";
  } else if (ttype->is_map()) {
    // TODO(atm): maybe this could be implemented with an associative array.
    return "AS3 DOESN'T HAVE MAPS";
  } else if (ttype->is_set()) {
    // TODO(atm): perhaps an arraycollection?
    return "AS3 DOESN'T HAVE SETS";
  } else if (ttype->is_list()) {
    return "Array";
  }
  
  //TODO(atm): What is this for?

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
 * @param container Is it going in a AS3 container?
 */
string t_as3_generator::base_type_name(t_base_type* type) {
  t_base_type::t_base tbase = type->get_base();

  switch (tbase) {
  case t_base_type::TYPE_VOID:
    return "void";
  case t_base_type::TYPE_STRING:
    if (type->is_binary()) {
      throw "compiler error: no AS 3 name for base type " + t_base_type::t_base_name(tbase);
    } else {
      return "String";
    }
  case t_base_type::TYPE_BOOL:
    return "Boolean";
  case t_base_type::TYPE_BYTE:
    return "uint";
  case t_base_type::TYPE_I16:
    return "int";
  case t_base_type::TYPE_I32:
    return "int";
  case t_base_type::TYPE_I64: // this isn't quite accurate, but we'll see how it goes.
  case t_base_type::TYPE_DOUBLE:
    return "Number";
  default:
    throw "compiler error: no AS 3 name for base type " + t_base_type::t_base_name(tbase);
  }
}


THRIFT_REGISTER_GENERATOR(as3, "AS3", "");

