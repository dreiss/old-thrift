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
 * - collections stuff needs LOTS of work
 * - all protocol write methods (and others too) that take gerrors need to have
 *   their return values checked and abrt things accordingly... 
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
  tmp[name.length()] = '\0';
  return tmp;
}

/**
 * C code generator. This is legitimacy incarnate.
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
                              string this_name, string this_get="", 
                              bool is_function=true);
  void generate_struct_reader(std::ofstream& out, t_struct* tstruct, 
                              string this_name, string this_get="",
                              bool is_function=true);

  void generate_serialize_field(ofstream& out, t_field* tfield,
                                string prefix, string suffix, 
                                int error_ret);
  void generate_serialize_struct(ofstream& out, t_struct* tstruct,
                                 string prefix, int error_ret);
  void generate_serialize_container(ofstream& out, t_type* ttype,
                                    string prefix, int error_ret);
  void generate_serialize_map_element(ofstream& out, t_map* tmap, string key,
                                      string value, int error_ret);
  void generate_serialize_set_element(ofstream& out, t_set* tmap, 
                                      string element, int error_ret);
  void generate_serialize_list_element(ofstream& out, t_list* tlist,
                                       string list, string index, 
                                       int error_ret);

  void generate_deserialize_field(std::ofstream& out, t_field* tfield,
                                  std::string prefix, std::string suffix,
                                  int error_ret);
  void generate_deserialize_struct(std::ofstream& out, t_struct* tstruct,
                                   std::string prefix, int error_ret);
  void generate_deserialize_xception(std::ofstream& out, t_struct* tstruct,
                                     std::string prefix, int error_ret);
  void generate_deserialize_container(std::ofstream& out, t_type* ttype,
                                      std::string prefix, int error_ret);
  void generate_deserialize_set_element(std::ofstream& out, t_set* tset,
                                        std::string prefix, int error_ret);
  void generate_deserialize_map_element(std::ofstream& out, t_map* tmap,
                                        std::string prefix, int error_ret);
  void generate_deserialize_list_element(std::ofstream& out, t_list* tlist,
                                         std::string prefix, std::string index, 
                                         int error_ret);

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
    "#include \"protocol/thrift_protocol.h\"" << endl <<
    "#include \"thrift_struct.h\"" << endl <<
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
    "#include \"" << program_name_u << "_types.h\"" << endl <<
    "#include \"thrift.h\"" << endl <<
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
 * Generates a typedef. This is just a simple 1-liner in C
 *
 * @param ttypedef The type definition
 */
void t_c_generator::generate_typedef(t_typedef* ttypedef) {
  f_types_ <<
    indent() << "typedef " << type_name(ttypedef->get_type(), true) << " " << 
    this->nspace << ttypedef->get_symbolic() << ";" << endl << endl;
}

/**
 * Generates code for an enumerated type. In C, this is essentially the same
 * as the thrift definition itself, using the enum keyword in C.
 *
 * @param tenum The enumeration
 */
