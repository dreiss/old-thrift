// Copyright (c) 2007- RM
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#include <cassert>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <sys/stat.h>
#include <ctype.h>

#include "platform.h"
#include "t_oop_generator.h"

using namespace std;


/**
 * C++ code generator. This is legitimacy incarnate.
 *
 * @author Mark Slee <mcslee@facebook.com>
 */
class t_c_generator : public t_oop_generator {
 public:
  t_c_generator(
      t_program* program,
      const map<string, string>& parsed_options,
      const string& option_string)
    : t_oop_generator(program)
  {
    this->out_dir_base_ = "gen-c";

    this->nspace = program_->get_namespace("c");
    if (this->nspace.empty())
      this->nspace = "Thrift";
    else
      this->nspace = "Thrift." + this->nspace;

    char * tmp = strdup(this->nspace.c_str());
    for (unsigned int i = 0; i < strlen(tmp); i++)
    {
      if (tmp[i] == '.')
        tmp[i] = '_';
      tmp[i] = tolower(tmp[i]);
    }
    this->nspace_u = tmp;
    this->nspace_u += "_";

    for (unsigned int i = 0; i < strlen(tmp); i++)
      tmp[i] = toupper(tmp[i]);
    this->nspace_uc = tmp;
    this->nspace_uc += "_";

    free(tmp);

    tmp = strdup(this->nspace.c_str());
    int pos = 0;
    for (unsigned int i = 0; i < strlen(tmp); i++)
    {
      if (tmp[i] != '.')
      {
        tmp[pos] = tmp[i];
        pos++;
      }
    }
    this->nspace = string(tmp, pos);
    free(tmp);
  }

  /**
   * Init and close methods
   */

  void init_generator();
  void close_generator();

  /**
   * Program-level generation functions
   */

  void generate_typedef(t_typedef* ttypedef);
  void generate_enum(t_enum* tenum);
  void generate_consts(vector<t_const*> consts);
  void generate_struct(t_struct* tstruct);
  void generate_service(t_service* tservice);
  void generate_xception (t_struct* txception);

 private:

  /**
   * File streams, stored here to avoid passing them as parameters to every
   * function.
   */

  ofstream f_types_;
  ofstream f_types_impl_;
  string nspace;
  string nspace_u;
  string nspace_uc;

  /** 
   * helper functions
   */

  void generate_object(t_struct* tstruct);
  string type_name(t_type* ttype);
  string base_type_name(t_base_type::t_base tbase);
  string constant_value(t_type * type, t_const_value * value);
};


/**
 * Prepares for file generation by opening up the necessary file output
 * streams.
 *
 * @param tprogram The program to generate
 */
void t_c_generator::init_generator() {
  // Make output directory
  MKDIR(get_out_dir().c_str());

  // Make output file
  string f_types_name = get_out_dir()+program_name_+"_types.h";
  f_types_.open(f_types_name.c_str());

  string f_types_impl_name = get_out_dir()+program_name_+"_types.c";
  f_types_impl_.open(f_types_impl_name.c_str());

  // Print header
  f_types_ <<
    autogen_comment();
  f_types_impl_ <<
    autogen_comment();

  // Start ifndef
  f_types_ <<
    "#ifndef " << program_name_ << "_TYPES_H" << endl <<
    "#define " << program_name_ << "_TYPES_H" << endl <<
    endl;

  // Include base types
  f_types_ <<
    "/* base includes */" << endl <<
    "#include <stdint.h>" << endl <<
    "#include <glib-object.h>" << endl <<
    endl;

  // Include other Thrift includes
  const vector<t_program*>& includes = program_->get_includes();
  for (size_t i = 0; i < includes.size(); ++i) {
    f_types_ <<
      "/* other thrift includes */" << endl <<
      "#include \"" << includes[i]->get_name() << "_types.h\"" << endl;
  }
  f_types_ << endl;

  // Include custom headers
  const vector<string>& c_includes = program_->get_c_includes();
  f_types_ << "/* custom thrift includes */" << endl;
  for (size_t i = 0; i < c_includes.size(); ++i) {
    if (c_includes[i][0] == '<') {
      f_types_ <<
        "#include " << c_includes[i] << endl;
    } else {
      f_types_ <<
        "#include \"" << c_includes[i] << "\"" << endl;
    }
  }
  f_types_ <<
    endl;

  // Include the types file
  f_types_impl_ <<
    endl <<
    "#include \"" << program_name_ << "_types.h\"" << endl <<
    endl;

  f_types_ << 
    "/* begin types */" << endl << endl;
}

/**
 * Closes the output files.
 */
void t_c_generator::close_generator() {

  // Close ifndef
  f_types_ <<
    "#endif /* " << program_name_ << "_TYPES_H */" << endl;

  // Close output file
  f_types_.close();
  f_types_impl_.close();
}

/**
 * Generates a typedef. This is just a simple 1-liner in C++
 *
 * @param ttypedef The type definition
 */
void t_c_generator::generate_typedef(t_typedef* ttypedef) {
  f_types_ <<
    indent() << "typedef " << type_name(ttypedef->get_type()) << " " << 
    ttypedef->get_symbolic() << ";" << endl << endl;
}

