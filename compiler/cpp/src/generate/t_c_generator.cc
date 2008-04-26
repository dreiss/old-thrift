// Copyright (c) 2007- RM
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/


/**
 * TODO:
 * - use glib stuff where it can be since we'll require it for the code
 *   we produce, g_strdup is an example. also may help with intl support
 * - use G_UNLIKELY where approrate
 */

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

// TODO: more of these
string initial_caps_to_underscores(string name)
{
  char buf[256];
  const char * tmp = name.c_str();
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

  return buf;
}

string to_upper_case(string name)
{
  char tmp[256];
  strncpy (tmp, name.c_str(), name.length ());
  for (unsigned int i = 0; i < strlen(tmp); i++)
    tmp[i] = toupper(tmp[i]);
  return tmp;
}

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
  std::ofstream f_header_;
  std::ofstream f_service_;

  string nspace;
  string nspace_u;
  string nspace_uc;

  /** 
   * helper functions
   */

  void generate_service_client(t_service* tservice);

  void generate_object(t_struct* tstruct);

  void generate_struct_writer(ofstream& out, t_struct* tstruct, 
                              bool pointers=false);
  void generate_serialize_field(ofstream& out, t_field* tfield,
                                string prefix="", string suffix="");
  void generate_serialize_struct(ofstream& out, t_struct* tstruct,
                                 string prefix="");
  void generate_serialize_container(ofstream& out, t_type* ttype,
                                    string prefix="");
  void generate_serialize_map_element(ofstream& out, t_map* tmap, string iter);
  void generate_serialize_set_element(ofstream& out, t_set* tmap, string iter);
  void generate_serialize_list_element(ofstream& out, t_list* tlist,
                                       string list, string index);

  std::string function_signature(t_function* tfunction);
  std::string argument_list(t_struct* tstruct, bool name_params=true);
  std::string type_name(t_type* ttype, bool in_typedef=false, bool is_const=false);
  string base_type_name(t_base_type::t_base tbase);
  string type_to_enum(t_type * type);
  string constant_value(t_type * type, t_const_value * value);
  std::string declare_field(t_field* tfield, bool init=false, bool pointer=false, bool constant=false, bool reference=false);

  bool is_complex_type(t_type* ttype) {
    ttype = get_true_type(ttype);

    return
      ttype->is_container() ||
      ttype->is_struct() ||
      ttype->is_xception() ||
      (ttype->is_base_type() && (((t_base_type*)ttype)->get_base() == t_base_type::TYPE_STRING));
  }

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

  string program_name_u = initial_caps_to_underscores(program_name_);

  // Make output file
  string f_types_name = get_out_dir()+program_name_u+"_types.h";
  f_types_.open(f_types_name.c_str());

  string f_types_impl_name = get_out_dir()+program_name_u+"_types.c";
  f_types_impl_.open(f_types_impl_name.c_str());

  // Print header
  f_types_ <<
    autogen_comment();
  f_types_impl_ <<
    autogen_comment();

  string program_name_uc = to_upper_case (program_name_u);

  // Start ifndef
  f_types_ <<
    "#ifndef " << program_name_uc << "_TYPES_H" << endl <<
    "#define " << program_name_uc << "_TYPES_H" << endl <<
    endl;

  // Include base types
  f_types_ <<
    "/* base includes */" << endl <<
    "#include <glib-object.h>" << endl <<
    "#include \"thrift_protocol.h\"" << endl <<
    "#include \"thrift_struct.h\"" << endl <<
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
    "#include \"" << program_name_u << "_types.h\"" << endl <<
    endl;

  f_types_ << 
    "/* begin types */" << endl << endl;
}

/**
 * Closes the output files.
 */
