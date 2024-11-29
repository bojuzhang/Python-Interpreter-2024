#ifndef SCPOE_H
#define SCPOE_H

#include <vector>
#include <any>
#include <string>
#include <unordered_map>

class Scope {
private:
  std::vector<std::unordered_map<std::string, std::any>> varmap_{};

public:
  Scope() {
    varmap_.emplace_back();
  }

  void varRegister(const std::string &varname, std::any vardata) ;
  std::any varQuery(const std::string varname) ;
  void varSet(const std::string &varname, std::any vardata) ;
  
  bool varFind(const std::string &varname) ;

  void layercreate() {
    varmap_.emplace_back();
  }
  void layerdelete() {
    varmap_.pop_back();
  }
};

#endif //SCOPE_H