void t_c_generator::generate_enum(t_enum* tenum) {

  string name = tenum->get_name();
  string name_uc = to_upper_case(initial_caps_to_underscores(name));

  // TODO: look in to using glib's enum facilities
  f_types_ <<
    indent() << "typedef enum _" << this->nspace << name << 
    " " << this->nspace << name << ";" << endl <<
    indent() << "enum _" << this->nspace << name << " {" << endl;
    
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
      indent() << this->nspace_uc << name_uc << "_" << (*c_iter)->get_name();
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
  string name = tstruct->get_name();
  string name_u = initial_caps_to_underscores (name);
  string name_uc = to_upper_case (name_u);

  f_types_ << "/* exception */" << endl <<
      "typedef enum" << endl <<
      "{" << endl <<
      "    " << this->nspace_uc << name_uc << "_ERROR_CODE," << endl <<
      "} " << this->nspace << name << "Error;" << endl <<
      endl <<
      "GQuark " << this->nspace_u << name_u << "_error_quark (void);" << endl <<
      "#define " << this->nspace_uc << name_uc << "_ERROR (" << this->nspace_u << name_u << "_error_quark ())" << endl <<
      endl <<
      "gint32 " << this->nspace_u << name_u << "_read (GError ** ex, ThriftProtocol * protocol, GError ** error);" << endl <<
      endl;

  /* TODO: this is a HACK */
  f_types_impl_ << 
    "gint32 " << this->nspace_u << name_u << "_read (GError ** ex, ThriftProtocol * protocol, GError ** error)" << endl <<
    "{" << endl <<
    "    gint32 ret;" << endl <<
    "    gint32 xfer = 0;" << endl <<
    "    gchar * name;" << endl <<
    "    ThriftType ftype;" << endl <<
    "    gint16 fid;" << endl <<
    endl <<
    "    if ((ret = thrift_protocol_read_struct_begin (protocol, &name, error)) < 0)" << endl <<
    "        return -1;" << endl <<
    "    xfer += ret;" << endl <<
    "    if (name) g_free (name);" << endl <<
    endl <<
    "    while (1)" << endl <<
    "    {" << endl <<
    "        if ((ret = thrift_protocol_read_field_begin (protocol, &name, &ftype, &fid, error)) < 0)" << endl <<
    "            return -1;" << endl <<
    "        xfer += ret;" << endl <<
    "        if (name) g_free (name);" << endl <<
    "        if (ftype == T_STOP) {" << endl <<
    "            break;" << endl <<
    "        }" << endl <<
    "        switch (fid)" << endl <<
    "        {" << endl <<
    "            case 1:" << endl <<
    "                if (ftype == T_STRING) {" << endl <<
    "                    gchar * what;" << endl <<
    "                    if ((ret = thrift_protocol_read_string (protocol, &what, error)) < 0)" << endl <<
    "                        return -1;" << endl <<
    "                    xfer += ret;" << endl <<
    "                    g_set_error (ex, 0, 0, \"%s\", what);" << endl <<
    "                    if (what) g_free (what);" << endl <<
    "                } else {" << endl <<
    "                    if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)" << endl <<
    "                        return -1;" << endl <<
    "                    xfer += ret;" << endl <<
    "                }" << endl <<
    "                break;" << endl <<
    "            default:" << endl <<
    "                if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)" << endl <<
    "                    return -1;" << endl <<
    "                xfer += ret;" << endl <<
    "                break;" << endl <<
    "        }" << endl <<
    "        if ((ret = thrift_protocol_read_field_end (protocol, error)) < 0)" << endl <<
    "            return -1;" << endl <<
    "        xfer += ret;" << endl <<
    "    }" << endl <<
    endl <<
    "    if ((ret = thrift_protocol_read_struct_end (protocol, error)) < 0)" << endl <<
    "        return -1;" << endl <<
    "    xfer += ret;" << endl <<
    endl <<
    "    return xfer;" << endl <<
    "}" << endl <<
    endl;
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
    "#define " << this->nspace_uc << name_uc << "(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), " << this->nspace_uc << "TYPE_" << name_uc << ", " << this->nspace << name << "))" << endl <<
    "#define " << this->nspace_uc << name_uc << "_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), " << this->nspace_uc << "TYPE_" << name_uc << ", " << this->nspace << name << "Class))" << endl <<
    "#define " << this->nspace_uc << "IS_" << name_uc << "(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), " << this->nspace_uc << "TYPE_" << name_uc << "))" << endl << 
    "#define " << this->nspace_uc << "IS_" << name_uc << "_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), " << this->nspace_uc << "TYPE_" << name_uc << "))" << endl <<
    "#define " << this->nspace_uc << name_uc << "_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), " << this->nspace_uc << "TYPE_" << name_uc << ", " << this->nspace << name << "Class))" << endl << 
    endl;

  string this_get = this->nspace + name + " * this_object = " +
    this->nspace_uc + name_uc + "(object);";

  generate_struct_writer (f_types_impl_, tstruct, "this_object->", this_get);

  generate_struct_reader (f_types_impl_, tstruct, "this_object->", this_get);

  f_types_impl_ <<
    "void " << this->nspace_u << name_u << "instance_init (" << this->nspace << name << " * object)" << endl <<
    "{" << endl;

  indent_up();
    
  indent (f_types_impl_) << "THRIFT_UNUSED_VAR (object);" << endl;

  /* TODO: init's for pointers (NULL) and containers as appro. */
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
        dval += t->is_string() ? "NULL" : "0";
      }
      indent(f_types_impl_) << "object->" << (*m_iter)->get_name() << 
        dval << ";" << endl;
    } else if (t->is_struct()) {
      string name = (*m_iter)->get_name();
      string type_name_uc = to_upper_case 
        (initial_caps_to_underscores((*m_iter)->get_type()->get_name()));
      indent (f_types_impl_) << "object->" << name << " = g_object_new (" << this->nspace_uc << "TYPE_" << type_name_uc << ", NULL);" << endl;
    } else if (t->is_xception()) {
      string name = (*m_iter)->get_name();
      indent (f_types_impl_) << "object->" << name << " = NULL;" << endl;
    } else if (t->is_container()) {
      string name = (*m_iter)->get_name();
      indent (f_types_impl_) << "/* object->" << name << " = not-implemented; */" << endl;
    }
  }

  /* TODO: finalize method that free's memory and otherwise cleans up */
  indent_down();
  f_types_impl_ << "}" << endl <<
    endl <<
    "void " << this->nspace_u << name_u << "class_init (ThriftStructClass * thrift_struct_class)" << endl <<
    "{" << endl <<
    /* TODO: make members properties... */
    "  thrift_struct_class->read = " << this->nspace_u << name_u << "read;" << endl <<
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
                                           string this_name, string this_get,
                                           bool is_function) {
  string name = tstruct->get_name();
  string name_u = initial_caps_to_underscores (name);
  string name_uc = to_upper_case (name_u);

  const vector<t_field*>& fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;

  int error_ret = 0;

  if (is_function) {
    error_ret = -1;
    indent(out) <<
      "gint32 " << this->nspace_u << name_u <<
      "_write (ThriftStruct * object, ThriftProtocol * protocol, GError ** error)" << endl;
  }

  indent(out) << "{" << endl;
  indent_up();

  out << 
    indent() << "gint32 ret;" << endl <<
    indent() << "gint32 xfer = 0;" << endl;

  indent(out) << this_get << endl;

  out << 
    indent() << "if ((ret = thrift_protocol_write_struct_begin (protocol, \"" << name << "\", error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl;

  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    // TODO: optional fields
    if ((*f_iter)->get_req() == t_field::T_OPTIONAL) {
      indent(out) << "if (this->__isset." << (*f_iter)->get_name() << ") {" << endl;
      indent_up();
    }
    // Write field header
    out <<
      indent() << "if ((ret = thrift_protocol_write_field_begin (protocol, " <<
      "\"" << (*f_iter)->get_name() << "\", " <<
      type_to_enum((*f_iter)->get_type()) << ", " <<
      (*f_iter)->get_key() << ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;
    // Write field contents
    generate_serialize_field(out, *f_iter, this_name, "", error_ret);
    // Write field closer
    out <<
      indent() << "if ((ret = thrift_protocol_write_field_end (protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;
    if ((*f_iter)->get_req() == t_field::T_OPTIONAL) {
      indent_down();
      indent(out) << '}' << endl;
    }
  }

  // Write the struct map
  out <<
    indent() << "if ((ret = thrift_protocol_write_field_stop(protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl <<
    indent() << "if ((ret = thrift_protocol_write_struct_end(protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl;

  if (is_function)
    indent(out) << "return xfer;" << endl;

  indent_down();
  indent(out) <<
    "}" << endl <<
    endl;
}