void t_c_generator::close_generator() {

  string program_name_uc = to_upper_case
    (initial_caps_to_underscores(program_name_));

  // Close ifndef
  f_types_ <<
    "#endif /* " << program_name_uc << "_TYPES_H */" << endl;

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
    indent() << "typedef " << type_name(ttypedef->get_type(), true) << " " << 
    this->nspace << ttypedef->get_symbolic() << ";" << endl << endl;
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
    indent() << "typedef enum _" << this->nspace << tenum->get_name() << 
    " " << this->nspace << tenum->get_name() << ";" << endl <<
    indent() << "enum _" << this->nspace << tenum->get_name() << " {" << endl;
    
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

  f_types_ << 
    "typedef struct _" << this->nspace << name << " " << this->nspace << name << ";" << endl <<
    "struct _" << this->nspace << name << endl <<
    "{ " << endl <<
    "    ThriftStruct parent; " << endl << 
    endl << 
    "    /* private */" << endl;

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
    "    ThriftStructClass parent; " << endl <<
    "}; " << endl <<
    "GType " << this->nspace_u << name_u << "get_type (void);" << endl <<
    "#define " << this->nspace_uc << "TYPE_" << name_uc << " (" << this->nspace_u << name_u << "get_type ())" << endl <<
    // TODO: get rid of trailing _ on this one
    "#define " << this->nspace_uc << name_uc << "(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), " << this->nspace_uc << "TYPE_" << name_uc << ", " << this->nspace << name << "))" << endl <<
    "#define " << this->nspace_uc << name_uc << "_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), " << this->nspace_uc << "TYPE_" << name_uc << ", " << this->nspace << name << "Class))" << endl <<
    "#define " << this->nspace_uc << "IS_" << name_uc << "(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), " << this->nspace_uc << "TYPE_" << name_uc << "))" << endl << 
    "#define " << this->nspace_uc << "IS_" << name_uc << "_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), " << this->nspace_uc << "TYPE_" << name_uc << "))" << endl <<
    "#define " << this->nspace_uc << name_uc << "_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), " << this->nspace_uc << "TYPE_" << name_uc << ", " << this->nspace << name << "Class))" << endl << 
    endl;

  generate_struct_writer (f_types_impl_, tstruct);

  f_types_impl_ <<
    "void " << this->nspace_u << name_u << "instance_init (" << this->nspace << name << " * object)" << endl <<
    "{" << endl;

  indent_up();

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
      indent(f_types_impl_) << "object->" << (*m_iter)->get_name() << 
        dval << ";" << endl;
    }
  }
  indent_down();
  f_types_impl_ << "}" << endl <<
    endl <<
    "void " << this->nspace_u << name_u << "class_init (ThriftStructClass * thrift_struct_class)" << endl <<
    "{" << endl <<
    "  thrift_struct_class->write = " << this->nspace_u << name_u << "write;" << endl <<
    "}" << endl <<
    endl <<
    "GType " << this->nspace_u << name_u << "get_type (void)" << endl <<
    "{" << endl <<
    "  static GType type = 0;" << endl << 
    endl <<
    "  if (type == 0) " << endl <<
    "  {" << endl <<
    "    static const GTypeInfo type_info = " << endl <<
    "    {" << endl <<
    "      sizeof (" << this->nspace << name << "Class)," << endl <<
    "      NULL, /* base_init */" << endl <<
    "      NULL, /* base_finalize */" << endl <<
    "      (GClassInitFunc)" << this->nspace_u << name_u << "class_init," << endl <<
    "      NULL, /* class_finalize */" << endl <<
    "      NULL, /* class_data */" << endl <<
    "      sizeof (" << this->nspace << name << ")," << endl <<
    "      0, /* n_preallocs */" << endl <<
    "      (GInstanceInitFunc)" << this->nspace_u << name_u << "instance_init," << endl <<
    "      NULL, /* value_table */" << endl <<
    "    };" << endl << 
    endl <<
    "    type = g_type_register_static (THRIFT_TYPE_STRUCT, " << endl <<
    "                                   \"" << this->nspace << name << "Type\"," << endl << 
    "                                   &type_info, 0);" << endl <<
    "  }" << endl << 
    endl << 
    "  return type;" << endl << 
    "}" << endl << 
    endl;
}

/**
 * Generates the write function.
 *
 * @param out Stream to write to
 * @param tstruct The struct
 */