/**
 * Generates code for an enumerated type. In C++, this is essentially the same
 * as the thrift definition itself, using the enum keyword in C++.
 *
 * @param tenum The enumeration
 */
void t_c_generator::generate_enum(t_enum* tenum) {
  // TODO: look in to using glib's enum facilities
  f_types_ <<
    indent() << "typedef enum _" << tenum->get_name() << " " << tenum->get_name() << ";" << endl <<
    indent() << "enum _" << tenum->get_name() << " {" << endl;
    
  indent_up();

  vector<t_enum_value*> constants = tenum->get_constants();
  vector<t_enum_value*>::iterator c_iter;
  bool first = true;
  for (c_iter = constants.begin(); c_iter != constants.end(); ++c_iter) {
    if (first) {
      first = false;
    } else {
      f_types_ << "," << endl;
    }
    f_types_ <<
      indent() << (*c_iter)->get_name();
    if ((*c_iter)->has_value()) {
      f_types_ <<
        " = " << (*c_iter)->get_value();
    }
  }

  indent_down();
  f_types_ <<
    endl <<
    "};" << endl <<
    endl;
}
  
void t_c_generator::generate_struct(t_struct* tstruct)
{
  f_types_ << "/* struct */" << endl;
  generate_object(tstruct);
}

void t_c_generator::generate_xception(t_struct* tstruct)
{
  // TODO: look in to/think about making these gerror's
  f_types_ << "/* exception */" << endl;
  generate_object(tstruct);
}

void t_c_generator::generate_object(t_struct* tstruct)
{
  string name = tstruct->get_name();

  char buf[256];
  const char * tmp;

  tmp = name.c_str();
  int pos = 0;
  buf[pos] = tolower(tmp[pos]);
  pos++;
  for (unsigned int i = pos; i < name.length(); i++)
  {
    buf[pos] = tolower(tmp[i]);
    if (buf[pos] != tmp[i])
    {
      // we just lowercase'd it
      buf[pos+1] = buf[pos];
      buf[pos] = '_';
      pos++;
    }
    pos++;
  }
  buf[pos] = '\0';
  string name_u = buf;
  name_u += "_";

  for (unsigned int i = 0; i < strlen(buf); i++)
    buf[i] = toupper(buf[i]);
  string name_uc = buf;
  name_uc += "_";

  f_types_ << 
    "typedef struct _" << this->nspace << name << " " << this->nspace << name << ";" << endl <<
    "struct _" << this->nspace << name << endl <<
    "{ " << endl <<
    "    GObject parent; " << endl;

  vector<t_field*>::const_iterator m_iter;
  const vector<t_field*>& members = tstruct->get_members();
  for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
    t_type* t = get_true_type((*m_iter)->get_type());
    f_types_ << "    " << type_name(t) << " " << (*m_iter)->get_name() << ";" << endl;
  }

  f_types_ << 
    "}; " << endl <<
    "typedef struct _" << this->nspace << name << "Class " << this->nspace << name << "Class;" << endl <<
    "struct _" << this->nspace << name << "Class" << endl <<
    "{ " << endl <<
    "    GObjectClass parent; " << endl <<
    "}; " << endl <<
    "GType " << this->nspace_u << name_u << "get_type (void);" << endl <<
    "void " << this->nspace_u << name_u << "class_init (gpointer g_class, gpointer class_data);" << endl <<
    "void " << this->nspace_u << name_u << "class_final (gpointer g_class, gpointer class_data);" << endl <<
    "void " << this->nspace_u << name_u << "instance_init (GTypeInstance *instance, gpointer g_class);" << endl <<
    "#define " << this->nspace_uc << name_uc << "TYPE (" << this->nspace_u << name_u << "get_type ())" << endl <<
    "#define " << this->nspace_uc << name_uc << "(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), " << this->nspace_uc << name_uc << "TYPE, " << this->nspace << name << "))" << endl <<
    "#define " << this->nspace_uc << name_uc << "CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), " << this->nspace_uc << name_uc << ", " << this->nspace << name << "Class))" << endl <<
    "#define " << this->nspace_uc << "IS_" << name_uc << "(obj) (G_TYPE_CHECK_TYPE ((obj), " << this->nspace_uc << name_uc << "TYPE))" << endl << 
    "#define " << this->nspace_uc << "IS_" << name_uc << "CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), " << this->nspace_uc << name_uc << "TYPE))" << endl <<
    "#define " << this->nspace_uc << name_uc << "GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), " << this->nspace_uc << name_uc << "TYPE, " << this->nspace << name << "Class))" << endl <<
    endl;

  f_types_impl_ <<
    "GType " << this->nspace_u << name_u << "get_type (void)" << endl <<
    "{" << endl <<
    "    static GType type = 0;" << endl << endl <<
    "    if (type == 0) " << endl <<
    "    {" << endl <<
    "        static const GTypeInfo type_info = " << endl <<
    "        {" << endl <<
    "            sizeof (" << this->nspace << name << "Class)," << endl <<
    "            NULL, /* base_init */" << endl <<
    "            NULL, /* base_finalize */" << endl <<
    "            NULL, /* class_init */" << endl <<
    "            NULL, /* class_finalize */" << endl <<
    "            NULL, /* class_data */" << endl <<
    "            sizeof (" << this->nspace << name << ")," << endl <<
    "            0, /* n_preallocs */" << endl <<
    "            " << this->nspace_u << name_u << "instance_init /* instance_init */" << endl <<
    "        };" << endl << endl <<
    "        type = g_type_register_static (G_TYPE_OBJECT, " << endl <<
    "            \"" << this->nspace << name << "Type\", /* type name as string */" << endl <<
    "            &type_info, 0);" << endl <<
    "    }" << endl << endl << 
    "    return type;" << endl << 
    "}" << endl << 
    "void " << this->nspace_u << name_u << "instance_init (GTypeInstance *instance, gpointer g_class) {" << endl <<
    "    " << this->nspace << name << " * this = " << this->nspace_uc << name_uc << "(instance);" << endl;

  for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
    t_type* t = get_true_type((*m_iter)->get_type());
    if (t->is_base_type()) {
      // only have init's for base types
      string dval = " = ";
      if (t->is_enum()) {
        dval += "(" + type_name(t) + ")";
      }
      t_const_value* cv = (*m_iter)->get_value();
      if (cv != NULL) {
        dval += constant_value(t, cv);
      } else {
        dval += t->is_string() ? "\"\"" : "0";
      }
      f_types_impl_ << "    this->" << (*m_iter)->get_name() << 
        dval << ";" << endl;
    }
  }
  f_types_impl_ << "}" << endl;
}

