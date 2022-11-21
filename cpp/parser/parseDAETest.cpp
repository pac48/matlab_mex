//
// Created by paulg on 2/9/2022.
//

#include "parseDAE.cpp"
#include <iostream>

int main(int argn, char** argv){
  if (argn != 2){
    std::cout << "usage: parseDAETest filename \nfilename is the absolute path to the dae file.  ";
    return 1;
  }


    std::string DAE_file_name = argv[1];
    ParseDAE parser;
    parser.parseDAE(string(DAE_file_name));

//    parseDAE("yumi.dae");
//    parseDAE("sawyer.dae");

//    while(meshes.size() > 0){
//        Mesh* mesh = meshes.back();
//        delete mesh;
//        meshes.pop_back();
//    }

    return 0;
}