void t_c_generator::generate_struct_writer(ofstream& out, t_struct* tstruct,
                                           bool pointers) {
  string name = tstruct->get_name();
  string name_u = initial_caps_to_underscores (name);
  string name_uc = to_upper_case (name_u);

  const vector<t_field*>& fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;

  indent(out) <<
    "gint32 " << this->nspace_u << name_u <<
    "_write (ThriftStruct * object, ThriftProtocol * thrift_protocol)" << endl <<
    "{" << endl;
  indent_up();

  out <<
    indent() << "gint32 xfer = 0;" << endl;

  indent(out) << this->nspace << name << " * this_object = " << this->nspace_uc << name_uc << "(object);" << endl;

  indent(out) << "xfer += thrift_protocol_write_struct_begin (thrift_protocol, \"" << name << "\");" << endl;

  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    if ((*f_iter)->get_req() == t_field::T_OPTIONAL) {
      indent(out) << "if (this->__isset." << (*f_iter)->get_name() << ") {" << endl;
      indent_up();
    }
    // Write field header
    out <<
      indent() << "xfer += thrift_protocol_write_field_begin (thrift_protocol, " <<
      "\"" << (*f_iter)->get_name() << "\", " <<
      type_to_enum((*f_iter)->get_type()) << ", " <<
      (*f_iter)->get_key() << ");" << endl;
    // Write field contents
    if (pointers) {
      generate_serialize_field(out, *f_iter, "(*(this_object->", "))");
    } else {
      generate_serialize_field(out, *f_iter, "this_object->");
    }
    // Write field closer
    indent(out) <<
      "xfer += thrift_protocol_write_field_end (thrift_protocol);" << endl;
    if ((*f_iter)->get_req() == t_field::T_OPTIONAL) {
      indent_down();
      indent(out) << '}' << endl;
    }
  }

  // Write the struct map
  out <<
    indent() << "xfer += thrift_protocol_write_field_stop(thrift_protocol);" << endl <<
    indent() << "xfer += thrift_protocol_write_struct_end(thrift_protocol);" << endl <<
    indent() << "return xfer;" << endl;

  indent_down();
  indent(out) <<
    "}" << endl <<
    endl;
}


/**
 * Serializes a field of any type.
 *
 * @param tfield The field to serialize
 * @param prefix Name to prepend to field name
 */
void t_c_generator::generate_serialize_field(ofstream& out, t_field* tfield,
                                             string prefix, string suffix) {
  t_type* type = get_true_type(tfield->get_type());

  string name = prefix + tfield->get_name() + suffix;

  // Do nothing for void types
  if (type->is_void()) {
    throw "CANNOT GENERATE SERIALIZE CODE FOR void TYPE: " + name;
  }

  if (type->is_struct() || type->is_xception()) {
    generate_serialize_struct(out,
                              (t_struct*)type,
                              name);
  } else if (type->is_container()) {
    generate_serialize_container(out, type, name);
  } else if (type->is_base_type() || type->is_enum()) {

    indent(out) <<
      "xfer += thrift_protocol_";

    if (type->is_base_type()) {
      t_base_type::t_base tbase = ((t_base_type*)type)->get_base();
      switch (tbase) {
      case t_base_type::TYPE_VOID:
        throw
          "compiler error: cannot serialize void field in a struct: " + name;
        break;
      case t_base_type::TYPE_STRING:
        if (((t_base_type*)type)->is_binary()) {
          out << "write_binary(thrift_protocol, " << name << ");";
        }
        else {
          out << "write_string(thrift_protocol, " << name << ");";
        }
        break;
      case t_base_type::TYPE_BOOL:
        out << "write_bool(thrift_protocol, " << name << ");";
        break;
      case t_base_type::TYPE_BYTE:
        out << "write_byte(thrift_protocol, " << name << ");";
        break;
      case t_base_type::TYPE_I16:
        out << "write_i16(thrift_protocol, " << name << ");";
        break;
      case t_base_type::TYPE_I32:
        out << "write_i32(thrift_protocol, " << name << ");";
        break;
      case t_base_type::TYPE_I64:
        out << "write_i64(thrift_protocol, " << name << ");";
        break;
      case t_base_type::TYPE_DOUBLE:
        out << "write_double(thrift_protocol, " << name << ");";
        break;
      default:
        throw "compiler error: no C++ writer for base type " + t_base_type::t_base_name(tbase) + name;
      }
    } else if (type->is_enum()) {
      out << "write_i32(thrift_protocol, (gint32)" << name << ");";
    }
    out << endl;
  } else {
    printf("DO NOT KNOW HOW TO SERIALIZE FIELD '%s' TYPE '%s'\n",
           name.c_str(),
           type_name(type).c_str());
  }
}