/**
 * Generates a class that holds all the constants.
 */
void t_c_generator::generate_consts(vector<t_const*> consts) {

  f_types_ << "/* constants */" << endl;

  vector<t_const*>::iterator c_iter;
  for (c_iter = consts.begin(); c_iter != consts.end(); ++c_iter) {
    string name = (*c_iter)->get_name();
    t_type * type = (*c_iter)->get_type();
    t_const_value * value = (*c_iter)->get_value();
    f_types_ << 
      indent() << "#define " << name << " " << constant_value(type, value) << endl;
  }

  f_types_ << endl;
}

/**
 * Generates a thrift service. In C++, this comprises an entirely separate
 * header and source file. The header file defines the methods and includes
 * the data types defined in the main header file, and the implementation
 * file contains implementations of the basic printer and default interfaces.
 *
 * @param tservice The service definition
 */
void t_c_generator::generate_service(t_service* tservice) {
}

string t_c_generator::type_name(t_type* ttype) {
  if (ttype->is_base_type()) {
    return base_type_name(((t_base_type*)ttype)->get_base());
  } else {
    fprintf(stderr, "ttype->name: %s\n", ttype->get_name().c_str());
    return ttype->get_name();
  }
}

/**
 * Returns the C++ type that corresponds to the thrift type.
 *
 * @param tbase The base type
 * @return Explicit C++ type, i.e. "int32_t"
 */
string t_c_generator::base_type_name(t_base_type::t_base tbase) {
  switch (tbase) {
  case t_base_type::TYPE_VOID:
    return "void";
  case t_base_type::TYPE_STRING:
    return "char *";
  case t_base_type::TYPE_BOOL:
    return "bool";
  case t_base_type::TYPE_BYTE:
    return "int8_t";
  case t_base_type::TYPE_I16:
    return "int16_t";
  case t_base_type::TYPE_I32:
    return "int32_t";
  case t_base_type::TYPE_I64:
    return "int64_t";
  case t_base_type::TYPE_DOUBLE:
    return "double";
  default:
    throw "compiler error: no C++ base type name for base type " + t_base_type::t_base_name(tbase);
  }
}

/**
 *
 */
string t_c_generator::constant_value(t_type * type, t_const_value * value) {

  ostringstream render;

  if (type->is_base_type()) {
    t_base_type::t_base tbase = ((t_base_type*)type)->get_base();
    switch (tbase) {
    case t_base_type::TYPE_STRING:
      render << "\"" + value->get_string() + "\"";
      break;
    case t_base_type::TYPE_BOOL:
      render << ((value->get_integer() != 0) ? "true" : "false");
      break;
    case t_base_type::TYPE_BYTE:
    case t_base_type::TYPE_I16:
    case t_base_type::TYPE_I32:
    case t_base_type::TYPE_I64:
      render << value->get_integer();
      break;
    case t_base_type::TYPE_DOUBLE:
      if (value->get_type() == t_const_value::CV_INTEGER) {
        render << value->get_integer();
      } else {
        render << value->get_double();
      }
      break;
    default:
      throw "compiler error: no const of base type " + t_base_type::t_base_name(tbase);
    }
  } else if (type->is_enum()) {
    render << "(" << type_name(type) << ")" << value->get_integer();
  } else {
    /* TODO: what is this case 
    string t = tmp("tmp");
    indent(out) << type_name(type) << " " << t << ";" << endl;
    print_const_value(out, t, type, value);
    render << t;
    */
    render << "\"not yet supported\"";
  }

  return render.str();
}

THRIFT_REGISTER_GENERATOR(c, "C", "");
