#include "t_const.h"
#include "generate/t_generator.h"

std::string t_const_value::get_string(const t_generator *generator) const {
  return generator->escape_string(stringVal_);
}