/**
 * Serializes all the members of a struct.
 *
 * @param tstruct The struct to serialize
 * @param prefix  String prefix to attach to all fields
 */
void t_c_generator::generate_serialize_struct(ofstream& out,
                                                t_struct* tstruct,
                                                string prefix) {
  indent(out) <<
    "xfer += thrift_struct_write (THRIFT_STRUCT (" << prefix << "), thrift_protocol);" << endl;
}

void t_c_generator::generate_serialize_container(ofstream& out,
                                                   t_type* ttype,
                                                   string prefix) {
  scope_up(out);

  if (ttype->is_map()) {
    indent(out) <<
      "xfer += thrift_protocol_write_map_begin(thrift_protocol, " <<
      type_to_enum(((t_map*)ttype)->get_key_type()) << ", " <<
      type_to_enum(((t_map*)ttype)->get_val_type()) << ", g_hash_table_size(" <<
      prefix << "));" << endl;
  } else if (ttype->is_set()) {
    indent(out) <<
      "xfer += thrift_protocol_write_set_begin(thrift_protocol, " <<
      type_to_enum(((t_set*)ttype)->get_elem_type()) << ", g_hash_table_size(" <<
      prefix << "));" << endl;
  } else if (ttype->is_list()) {
    indent(out) <<
      "xfer += thrift_protocol_write_list_begin(thrift_protocol, " <<
      type_to_enum(((t_list*)ttype)->get_elem_type()) << ", " <<
      prefix << "->len);" << endl;
  }

  string iter = tmp("_iter");
  out <<
    indent() << "int i;" << endl <<
    indent() << "for (i = 0; i < " << prefix << "->len; i++)" << endl;
  scope_up(out);
    if (ttype->is_map()) {
      generate_serialize_map_element(out, (t_map*)ttype, iter);
    } else if (ttype->is_set()) {
      generate_serialize_set_element(out, (t_set*)ttype, iter);
    } else if (ttype->is_list()) {
      generate_serialize_list_element(out, (t_list*)ttype, prefix, "i");
    }
  scope_down(out);

  if (ttype->is_map()) {
    indent(out) <<
      "xfer += thrift_protocol_write_map_end(thrift_protocol);" << endl;
  } else if (ttype->is_set()) {
    indent(out) <<
      "xfer += thrift_protocol_write_set_end(thrift_protocol);" << endl;
  } else if (ttype->is_list()) {
    indent(out) <<
      "xfer += thrift_protocol_write_list_end(thrift_protocol);" << endl;
  }

  scope_down(out);
}

/**
 * Serializes the members of a map.
 *
 */
void t_c_generator::generate_serialize_map_element(ofstream& out,
                                                     t_map* tmap,
                                                     string iter) {
  t_field kfield(tmap->get_key_type(), iter + "->first");
  generate_serialize_field(out, &kfield, "");

  t_field vfield(tmap->get_val_type(), iter + "->second");
  generate_serialize_field(out, &vfield, "");
}

/**
 * Serializes the members of a set.
 */
void t_c_generator::generate_serialize_set_element(ofstream& out,
                                                     t_set* tset,
                                                     string iter) {
  t_field efield(tset->get_elem_type(), "(*" + iter + ")");
  generate_serialize_field(out, &efield, "");
}

/**
 * Serializes the members of a list.
 */