/**
 * Makes a helper function to gen a struct reader.
 *
 * @param out Stream to write to
 * @param tstruct The struct
 */
void t_c_generator::generate_struct_reader(ofstream& out, t_struct* tstruct,
                                           string this_name, string this_get,
                                           bool is_function) {
  string name = tstruct->get_name();
  string name_u = initial_caps_to_underscores (name);
  string name_uc = to_upper_case (name_u);

  int error_ret = 0;

  if (is_function) {
    error_ret = -1;
    indent(out) <<
      "gint32 " << this->nspace_u << name_u << 
      "_read (ThriftStruct * object, ThriftProtocol * protocol, GError ** error)" << endl;
  }

  indent(out) << "{" << endl;
  indent_up();

  const vector<t_field*>& fields = tstruct->get_members();
  vector<t_field*>::const_iterator f_iter;

  // Declare stack tmp variables
  out <<
    indent() << "gint32 ret;" << endl <<
    indent() << "gint32 xfer = 0;" << endl <<
    indent() << "gchar * name;" << endl <<
    indent() << "ThriftType ftype;" << endl <<
    indent() << "gint16 fid;" << endl <<
    endl <<
    indent() << "if ((ret = thrift_protocol_read_struct_begin (protocol, &name, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl <<
    indent() << "if (name) g_free (name);" << endl << 
    indent() << this_get << endl;

  // Required variables aren't in __isset, so we need tmp vars to check them.
  // TODO: optional vars
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    if ((*f_iter)->get_req() == t_field::T_REQUIRED)
      indent(out) << "bool isset_" << (*f_iter)->get_name() << " = false;" << endl;
  }
  out << endl;


  // Loop over reading in fields
  indent(out) <<
    "while (1)" << endl;
  scope_up(out);

  // Read beginning field marker
  out <<
    indent() << "if ((ret = thrift_protocol_read_field_begin (protocol, &name, &ftype, &fid, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl <<
    indent() << "if (name) g_free (name);" << endl;

  // Check for field STOP marker
  out <<
    indent() << "if (ftype == T_STOP) {" << endl <<
    indent() << "  break;" << endl <<
    indent() << "}" << endl;

  // Switch statement on the field we are reading
  indent(out) <<
    "switch (fid)" << endl;

  scope_up(out);

  // Generate deserialization code for known cases
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {

    indent(out) <<
      "case " << (*f_iter)->get_key() << ":" << endl;
    indent_up();
    indent(out) <<
      "if (ftype == " << type_to_enum((*f_iter)->get_type()) << ") {" << endl;
    indent_up();

    generate_deserialize_field(out, *f_iter, this_name, "", error_ret);

    indent_down();
    out <<
      indent() << "} else {" << endl <<
      indent() << "  if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)" << endl <<
      indent() << "    return " << error_ret << ";" << endl <<
      indent() << "  xfer += ret;" << endl <<
      indent() << "}" << endl <<
      indent() << "break;" << endl;
    indent_down();
  }

  // In the default case we skip the field
  out <<
    indent() << "default:" << endl <<
    indent() << "  if ((ret = thrift_protocol_skip (protocol, ftype, error)) < 0)" << endl <<
    indent() << "    return " << error_ret << ";" << endl <<
    indent() << "  xfer += ret;" << endl <<
    indent() << "  break;" << endl;

  scope_down(out);

  // Read field end marker
  out <<
    indent () << "if ((ret = thrift_protocol_read_field_end (protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl;

  scope_down(out);

  out <<
    endl <<
    indent() << "if ((ret = thrift_protocol_read_struct_end (protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl <<
    indent() << "xfer += ret;" << endl;

  // Throw if any required fields are missing.
  // We do this after reading the struct end so that
  // there might possibly be a chance of continuing.
  // TODO: fix
  out << endl;
  for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
    if ((*f_iter)->get_req() == t_field::T_REQUIRED)
      out <<
        indent() << "if (!isset_" << (*f_iter)->get_name() << ')' << endl <<
        indent() << "  throw TProtocolException(TProtocolException::INVALID_DATA);" << endl;
  }

  if (is_function)
    indent(out) << "return xfer;" << endl;

  indent_down();
  indent(out) <<
    "}" << endl << endl;
}


