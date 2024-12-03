#include "Scope.h"
#include <iostream>

void Scope::varRegister(const std::string &varname, std::any vardata) {
  varmap_.back()[varname] = vardata;
}

std::any Scope::varQuery(const std::string varname) {
  auto now = varmap_.back();
  // std::cerr << varmap_.size() << "\n";
  if (!now.empty() && now.find(varname) != now.end()) {
    return now[varname];
  } else {
    return varmap_[0][varname];
  }
}

void Scope::varSet(const std::string &varname, std::any vardata) {
  // std::cerr << "test " << varname << " " << vardata.type().name() << " " << varmap_.size() << "\n";
  if (!varmap_.back().empty() &&
      varmap_.back().find(varname) != varmap_.back().end()) {
    varmap_.back()[varname] = vardata;
  } else if (!varmap_[0].empty() &&
             varmap_[0].find(varname) != varmap_[0].end()) {
    varmap_[0][varname] = vardata;
  } else {
    varmap_.back()[varname] = vardata;
  }
  // std::cerr << "end varset\n";
}

bool Scope::varFind(const std::string &varname) {
  return (!varmap_.back().empty() &&
          varmap_.back().find(varname) != varmap_.back().end()) ||
         (!varmap_[0].empty() && varmap_[0].find(varname) != varmap_[0].end());
}