void t_c_generator::generate_serialize_list_element(ofstream& out,
                                                    t_list* tlist,
                                                    string list,
                                                    string index) {
  t_field efield(tlist->get_elem_type(), 
                 "g_ptr_array_index(" + list + ", " + index + ")");
  generate_serialize_field(out, &efield, "");
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
      indent() << "#define " << this->nspace_uc << name << " " << constant_value(type, value) << endl;
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
  string svcname = initial_caps_to_underscores (tservice->get_name());

  // Make output files
  string f_header_name = get_out_dir()+svcname+".h";
  f_header_.open(f_header_name.c_str());

  string program_name_u = initial_caps_to_underscores (program_name_);

  // Print header file includes
  f_header_ <<
    autogen_comment();
  f_header_ <<
    "#ifndef " << svcname << "_H" << endl <<
    "#define " << svcname << "_H" << endl <<
    endl <<
    "#include \"thrift_client.h\"" << endl <<
    "#include \"" << program_name_u << "_types.h\"" << endl;

  t_service* extends_service = tservice->get_extends();
  if (extends_service != NULL) {
    f_header_ <<
      "#include \"" << extends_service->get_name() << ".h\"" << endl;
  }

  f_header_ <<
    endl;

  // Service implementation file includes
  string f_service_name = get_out_dir()+svcname+".c";
  f_service_.open(f_service_name.c_str());
  f_service_ <<
    autogen_comment();
  f_service_ <<
    "#include \"" << svcname << ".h\"" <<
    endl <<
    endl;

  // Generate all the components
#if 0
  generate_service_interface(tservice);
  generate_service_null(tservice);
  generate_service_helpers(tservice);
#endif
  generate_service_client(tservice);
#if 0
  generate_service_processor(tservice);
  generate_service_multiface(tservice);
  generate_service_skeleton(tservice);
#endif

  f_header_ <<
    "#endif /* " << svcname << "_H */" << endl;

  // Close the files
  f_service_.close();
  f_header_.close();
}

/**
 * Generates a service client definition.
 *
 * @param tservice The service to generate a server for.
 */
