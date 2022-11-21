//
// Created by paulg on 2/10/2022.
//
#include "assert.h"
#include "unordered_map"
#include <string>
#include <vector>

#define ASSERT(condition, message)                                             \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf((message));                                                       \
    }                                                                          \
    assert((condition));                                                       \
  } while (false)

#ifndef LOADDAE_STRUCT_TYPES_H
#define LOADDAE_STRUCT_TYPES_H

using namespace std;

enum ArrayTypes {
  FLOAT,
  NAME,
};

struct Source {
  void *array = nullptr;
  ArrayTypes type;
  string params;
  int count = 0;
  int stride = 0;
};

struct Input {
  int offset = 0;
  Source *source = nullptr;
  string semantic;
  int set = 0;
};

struct Light {
  string name;
  float color[3] = {0};
  float direction[3] = {0};
  string type;
  float transform[4 * 4] = {0};
};

struct Node {
  float transform[4 * 4] = {0};
  vector<Node *> children;
  Node *parent = nullptr;
  string type;
  string id;
  string sid;
};

struct Mesh {
  string name;
  int numFaces;
  vector<int> vertOrder;

  vector<float> vertsX;
  vector<float> vertsY;
  vector<float> vertsZ;

  vector<float> normalsX;
  vector<float> normalsY;
  vector<float> normalsZ;

  vector<float> textureCoordsU;
  vector<float> textureCoordsV;

  float transform[4 * 4] = {0};

  bool mark_exclude = false;
};

struct Controller {
  string name;
  Mesh *skin;
  float transform[4 * 4] = {0};
  float bindShapeMatrix[16] = {0};
  unordered_map<string, vector<float>> influences;
  unordered_map<string, vector<float>> joint2BindInv;
  vector<Node *> skeletons;
  int numVerts = 0;
};

struct Camera {
  string name;
  float xfov = 0;
  float aspect_ratio = 0;
  float znear = 0;
  float zfar = 0;
  float transform[4 * 4] = {0};
};

void allocateMesh(Mesh *mesh, int numFaces, int numNormals,
                  int numtextureCoords) {
  mesh->numFaces = numFaces;

  mesh->vertsX.reserve(numFaces);
  mesh->vertsY.reserve(numFaces);
  mesh->vertsZ.reserve(numFaces);

  mesh->normalsX.reserve(numNormals);
  mesh->normalsY.reserve(numNormals);
  mesh->normalsZ.reserve(numNormals);

  mesh->textureCoordsU.reserve(numtextureCoords);
  mesh->textureCoordsV.reserve(numtextureCoords);
}

void addInput(Mesh *mesh, Input *input, int index) {
  int startInd = input->source->stride * index;
  float *data = (float *)input->source->array;
  if (input->semantic == "VERTEX") {
    ASSERT(input->source->params == "XYZ", "params in wrong order");
    // assume params==XYZ
    mesh->vertsX.emplace_back(data[startInd]);
    mesh->vertsY.emplace_back(data[startInd + 1]);
    mesh->vertsZ.emplace_back(data[startInd + 2]);
    mesh->vertOrder.push_back(index + 1); // matlab is 1 indexed
  } else if (input->semantic == "NORMAL") {
    ASSERT(input->source->params == "XYZ", "params in wrong order");
    // assume params==XYZ
    mesh->normalsX.emplace_back(data[startInd]);
    mesh->normalsY.emplace_back(data[startInd + 1]);
    mesh->normalsZ.emplace_back(data[startInd + 2]);

  } else if (input->semantic == "TEXCOORD") {
    ASSERT(input->source->params == "ST", "params in wrong order");
    mesh->textureCoordsU.emplace_back(data[startInd]);
    mesh->textureCoordsV.emplace_back(data[startInd + 1]);
  } else {
    ASSERT(0, "The input semantic must be VERTEX, NORMAL, or TEXTCOORD");
  }
}

void addInput(Controller *controller,
              unordered_map<int, Input> &controllerInputs, int jointIndex,
              int weightIndex, int vertInd) {
  //    int startInd = input->source->stride*index;
  //    float* data = (float *) input->source->array;
  string *names;
  float *weights;
  for (int k = 0; k < 2; k++) {
    if (controllerInputs[k].semantic == "JOINT") {
      names = (string *)controllerInputs[k].source->array;
    } else {
      weights = (float *)controllerInputs[k].source->array;
    }
  }

  controller->influences[names[jointIndex]][vertInd] = weights[weightIndex];
}

#endif // LOADDAE_STRUCT_TYPES_H