/**
 * Serializes a field of any type.
 *
 * @param tfield The field to serialize
 * @param prefix Name to prepend to field name
 */
void t_c_generator::generate_serialize_field(ofstream& out, t_field* tfield,
                                             string prefix, string suffix, 
                                             int error_ret) {
  t_type* type = get_true_type(tfield->get_type());

  string name = prefix + tfield->get_name() + suffix;

  // Do nothing for void types
  if (type->is_void()) {
    throw "CANNOT GENERATE SERIALIZE CODE FOR void TYPE: " + name;
  }

  if (type->is_struct() || type->is_xception()) {
    generate_serialize_struct(out, (t_struct*)type, name, error_ret);
  } else if (type->is_container()) {
    generate_serialize_container(out, type, name, error_ret);
  } else if (type->is_base_type() || type->is_enum()) {

    indent(out) <<
      "if ((ret = thrift_protocol_write_";

    if (type->is_base_type()) {
      t_base_type::t_base tbase = ((t_base_type*)type)->get_base();
      switch (tbase) {
      case t_base_type::TYPE_VOID:
        throw
          "compiler error: cannot serialize void field in a struct: " + name;
        break;
      case t_base_type::TYPE_STRING:
        if (((t_base_type*)type)->is_binary()) {
          out << "binary(protocol, " << name;
        }
        else {
          out << "string(protocol, " << name;
        }
        break;
      case t_base_type::TYPE_BOOL:
        out << "bool(protocol, " << name;
        break;
      case t_base_type::TYPE_BYTE:
        out << "byte(protocol, " << name;
        break;
      case t_base_type::TYPE_I16:
        out << "i16(protocol, " << name;
        break;
      case t_base_type::TYPE_I32:
        out << "i32(protocol, " << name;
        break;
      case t_base_type::TYPE_I64:
        out << "i64(protocol, " << name;
        break;
      case t_base_type::TYPE_DOUBLE:
        out << "double(protocol, " << name;
        break;
      default:
        throw "compiler error: no C writer for base type " + t_base_type::t_base_name(tbase) + name;
      }
    } else if (type->is_enum()) {
      out << "i32(protocol, (gint32)" << name;
    }
    out << ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl;

  } else {
    printf("DO NOT KNOW HOW TO SERIALIZE FIELD '%s' TYPE '%s'\n",
           name.c_str(), type_name(type).c_str());
  }
}

/**
 * Serializes all the members of a struct.
 *
 * @param tstruct The struct to serialize
 * @param prefix  String prefix to attach to all fields
 */
void t_c_generator::generate_serialize_struct(ofstream& out, t_struct* tstruct,
                                              string prefix, int error_ret) {
  out <<
    indent() << "if ((ret = thrift_struct_write (THRIFT_STRUCT (" << prefix << "), protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl << 
    indent() << "xfer += ret;" << endl;
}

void t_c_generator::generate_serialize_container(ofstream& out, t_type* ttype,
                                                 string prefix, int error_ret) {
  scope_up(out);

  if (ttype->is_map()) {
    string length = "g_hash_table_size ((GHashTable *)" + prefix + ")";
    out <<
      indent() << "if ((ret = thrift_protocol_write_map_begin(protocol, " <<
      type_to_enum(((t_map*)ttype)->get_key_type()) << ", " <<
      type_to_enum(((t_map*)ttype)->get_val_type()) << ", (gint32)" << length <<
      ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << "GHashTableIter iter;" << endl <<
      indent() << "gpointer key, value;" << endl <<
      indent() << "g_hash_table_iter_init (&iter, (GHashTable *)" << prefix << ");" << endl <<
      indent() << "while (g_hash_table_iter_next (&iter, &key, &value))" << endl;

    scope_up(out);
    generate_serialize_map_element(out, (t_map*)ttype, "key", "value", error_ret);
    scope_down(out);

    out <<
      indent() << "if ((ret = thrift_protocol_write_map_end(protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;

  } else if (ttype->is_set()) {
    string length = "g_hash_table_size ((GHashTable *)" + prefix + ")";
    out <<
      indent() << "if ((ret = thrift_protocol_write_set_begin(protocol, " <<
      type_to_enum(((t_set*)ttype)->get_elem_type()) << ", (gint32)" << length <<
      ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << "GHashTableIter iter;" << endl <<
      indent() << "gpointer key, value;" << endl <<
      indent() << "g_hash_table_iter_init (&iter, (GHashTable *)" << prefix << ");" << endl <<
      indent() << "while (g_hash_table_iter_next (&iter, &key, &value))" << endl;

    scope_up(out);
    generate_serialize_set_element(out, (t_set*)ttype, "key", error_ret);
    scope_down(out);

    out <<
      indent() << "if ((ret = thrift_protocol_write_set_end(protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;

  } else if (ttype->is_list()) {
    string length = prefix + "->len";
    out <<
      indent() << "if ((ret = thrift_protocol_write_list_begin(protocol, " <<
      type_to_enum(((t_list*)ttype)->get_elem_type()) << ", (gint32)" <<
      length << ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << "guint i;" << endl <<
      indent() << "for (i = 0; i < " << length << "; i++)" << endl;

    scope_up(out);
    generate_serialize_list_element(out, (t_list*)ttype, prefix, "i", error_ret);
    scope_down(out);

    out <<
      indent() << "if ((ret = thrift_protocol_write_list_end(protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;
  }

  scope_down(out);
}