void t_c_generator::generate_service_client(t_service* tservice) {
  string extends = "";
  string extends_client = "";
  if (tservice->get_extends() != NULL) {
    extends = type_name(tservice->get_extends());
    extends_client = ", public " + extends + "Client";
  }

  // Generate the header portion
  f_header_ <<
    "typedef struct _Thrift" << service_name_ << "Client Thrift" << service_name_ << "Client;" << endl <<
    "struct _Thrift" << service_name_ << "Client" << endl <<
    "{" << endl <<
    "  ThriftClient parent;" << endl <<
    "};" << endl <<
    "typedef struct _Thrift" << service_name_ << "ClientClass Thrift" << service_name_ << "ClientClass;" << endl <<
    "struct _Thrift" << service_name_ << "ClientClass" << endl;
  scope_up(f_header_);
  indent(f_header_) << "ThriftClientClass parent;" << endl << endl;
  indent_down();
  f_header_ << "};" << endl <<
      endl;

  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::const_iterator f_iter;
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    t_function send_function(g_type_void,
                             string("send_") + (*f_iter)->get_name(),
                             (*f_iter)->get_arglist());
    indent(f_header_) << function_signature(*f_iter) << ";" << endl;
    indent(f_header_) << function_signature(&send_function) << ";" << endl;
    if (!(*f_iter)->is_async()) {
      t_struct noargs(program_);
      t_function recv_function((*f_iter)->get_returntype(),
                               string("recv_") + (*f_iter)->get_name(),
                               &noargs);
      indent(f_header_) << function_signature(&recv_function) << ";" << endl;
    }
  }

  f_header_ << endl;

  // Generate client method implementations
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    string funname = initial_caps_to_underscores((*f_iter)->get_name());

    // Open function
    indent(f_service_) <<
      function_signature(*f_iter) << endl;
    scope_up(f_service_);
    indent(f_service_) <<
      this->nspace_u << "send_" << funname << " (client";

    // Get the struct of function call params
    t_struct* arg_struct = (*f_iter)->get_arglist();

    // Declare the function arguments
    const vector<t_field*>& fields = arg_struct->get_members();
    vector<t_field*>::const_iterator fld_iter;
    for (fld_iter = fields.begin(); fld_iter != fields.end(); ++fld_iter) {
      f_service_ << ", " << (*fld_iter)->get_name();
    }
    f_service_ << ");" << endl;

    if (!(*f_iter)->is_async()) {
      f_service_ << indent();
      if (!(*f_iter)->get_returntype()->is_void()) {
        if (is_complex_type((*f_iter)->get_returntype())) {
          f_service_ << this->nspace_u << "recv_" << funname <<
            " (client, _return);" <<
            endl;
        } else {
          f_service_ << "return " << this->nspace_u << "recv_" << funname <<
            " (client);" << endl;
        }
      } else {
        f_service_ <<
          this->nspace_u << "recv_" << funname << " (client);" << endl;
      }
    }
    scope_down(f_service_);
    f_service_ << endl;

    // Function for sending
    t_function send_function(g_type_void,
                             string("send_") + (*f_iter)->get_name(),
                             (*f_iter)->get_arglist());

    // Open the send function
    indent(f_service_) <<
      function_signature(&send_function) << endl;
    scope_up(f_service_);

    // Function arguments and results
    string argsname = tservice->get_name() + "_" + (*f_iter)->get_name() + "_pargs";
    string resultname = tservice->get_name() + "_" + (*f_iter)->get_name() + "_presult";

    // Serialize the request
    f_service_ <<
      indent() << "gint32 cseqid = 0;" << endl <<
      indent() << "ThriftProtocol * thrift_protocol = THRIFT_CLIENT (client)->thrift_protocol;" << endl <<
      endl <<
      indent() << "thrift_protocol_write_message_begin (thrift_protocol, \"" << (*f_iter)->get_name() << "\", T_CALL, cseqid);" << endl <<
      endl <<
      indent() << argsname << " args;" << endl;

    for (fld_iter = fields.begin(); fld_iter != fields.end(); ++fld_iter) {
      f_service_ <<
        indent() << "args." << (*fld_iter)->get_name() << " = &" << (*fld_iter)->get_name() << ";" << endl;
    }

    f_service_ <<
      indent() << "args.write(oprot_);" << endl <<
      endl <<
      indent() << "thrift_protocol_write_message_end (thrift_protocol);" << endl;

    scope_down(f_service_);
    f_service_ << endl;

    // Generate recv function only if not an async function
    if (!(*f_iter)->is_async()) {
      t_struct noargs(program_);
      t_function recv_function((*f_iter)->get_returntype(),
                               string("recv_") + (*f_iter)->get_name(),
                               &noargs);
      // Open function
      indent(f_service_) <<
        function_signature(&recv_function) << endl;
      scope_up(f_service_);

#if 0
      f_service_ <<
        endl <<
        indent() << "gint32 rseqid = 0;" << endl <<
        indent() << "std::string fname;" << endl <<
        indent() << "facebook::thrift::protocol::TMessageType mtype;" << endl <<
        endl <<
        indent() << "iprot_->readMessageBegin(fname, mtype, rseqid);" << endl <<
        indent() << "if (mtype == facebook::thrift::protocol::T_EXCEPTION) {" << endl <<
        indent() << "  facebook::thrift::TApplicationException x;" << endl <<
        indent() << "  x.read(iprot_);" << endl <<
        indent() << "  iprot_->readMessageEnd();" << endl <<
        indent() << "  iprot_->getTransport()->readEnd();" << endl <<
        indent() << "  throw x;" << endl <<
        indent() << "}" << endl <<
        indent() << "if (mtype != facebook::thrift::protocol::T_REPLY) {" << endl <<
        indent() << "  iprot_->skip(facebook::thrift::protocol::T_STRUCT);" << endl <<
        indent() << "  iprot_->readMessageEnd();" << endl <<
        indent() << "  iprot_->getTransport()->readEnd();" << endl <<
        indent() << "  throw facebook::thrift::TApplicationException(facebook::thrift::TApplicationException::INVALID_MESSAGE_TYPE);" << endl <<
        indent() << "}" << endl <<
        indent() << "if (fname.compare(\"" << (*f_iter)->get_name() << "\") != 0) {" << endl <<
        indent() << "  iprot_->skip(facebook::thrift::protocol::T_STRUCT);" << endl <<
        indent() << "  iprot_->readMessageEnd();" << endl <<
        indent() << "  iprot_->getTransport()->readEnd();" << endl <<
        indent() << "  throw facebook::thrift::TApplicationException(facebook::thrift::TApplicationException::WRONG_METHOD_NAME);" << endl <<
        indent() << "}" << endl;

      if (!(*f_iter)->get_returntype()->is_void() &&
          !is_complex_type((*f_iter)->get_returntype())) {
        t_field returnfield((*f_iter)->get_returntype(), "_return");
        f_service_ <<
          indent() << declare_field(&returnfield) << endl;
      }

      f_service_ <<
        indent() << resultname << " result;" << endl;

      if (!(*f_iter)->get_returntype()->is_void()) {
        f_service_ <<
          indent() << "result.success = &_return;" << endl;
      }

      f_service_ <<
        indent() << "result.read(iprot_);" << endl <<
        indent() << "iprot_->readMessageEnd();" << endl <<
        indent() << "iprot_->getTransport()->readEnd();" << endl <<
        endl;

      // Careful, only look for _result if not a void function
      if (!(*f_iter)->get_returntype()->is_void()) {
        if (is_complex_type((*f_iter)->get_returntype())) {
          f_service_ <<
            indent() << "if (result.__isset.success) {" << endl <<
            indent() << "  // _return pointer has now been filled" << endl <<
            indent() << "  return;" << endl <<
            indent() << "}" << endl;
        } else {
          f_service_ <<
            indent() << "if (result.__isset.success) {" << endl <<
            indent() << "  return _return;" << endl <<
            indent() << "}" << endl;
        }
      }

      t_struct* xs = (*f_iter)->get_xceptions();
      const std::vector<t_field*>& xceptions = xs->get_members();
      vector<t_field*>::const_iterator x_iter;
      for (x_iter = xceptions.begin(); x_iter != xceptions.end(); ++x_iter) {
        f_service_ <<
          indent() << "if (result.__isset." << (*x_iter)->get_name() << ") {" << endl <<
          indent() << "  throw result." << (*x_iter)->get_name() << ";" << endl <<
          indent() << "}" << endl;
      }

      // We only get here if we are a void function
      if ((*f_iter)->get_returntype()->is_void()) {
        indent(f_service_) <<
          "return;" << endl;
      } else {
        f_service_ <<
          indent() << "throw facebook::thrift::TApplicationException(facebook::thrift::TApplicationException::MISSING_RESULT, \"" << (*f_iter)->get_name() << " failed: unknown result\");" << endl;
      }
#endif

      // Close function
      scope_down(f_service_);
      f_service_ << endl;
    }
  }
}

