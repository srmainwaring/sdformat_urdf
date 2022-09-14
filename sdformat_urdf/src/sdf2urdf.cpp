
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <tinyxml2.h>

#include <urdf_parser/urdf_parser.h>

#include <urdf_model/model.h>
#include <urdf_model/types.h>
#include <sdformat_urdf/sdformat_urdf.hpp>

std::string get_file(const char * path)
{
  std::ifstream file_reader(path, std::ifstream::in);
  std::stringstream buffer;
  buffer << file_reader.rdbuf();
  return buffer.str();
}

int main(int argc, char ** argv)
{
  // read model sdf file
  if (argc < 2)
  {
    std::cout << "Must provide filename\n";
    return -1;
  }

  std::string filename = argv[1];
  std::cout << "Reading: " << filename << "\n";

  // parse sdf to urdf
  sdf::Errors errors;
  urdf::ModelInterfaceSharedPtr model = sdformat_urdf::parse(
    get_file(filename.c_str()), errors);

  if (!errors.empty())
  {
    std::cout << "Failed to parse: " << filename << "\n";
    for (auto& e : errors)
    {
      std::cout << e.Message() << "\n";
    }
    return -1;
  }

  // urdfdom is using tinyxml (not tinyxml2?)
  auto xmldoc = urdf::exportURDF(model);

  // write to file
  std::string outfile = std::string(filename).append(".urdf");
  xmldoc->SaveFile(outfile);



  return 0;
}