/**
 * Serializes the members of a map.
 *
 */
void t_c_generator::generate_serialize_map_element(ofstream& out,
                                                   t_map* tmap,
                                                   string key,
                                                   string value,
                                                   int error_ret) {
  t_field kfield(tmap->get_key_type(), key);
  generate_serialize_field(out, &kfield, "", "", error_ret);

  t_field vfield(tmap->get_val_type(), value);
  generate_serialize_field(out, &vfield, "", "", error_ret);
}

/**
 * Serializes the members of a set.
 */
void t_c_generator::generate_serialize_set_element(ofstream& out,
                                                     t_set* tset,
                                                     string element,
                                                     int error_ret) {
  t_field efield(tset->get_elem_type(), element);
  generate_serialize_field(out, &efield, "", "", error_ret);
}

/**
 * Serializes the members of a list.
 */
void t_c_generator::generate_serialize_list_element(ofstream& out,
                                                    t_list* tlist,
                                                    string list,
                                                    string index, 
                                                    int error_ret) {
  t_field efield(tlist->get_elem_type(), 
                 "g_ptr_array_index((GPtrArray *)" + list + ", " + index + ")");
  generate_serialize_field(out, &efield, "", "", error_ret);
}

/**
 * Deserializes a field of any type.
 */
void t_c_generator::generate_deserialize_field(ofstream& out, t_field* tfield,
                                               string prefix, string suffix,
                                               int error_ret) {
  t_type* type = get_true_type(tfield->get_type());

  if (type->is_void()) {
    throw "CANNOT GENERATE DESERIALIZE CODE FOR void TYPE: " +
      prefix + tfield->get_name();
  }

  string name = prefix + tfield->get_name() + suffix;

  if (type->is_struct()) {
    generate_deserialize_struct(out, (t_struct*)type, name, error_ret);
  } else if (type->is_xception()) {
    generate_deserialize_xception(out, (t_struct*)type, name, error_ret);
  } else if (type->is_container()) {
    generate_deserialize_container(out, type, name, error_ret);
  } else if (type->is_base_type()) {
    indent(out) << "if ((ret = thrift_protocol_read_";
    t_base_type::t_base tbase = ((t_base_type*)type)->get_base();
    switch (tbase) {
    case t_base_type::TYPE_VOID:
      throw "compiler error: cannot serialize void field in a struct: " + name;
      break;
    case t_base_type::TYPE_STRING:
      if (((t_base_type*)type)->is_binary()) {
        /* TODO: what about len */
        out << "binary (protocol, &" << name;
      }
      else {
        out << "string (protocol, &" << name;
      }
      break;
    case t_base_type::TYPE_BOOL:
      out << "bool (protocol, &" << name;
      break;
    case t_base_type::TYPE_BYTE:
      out << "byte (protocol, &" << name;
      break;
    case t_base_type::TYPE_I16:
      out << "i16 (protocol, &" << name;
      break;
    case t_base_type::TYPE_I32:
      out << "i32 (protocol, &" << name;
      break;
    case t_base_type::TYPE_I64:
      out << "i64 (protocol, &" << name;
      break;
    case t_base_type::TYPE_DOUBLE:
      out << "double (protocol, &" << name;
      break;
    default:
      throw "compiler error: no C reader for base type " + t_base_type::t_base_name(tbase) + name;
    }
    out << ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;
  } else if (type->is_enum()) {
    string t = tmp("ecast");
    out <<
      indent() << "gint32 " << t << ";" << endl <<
      indent() << "if ((ret = thrift_protocol_read_i32 (protocol, &" << t << ", error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << name << " = (" << type_name(type) << ")" << t << ";" << endl;
  } else {
    printf("DO NOT KNOW HOW TO DESERIALIZE FIELD '%s' TYPE '%s'\n",
           tfield->get_name().c_str(), type_name(type).c_str());
  }
}

/**
 * Generates an unserializer for a variable. This makes two key assumptions,
 * first that there is a const char* variable named data that points to the
 * buffer for deserialization, and that there is a variable protocol which
 * is a reference to a TProtocol serialization object.
 */
void t_c_generator::generate_deserialize_struct(ofstream& out, 
                                                t_struct* tstruct,
                                                string prefix, int error_ret) {
  out <<
    indent() << "if ((ret = thrift_struct_read (THRIFT_STRUCT (" << prefix << "), protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl << 
    indent() << "xfer += ret;" << endl;
}

void t_c_generator::generate_deserialize_xception(ofstream& out, 
                                                  t_struct* tstruct,
                                                  string prefix, 
                                                  int error_ret) {

  string name_u = initial_caps_to_underscores (tstruct->get_name());

  out <<
    indent() << "if ((ret = " << this->nspace_u << name_u << "_read (&" << prefix << ", protocol, error)) < 0)" << endl <<
    indent() << "  return " << error_ret << ";" << endl << 
    indent() << "xfer += ret;" << endl;
}