/**
 * Renders a function signature of the form 'type name(args)'
 *
 * @param tfunction Function definition
 * @return String of rendered function definition
 */
string t_c_generator::function_signature(t_function* tfunction) {
  t_type* ttype = tfunction->get_returntype();
  t_struct* arglist = tfunction->get_arglist();

  string fname = initial_caps_to_underscores(tfunction->get_name());

  bool empty = arglist->get_members().size() == 0;
  if (is_complex_type(ttype)) {
    return
      "void " + this->nspace_u + fname + " (Thrift" + service_name_ +
      "Client * client, " + type_name(ttype) + "* _return" +
      (empty ? "" : (", " + argument_list(arglist))) + ")";
  } else {
    return
      type_name(ttype) + " " + this->nspace_u + fname +
      " (Thrift" + service_name_ + "Client * client" + 
      (empty ? "" : ", " + argument_list(arglist)) + ")";
  }
}

/**
 * Renders a field list
 *
 * @param tstruct The struct definition
 * @return Comma sepearated list of all field names in that struct
 */
string t_c_generator::argument_list(t_struct* tstruct, bool name_params) {
  string result = "";

  const vector<t_field*>& fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;
  bool first = true;
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    if (first) {
      first = false;
    } else {
      result += ", ";
    }
    result += type_name((*f_iter)->get_type(), false, true) + " " +
      (name_params ? (*f_iter)->get_name() : "/* " + (*f_iter)->get_name() + " */");
  }
  return result;
}

/**
 * Declares a field, which may include initialization as necessary.
 *
 * @param ttype The type
 * @return Field declaration, i.e. int x = 0;
 */
string t_c_generator::declare_field(t_field* tfield, bool init, bool pointer, bool constant, bool reference) {
  // TODO(mcslee): do we ever need to initialize the field?
  string result = "";
  if (constant) {
    result += "const ";
  }
  result += type_name(tfield->get_type());
  if (pointer) {
    result += "*";
  }
  if (reference) {
    result += "&";
  }
  result += " " + tfield->get_name();
  if (init) {
    t_type* type = get_true_type(tfield->get_type());

    if (type->is_base_type()) {
      t_base_type::t_base tbase = ((t_base_type*)type)->get_base();
      switch (tbase) {
      case t_base_type::TYPE_VOID:
        break;
      case t_base_type::TYPE_STRING:
        result += " = \"\"";
        break;
      case t_base_type::TYPE_BOOL:
        result += " = false";
        break;
      case t_base_type::TYPE_BYTE:
      case t_base_type::TYPE_I16:
      case t_base_type::TYPE_I32:
      case t_base_type::TYPE_I64:
        result += " = 0";
        break;
      case t_base_type::TYPE_DOUBLE:
        result += " = (double)0";
        break;
      default:
        throw "compiler error: no C++ initializer for base type " + t_base_type::t_base_name(tbase);
      }
    } else if (type->is_enum()) {
      result += " = (" + type_name(type) + ")0";
    }
  }
  if (!reference) {
    result += ";";
  }
  return result;
}

