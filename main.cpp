#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

fs::path get_directory_name(std::string const& name) {
  std::string temp;
  temp.push_back(std::toupper(name[0]));

  for (int i {1}; i < name.length(); i++) {
    char const c = name[i];
    if (c != '_') {
      temp.push_back(std::tolower(c));
    } else {
      temp.push_back(std::toupper(name[++i]));
    }
  }

  return temp;
}

void get_inputs(std::string& name, std::string& cxx) {
  std::cout << "PROJECT_NAME: ?" << std::endl;
  std::getline(std::cin, name);
  std::string directory_name = get_directory_name(name);

  std::cout << "CXX version: ?" << std::endl;
  std::string temp;
  std::cin >> temp;

  if (!temp.empty())
    cxx = temp;

  if (name.find(' ') != std::string::npos) {
    std::cout << "PROJECT_NAME cannot contain spaces!" << std::endl;
    std::exit(0);
  }
}

int main() {
  std::string name, cxx;
  get_inputs(name, cxx);

  std::string const cmake_contents =
"cmake_minimum_required(VERSION 3.9.1)\n"

"# Configuration\n"
"set(PROJECT_NAME " + name + ")\n"
"set(HEADER_DIRECTORY \"${CMAKE_SOURCE_DIR}/include\")\n"

"project(${PROJECT_NAME} CXX)\n"

"# Standard\n"
"set(CMAKE_CXX_STANDARD " + cxx + ")\n"
"set(CMAKE_CXX_STANDARD_REQUIRED True)\n"

"# Sources\n"
"set(SOURCES\n"
"\tsrc/main.cpp)\n"

"add_executable(${PROJECT_NAME}\n"
"\t${SOURCES})\n"

"# Include\n"
"target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER_DIRECTORY})\n"

"# Output directory\n"
"set_target_properties(${PROJECT_NAME}\n"
"\tPROPERTIES\n"
"\tRUNTIME_OUTPUT_DIRECTORY \"${PROJECT_SOURCE_DIR}/bin\")\n";

  auto directory = get_directory_name(name);

  create_directory(directory);
  create_directory(directory / "bin");
  create_directory(directory / "build");
  create_directory(directory / "include");
  create_directory(directory / "src");

  std::ofstream file {directory / "CMakeLists.txt"};
  file << cmake_contents;

  std::string main_contents = R"***(#include <iostream>
int main()
{
  std::cout << "Hello World" << std::endl;
}
)***";

  std::ofstream main {directory / "src" / "main.cpp"};
  main << main_contents;
}