void t_c_generator::generate_deserialize_container(ofstream& out, t_type* ttype,
                                                   string prefix, 
                                                   int error_ret) {
  /* TODO: treating everything as pointers is not going to work as a long term
   * solution here */

  scope_up(out);

  // Declare variables, read header
  if (ttype->is_map()) {
    out <<
      indent() << "guint32 size;" << endl <<
      indent() << "ThriftType key_type;" << endl <<
      indent() << "ThriftType value_type;" << endl <<
      indent() << "if ((ret = thrift_protocol_read_map_begin (protocol, &key_type, &value_type, &size, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << "guint32 i;" << endl <<
      indent() << "*_return = g_hash_table_new (NULL, NULL);" << endl <<
      indent() << "for (i = 0; i < size; ++i)" << endl;

    scope_up(out);
    generate_deserialize_map_element(out, (t_map*)ttype, prefix, error_ret);
    scope_down(out);

    out << 
      indent() << "if ((ret = thrift_protocol_read_map_end (protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;

  } else if (ttype->is_set()) {
    out <<
      indent() << "guint32 size;" << endl <<
      indent() << "ThriftType element_type;" << endl <<
      indent() << "if ((ret = thrift_protocol_read_set_begin (protocol, &element_type, &size, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << "guint32 i;" << endl <<
      indent() << "for (i = 0; i < size; ++i)" << endl;

    scope_up(out);
    generate_deserialize_set_element(out, (t_set*)ttype, prefix, error_ret);
    scope_down(out);

    out << 
      indent() << "if ((ret = thrift_protocol_read_set_end (protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;

  } else if (ttype->is_list()) {
    out <<
      indent() << "guint32 size;" << endl <<
      indent() << "ThriftType element_type;" << endl <<
      indent() << "if ((ret = thrift_protocol_read_list_begin (protocol, &element_type, &size, error)) < 0)" << endl << 
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl <<
      indent() << "guint32 i;" << endl <<
      indent() << "for (i = 0; i < size; ++i)" << endl;

    scope_up(out);
    generate_deserialize_list_element(out, (t_list*)ttype, prefix, "i", 
                                      error_ret);
    scope_down(out);

    out << 
      indent() << "if ((ret = thrift_protocol_read_list_end (protocol, error)) < 0)" << endl <<
      indent() << "  return " << error_ret << ";" << endl <<
      indent() << "xfer += ret;" << endl;

  }

  scope_down(out);
}


/**
 * Generates code to deserialize a map
 */
void t_c_generator::generate_deserialize_map_element(ofstream& out,
                                                       t_map* tmap,
                                                       string prefix,
                                                       int error_ret) {
  t_field fkey(tmap->get_key_type(), "key");
  t_field fval(tmap->get_val_type(), "val");

  out <<
    indent() << declare_field(&fkey, true) << endl <<
    indent() << declare_field(&fval, true) << endl;

  generate_deserialize_field(out, &fkey, "", "", error_ret);
  generate_deserialize_field(out, &fval, "", "", error_ret);

  indent(out) << 
    "g_hash_table_insert ((GHashTable *)" << prefix << ", (gpointer)key, (gpointer)val);" << endl;
}

void t_c_generator::generate_deserialize_set_element(ofstream& out,
                                                       t_set* tset,
                                                       string prefix,
                                                       int error_ret) {
  t_field felem(tset->get_elem_type(), "elem");

  indent(out) <<
    declare_field(&felem, true) << endl;

  generate_deserialize_field(out, &felem, "", "", error_ret);

  indent(out) <<
    "g_hash_table_insert((GHashTable *)" << prefix << ", (gpointer)elem, NULL);" << endl;
}