/**
 * Returns a C++ type name
 *
 * @param ttype The type
 * @return String of the type name, i.e. std::set<type>
 */
string t_c_generator::type_name(t_type* ttype, bool in_typedef, bool is_const) {

  if (ttype->is_base_type()) {
    string bname = base_type_name(((t_base_type*)ttype)->get_base());

    if (is_const) {
      return "const " + bname;
    } else {
      return bname;
    }
  }

  // Check for a custom overloaded C++ name
  if (ttype->is_container()) {
    string cname;

    t_container* tcontainer = (t_container*) ttype;
    if (tcontainer->has_cpp_name()) {
      cname = tcontainer->get_cpp_name();
    } else if (ttype->is_map()) {
      cname = "GHashTable *";
      /* TODO:
       * t_map* tmap = (t_map*) ttype;
       * type_name(tmap->get_key_type(), in_typedef) + ", " +
       * type_name(tmap->get_val_type(), in_typedef) + "> ";
       */
    } else if (ttype->is_set()) {
      cname = "GHashTable *";
      /* TODO:
       * t_set* tset = (t_set*) ttype;
       * + type_name(tset->get_elem_type(), in_typedef) + "> ";
       */
    } else if (ttype->is_list()) {
      cname = "GPtrArray *";
      /* TODO:
       * t_list* tlist = (t_list*) ttype;
       * + type_name(tlist->get_elem_type(), in_typedef) + "> ";
       */
    }

    if (is_const) {
      return "const " + cname;
    } else {
      return cname;
    }
  }

  // Check if it needs to be namespaced
  string pname = this->nspace + ttype->get_name();

  if (is_complex_type(ttype)) {
    pname += " *";
  }

  if (is_const) {
    return "const " + pname;
  } else {
    return pname;
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
    return "gchar *";
  case t_base_type::TYPE_BOOL:
    return "bool";
  case t_base_type::TYPE_BYTE:
    return "gint8";
  case t_base_type::TYPE_I16:
    return "gint16";
  case t_base_type::TYPE_I32:
    return "gint32";
  case t_base_type::TYPE_I64:
    return "gint64";
  case t_base_type::TYPE_DOUBLE:
    return "double";
  default:
    throw "compiler error: no C++ base type name for base type " + t_base_type::t_base_name(tbase);
  }
}

string t_c_generator::type_to_enum(t_type* type) {
  type = get_true_type(type);

  if (type->is_base_type()) {
    t_base_type::t_base tbase = ((t_base_type*)type)->get_base();
    switch (tbase) {
    case t_base_type::TYPE_VOID:
      throw "NO T_VOID CONSTRUCT";
    case t_base_type::TYPE_STRING:
      return "T_STRING";
    case t_base_type::TYPE_BOOL:
      return "T_BOOL";
    case t_base_type::TYPE_BYTE:
      return "T_BYTE";
    case t_base_type::TYPE_I16:
      return "T_I16";
    case t_base_type::TYPE_I32:
      return "T_I32";
    case t_base_type::TYPE_I64:
      return "T_I64";
    case t_base_type::TYPE_DOUBLE:
      return "T_DOUBLE";
    }
  } else if (type->is_enum()) {
    return "T_I32";
  } else if (type->is_struct()) {
    return "T_STRUCT";
  } else if (type->is_xception()) {
    return "T_STRUCT";
  } else if (type->is_map()) {
    return "T_MAP";
  } else if (type->is_set()) {
    return "T_SET";
  } else if (type->is_list()) {
    return "T_LIST";
  }

  throw "INVALID TYPE IN type_to_enum: " + type->get_name();
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