void t_c_generator::generate_deserialize_list_element(ofstream& out,
                                                      t_list* tlist,
                                                      string prefix,
                                                      string index,
                                                      int error_ret) {
  string elem = tmp("_elem");
  t_field felem(tlist->get_elem_type(), elem);
  indent(out) << declare_field(&felem, true) << endl;
  generate_deserialize_field(out, &felem, "", "", error_ret);
  indent(out) << "g_ptr_array_add (" << prefix << ", " << elem << ");" << endl;
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
 * Generates a thrift service. In C, this comprises an entirely separate
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
    "#include \"" << svcname << ".h\"" << endl << 
    "#include \"thrift_client.h\"" << endl <<
    "#include <string.h>" << endl <<
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

  string service_name_u = initial_caps_to_underscores(service_name_);
  string service_name_uc = to_upper_case(service_name_u);

  f_header_ <<
    "GType thrift_" << service_name_u << "_client_get_type (void);" << endl <<
    "#define THRIFT_" << service_name_uc << "_TYPE_CLIENT (thrift_" << service_name_u << "_client_get_type ())" << endl <<
    "#define THRIFT_" << service_name_uc << "_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_" << service_name_uc << "_TYPE_CLIENT, Thrift" << service_name_ << "ClientClass))" << endl <<
    "#define THRIFT_" << service_name_uc << "_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_" << service_name_uc << "_TYPE_CLIENT, Thrift" << service_name_ << "ClientClass))" << endl <<
    "#define THRIFT_" << service_name_uc << "_IS_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_" << service_name_uc << "_TYPE_CLIENT))" << endl << 
    "#define THRIFT_" << service_name_uc << "_IS_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_" << service_name_uc << "_TYPE_CLIENT))" << endl <<
    "#define THRIFT_" << service_name_uc << "_CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_" << service_name_uc << "_TYPE_CLIENT, Thrift" << service_name_ << "ClientClass))" << endl << 
    endl;

  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::const_iterator f_iter;
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    string funname = initial_caps_to_underscores((*f_iter)->get_name());
    t_function send_function(g_type_void,
                             string("send_") + funname,
                             (*f_iter)->get_arglist());
    indent(f_header_) << function_signature(*f_iter) << ";" << endl;
    indent(f_header_) << function_signature(&send_function) << ";" << endl;
    if (!(*f_iter)->is_async()) {
      t_struct noargs(program_);
      t_function recv_function((*f_iter)->get_returntype(),
                               string("recv_") + funname,
                               &noargs);
      indent(f_header_) << function_signature(&recv_function) << ";" << endl;
    }
  }

  f_header_ << endl;

  // Generate client method implementations
  for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
    string name = (*f_iter)->get_name();
    string funname = initial_caps_to_underscores(name);

    // Open function
    indent(f_service_) <<
      function_signature(*f_iter) << endl;
    scope_up(f_service_);
    f_service_ <<
      indent() << "if (!" << this->nspace_u << "send_" << funname << " (client";

    // Get the struct of function call params
    t_struct* arg_struct = (*f_iter)->get_arglist();

    // Declare the function arguments
    const vector<t_field*>& fields = arg_struct->get_members();
    vector<t_field*>::const_iterator fld_iter;
    for (fld_iter = fields.begin(); fld_iter != fields.end(); ++fld_iter) {
      f_service_ << ", " << (*fld_iter)->get_name();
    }
    f_service_ << ", error))" << endl <<
      indent() << "  return 0;" << endl;

    if (!(*f_iter)->is_async()) {
      string ret = (*f_iter)->get_returntype()->is_void() ? "" : ", _return";
      f_service_ << 
        indent() << "if (!" << this->nspace_u << "recv_" << funname << " (client" << ret << ", error))" << endl <<
        indent() << "  return 0;" << endl;
    }
    indent(f_service_) << "return 1;" << endl;
    scope_down(f_service_);
    f_service_ << endl;

    // Function for sending
    t_function send_function(g_type_void,
                             string("send_") + funname,
                             (*f_iter)->get_arglist());

    // Open the send function
    indent(f_service_) <<
      function_signature(&send_function) << endl;
    scope_up(f_service_);

    // Serialize the request
    f_service_ <<
      indent() << "gint32 cseqid = 0;" << endl <<
      indent() << "ThriftProtocol * protocol = THRIFT_CLIENT (client)->protocol;" << endl <<
      endl <<
      indent() << "if (thrift_protocol_write_message_begin (protocol, \"" << name << "\", T_CALL, cseqid, error) < 0)" << endl <<
      indent() << "  return 0;" << endl <<
      endl;

    generate_struct_writer (f_service_, arg_struct, "", "", false);

    f_service_ <<
      indent() << "if (thrift_protocol_write_message_end (protocol, error) < 0)" << endl <<
      indent() << "  return 0;" << endl <<
      endl <<
      indent () << "if (!thrift_transport_flush (protocol->transport, error))" << endl <<
      indent() << "  return 0;" << endl <<
      indent () << "if (!thrift_transport_write_end (protocol->transport, error))" << endl <<
      indent() << "  return 0;" << endl <<
      endl <<
      indent() << "return 1;" << endl;

    scope_down(f_service_);
    f_service_ << endl;

    // Generate recv function only if not an async function
    if (!(*f_iter)->is_async()) {
      t_struct noargs(program_);
      t_function recv_function((*f_iter)->get_returntype(),
                               string("recv_") + funname, &noargs);
      // Open function
      indent(f_service_) <<
        function_signature(&recv_function) << endl;
      scope_up(f_service_);

      f_service_ <<
        endl <<
        indent() << "gint32 rseqid;" << endl <<
        indent() << "gchar * fname;" << endl <<
        indent() << "ThriftMessageType mtype;" << endl <<
        indent() << "ThriftProtocol * protocol = THRIFT_CLIENT (client)->protocol;" << endl <<
        endl <<
        indent() << "if (thrift_protocol_read_message_begin (protocol, &fname, &mtype, &rseqid, error) < 0)" << endl <<
        indent() << "  return 0;" << endl <<
        endl <<
        indent() << "if (mtype == T_EXCEPTION) {" << endl <<
        indent() << "  /* TODO: ThriftApplicationException x;" << endl <<
        indent() << "  x.read(iprot_); */" << endl <<
        indent() << "  thrift_protocol_skip (protocol, T_STRUCT, error);" << endl <<
        indent() << "  thrift_protocol_read_message_end (protocol, error);" << endl <<
        indent() << "  thrift_transport_read_end (protocol->transport, error);" << endl <<
        indent() << "  /* TODO: error handling throw x; */" << endl <<
        indent() << "  return 0;" << endl <<
        indent() << "} else if (mtype != T_REPLY) {" << endl <<
        indent() << "  thrift_protocol_skip (protocol, T_STRUCT, error);" << endl <<
        indent() << "  thrift_protocol_read_message_end (protocol, error);" << endl <<
        indent() << "  thrift_transport_read_end (protocol->transport, error);" << endl <<
        indent() << "  /* TODO: error handling throw facebook::thrift::TApplicationException(facebook::thrift::TApplicationException::INVALID_MESSAGE_TYPE); */" << endl <<
        indent() << "  return 0;" << endl <<
        indent() << "} else if (strncmp (fname, \"" << name << "\", " << name.length () << ") != 0) {" << endl <<
        indent() << "  thrift_protocol_skip (protocol, T_STRUCT, error);" << endl <<
        indent() << "  thrift_protocol_read_message_end (protocol, error);" << endl <<
        indent() << "  thrift_transport_read_end (protocol->transport, error);" << endl <<
        indent() << "  /* TODO: error handling throw facebook::thrift::TApplicationException(facebook::thrift::TApplicationException::WRONG_METHOD_NAME); */" << endl <<
        indent() << "  return 0;" << endl <<
        indent() << "}" << endl <<
        indent() << "if (fname) g_free (fname);" << endl << 
        endl;

      {
        t_struct result(program_, tservice->get_name() + "_" + 
                        (*f_iter)->get_name() + "_result");
        t_field success((*f_iter)->get_returntype(), "*_return", 0);
        if (!(*f_iter)->get_returntype()->is_void()) {
          result.append(&success);
        }

        generate_struct_reader (f_service_, &result, "", "", false);
      }

      // TODO: error handling, throw on exceptions etc.
      f_service_ <<
        indent() << "if (thrift_protocol_read_message_end (protocol, error) < 0)" << endl <<
        indent() << "  return 0;" << endl <<
        endl <<
        indent() << "if (!thrift_transport_read_end (protocol->transport, error))" << endl <<
        indent() << "  return 0;" << endl <<
        endl;

      // Close function
      indent(f_service_) << "return 1;" << endl;
      scope_down(f_service_);
      f_service_ << endl;
    }
  }

  f_service_ <<
    "GType thrift_" << service_name_u << "_client_get_type (void)" << endl <<
    "{" << endl <<
    "  static GType type = 0;" << endl << 
    endl <<
    "  if (type == 0) " << endl <<
    "  {" << endl <<
    "    static const GTypeInfo type_info = " << endl <<
    "    {" << endl <<
    "      sizeof (Thrift" << service_name_ << "ClientClass)," << endl <<
    "      NULL, /* base_init */" << endl <<
    "      NULL, /* base_finalize */" << endl <<
    "      NULL, /* class_init */" << endl <<
    "      NULL, /* class_finalize */" << endl <<
    "      NULL, /* class_data */" << endl <<
    "      sizeof (Thrift" << service_name_ << "Client)," << endl <<
    "      0, /* n_preallocs */" << endl <<
    "      NULL, /* instance_init */" << endl <<
    "      NULL, /* value_table */" << endl <<
    "    };" << endl << 
    endl <<
    "    type = g_type_register_static (THRIFT_TYPE_CLIENT, " << endl <<
    "                                   \"Thrift" << service_name_ << "ClientType\"," << endl << 
    "                                   &type_info, 0);" << endl <<
    "  }" << endl << 
    endl << 
    "  return type;" << endl << 
    "}" << endl << 
    endl;
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

  bool has_return = !ttype->is_void();
  bool has_args = arglist->get_members().size() == 0;
  return
    "gboolean " + this->nspace_u + fname + " (Thrift" + service_name_ +
    "Client * client" + 
    (has_return ? ", " + type_name(ttype) + "* _return" : "") +
    (has_args ? "" : (", " + argument_list(arglist))) + ", GError ** error)";
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
    /* TODO: get rid of name_params */
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
    result += "*";
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
        result += " = NULL";
        break;
      case t_base_type::TYPE_BOOL:
        result += " = 0";
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
        throw "compiler error: no C initializer for base type " + t_base_type::t_base_name(tbase);
      }
    } else if (type->is_enum()) {
      result += " = (" + type_name(type) + ")0";
    } else if (tfield->get_type()->is_struct()) {
      string name_uc = to_upper_case
        (initial_caps_to_underscores(tfield->get_type()->get_name()));
      result += " = g_object_new (" + this->nspace_uc + "TYPE_" + name_uc + 
        ", NULL);";
    } else if (tfield->get_type()->is_xception()) {
      /* GError's always need to be set to NULL */  
      result += " = NULL;";
    }
  }

  
  if (!reference) {
    result += ";";
  }
  return result;
}

/**
 * Returns a C type name
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

  if (ttype->is_xception()) {
    return "GError *";
  }

  // Check for a custom overloaded C name
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
 * Returns the C type that corresponds to the thrift type.
 *
 * @param tbase The base type
 * @return Explicit C type, i.e. "gint32"
 */
string t_c_generator::base_type_name(t_base_type::t_base tbase) {
  switch (tbase) {
  case t_base_type::TYPE_VOID:
    return "void";
  case t_base_type::TYPE_STRING:
    return "gchar *";
  case t_base_type::TYPE_BOOL:
    return "gboolean";
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
    throw "compiler error: no C base type name for base type " + t_base_type::t_base_name(tbase);
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
      render << ((value->get_integer() != 0) ? 1 : 0);
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
