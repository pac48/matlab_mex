//
// Created by paulg on 2/9/2022.
//
#include "parseDAE.h"


using namespace tinyxml2;
using namespace std;

Cases ParseDAE::getCase(const char *type) {
  auto key = string(type);
  if (caseMap.find(key) != caseMap.end()) {
    return caseMap[string(type)];
  } else {
    return NONE;
  }
}

template <typename T>
void ParseDAE::fillArray(const char *data, T *array, int count) {
  stringstream ss(data);
  for (int i = 0; i < count; i++) {
    ss >> array[i];
  }
}

void ParseDAE::parseTransform(const XMLNode *cur, Node *node) {
  ASSERT(cur->ToElement()->FindAttribute("sid") &&
             string(cur->ToElement()->FindAttribute("sid")->Value()) ==
                 "transform",
         "The joint node is formatted incorrectly");
  fillArray<float>(cur->ToElement()->FirstChild()->Value(), node->transform,
                   16);
}

template <typename T>
void ParseDAE::parseArray(const XMLNode *cur, Source *source) {
  const XMLElement *curElem = cur->ToElement();
  int count = stoi(string(curElem->FindAttribute("count")->Value()));
  string data = string(cur->FirstChild()->Value());

  if (is_same<T, float>::value) {
    source->type = FLOAT;
    float *array = new float[count];
    fillArray(cur->FirstChild()->Value(), array, count);
    source->array = array;

  } else if (is_same<T, string>::value) {
    source->type = NAME;
    string *array = new string[count];
    fillArray(cur->FirstChild()->Value(), array, count);
    source->array = array;

  } else {
    ASSERT(0, "The array type of source is neither float or name");
  }
}

void ParseDAE::parseTechnique(const XMLNode *cur, Source *source) {
  const XMLElement *childElem = cur->FirstChild()->ToElement();
  int count = stoi(string(childElem->FindAttribute("count")->Value()));
  int stride = stoi(string(childElem->FindAttribute("stride")->Value()));
  source->stride = stride;
  source->count = count;
  for (const XMLNode *node = childElem->FirstChild(); node;
       node = node->NextSibling()) {
    source->params += node->ToElement()->FindAttribute("name")->Value();
  }
}

template <typename T>
void ParseDAE::parseTechnique(const XMLNode *cur, T *object) {
} // default implementation

void ParseDAE::parseTechnique(const XMLNode *cur, Light *light) {
  const XMLElement *childElem = cur->FirstChildElement()->ToElement();
  ASSERT(string(childElem->Value()) == "directional",
         "make sure light is directional mode");
  light->type = "directional";
  for (const XMLNode *child = childElem->FirstChild(); child;
       child = child->NextSibling()) {
    if (string(child->Value()) == "color") {
      fillArray(child->FirstChild()->Value(), light->color, 3);
    } else {
      ASSERT(0, "light has bad attributes");
    }
  }
}

void ParseDAE::parseTechnique(const XMLNode *cur, Camera *camera) {
  const XMLElement *childElem = cur->FirstChildElement()->ToElement();
  ASSERT(string(childElem->Value()) == "perspective",
         "make sure camera is in perspective mode");

  for (const XMLNode *node = childElem->FirstChild(); node;
       node = node->NextSibling()) {
    if (string(node->Value()) == "xfov") {
      camera->xfov = stof(string(node->FirstChild()->Value()));
    } else if (string(node->Value()) == "aspect_ratio") {
      camera->aspect_ratio = stof(string(node->FirstChild()->Value()));
    } else if (string(node->Value()) == "znear") {
      camera->znear = stof(string(node->FirstChild()->Value()));
    } else if (string(node->Value()) == "zfar") {
      camera->zfar = stof(string(node->FirstChild()->Value()));
    } else {
      ASSERT(0, "camera has bad attributes");
    }
  }
}

void ParseDAE::parseSource(const XMLNode *cur) {
  // TODO add id to source map
  const XMLElement *curElem = cur->ToElement();
   auto source = &source_id_map[string(curElem->FindAttribute("id")->Value())];

  for (const XMLNode *node = cur->FirstChild(); node;
       node = node->NextSibling()) {
    switch (getCase(node->Value())) {
    case FLOATARRAY:
      parseArray<float>(node, source);
      break;
    case NAMEARRAY:
      parseArray<string>(node, source);
      break;
    case TECHNIQUECOMMON:
      parseTechnique(node, source);
      break;
    }
  }
}

void ParseDAE::parseVertices(const XMLNode *cur) {
  auto id = cur->ToElement()->FindAttribute("id")->Value();
  const XMLElement *childElem = cur->FirstChild()->ToElement();
  string sourceID = string(childElem->FindAttribute("source")->Value());
  ASSERT(string(childElem->FindAttribute("semantic")->Value()) == "POSITION",
         "Vertex semantic must be position");
  source_id_map[id] = source_id_map[sourceID.substr(1, sourceID.size() - 1)];
}

void ParseDAE::parseInput2(const XMLNode *cur,
                           unordered_map<int, Input> &inputs, Input &input) {
  inputs[input.offset] = input;
}
void ParseDAE::parseInput2(const XMLNode *cur, vector<Input> &inputs,
                           Input &input) {
  inputs.push_back(input);
}

template <typename T> void ParseDAE::parseInput(const XMLNode *cur, T &inputs) {
  const XMLElement *curElem = cur->ToElement();
  Input input = Input();
  const XMLAttribute *attr = curElem->FindAttribute("semantic");
  if (attr)
    input.semantic = string(attr->Value());
  attr = curElem->FindAttribute("offset");
  if (attr)
    input.offset = stoi(string(attr->Value()));
  attr = curElem->FindAttribute("set");
  if (attr)
    input.set = stoi(string(attr->Value()));
  string sourceID = string(curElem->FindAttribute("source")->Value());
  input.source = &source_id_map[sourceID.substr(1, sourceID.size() - 1)];
  parseInput2(cur, inputs, input);
}

void ParseDAE::parseJoints(const XMLNode *cur, Controller *controller) {
  vector<Input> jointInputs;
  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    switch (getCase(child->Value())) {
    case INPUT:
      parseInput(child, jointInputs);
      break;
    default:
      ASSERT(0, "joints tag should only have input for children");
    }
  }
  Source *jointNames;
  Source *invBindTransforms;
  for (int i = 0; i < 2; i++) {
    if (jointInputs[i].semantic == "JOINT") {
      jointNames = jointInputs[i].source;
    } else {
      invBindTransforms = jointInputs[i].source;
    }
  }

  for (int j = 0; j < jointNames->count; j++) {
    copy((float *)invBindTransforms->array + j * 16,
         ((float *)invBindTransforms->array) + +(j + 1) * 16,
         back_inserter(
             controller->joint2BindInv[((string *)jointNames->array)[j]]));
  }
}

void ParseDAE::parsePrimitives(const XMLNode *cur,
                               unordered_map<int, Input> meshInputs,
                               Mesh *mesh) {
  const char *data = cur->FirstChild()->Value();
  stringstream ss(data);
  while (!ss.eof()) {
    for (int i = 0; i < meshInputs.size(); i++) {
      int index;
      ss >> index;
      addInput(mesh, &meshInputs[i], index);
    }
  }
}

template <typename T>
void ParseDAE::parseTriangles(const XMLNode *cur, T *mesh) {}

void ParseDAE::parseTriangles(const XMLNode *cur, Mesh *mesh) {
  int numFaces = stoi(cur->ToElement()->FindAttribute("count")->Value());
  unordered_map<int, Input> meshInputs;
  allocateMesh(mesh, numFaces * 3, numFaces * 3, numFaces * 3);

  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    switch (getCase(child->Value())) {
    case INPUT:
      parseInput(child, meshInputs);
      break;
    case PRIMITIVE:
      parsePrimitives(child, meshInputs, mesh);
      break;
    }
  }
}

void ParseDAE::parseInfluences(const char *data, int *vcount, int numVerts,
                               Controller *controller,
                               unordered_map<int, Input> &controllerInputs) {

  //    controller->influences

  for (int k = 0; k < 2; k++) {
    if (controllerInputs[k].semantic == "JOINT") {
      string *names = (string *)controllerInputs[k].source->array;
      for (int i = 0; i < controllerInputs[k].source->count; i++) {
        for (int j = 0; j < numVerts; j++) {
          controller->influences[names[i]].push_back(0);
        }
        //                controller->influences[names[i]] = new
        //                float[numVerts];
        //                memset(controller->influences[names[i]], 0,
        //                sizeof(float)*numVerts);
      }
    }
  }

  ASSERT(controllerInputs.size() == 2,
         "There should only be two inputs for the influences");
  stringstream ss(data);

  for (int i = 0; i < numVerts; i++) {
    int numInluences = vcount[i];
    for (int j = 0; j < numInluences; j++) {

      int weightIndex;
      int jointIndex;
      for (int k = 0; k < 2; k++) {
        if (controllerInputs[k].semantic == "JOINT") {
          ss >> jointIndex;
        } else {
          ss >> weightIndex;
        }
      }
      addInput(controller, controllerInputs, jointIndex, weightIndex, i);
    }
  }
}

void ParseDAE::parseVertexWeights(const XMLNode *cur, Controller *controller) {
  int numVerts = stoi(cur->ToElement()->FindAttribute("count")->Value());
  unordered_map<int, Input> controllerInputs;
  int *vcount = new int[numVerts];
  controller->numVerts = numVerts;

  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    switch (getCase(child->Value())) {
    case INPUT:
      parseInput(child, controllerInputs);
      break;
    case VCOUNT:
      fillArray(child->FirstChild()->Value(), vcount, numVerts);
      break;
    case INFLUENCES:
      parseInfluences(child->FirstChild()->Value(), vcount, numVerts,
                      controller, controllerInputs);
      //            case PRIMITIVE:
      //                parsePrimitives(node, controllerInputs, mesh);
      //                break;
    }
  }

  delete[] vcount;
}

template <typename T>
void ParseDAE::parseSkin(const XMLNode *cur, T controller) {}

void ParseDAE::parseSkin(const XMLNode *cur, Controller *controller) {
  string url = string(cur->ToElement()->FindAttribute("source")->Value());
  Mesh *mesh = &mesh_id_map[url.substr(1, url.size() - 1)];
  controller->skin = mesh;
  controller->skin->mark_exclude = true;
  controller->skin->transform[0] = 1;
  controller->skin->transform[0+5] = 1;
  controller->skin->transform[0+10] = 1;
  controller->skin->transform[0+15] = 1;

  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    switch (getCase(child->Value())) {
    case SOURCE:
      parseSource(child);
      break;
    case BINDSHAPEMATRIX:
      fillArray(child->FirstChild()->Value(), controller->bindShapeMatrix, 16);
      break;
    case JOINTS:
      parseJoints(child, controller);
      break;
    case VERTEXWEIGHTS:
      parseVertexWeights(child, controller);
      break;
    }
  }
}

void ParseDAE::instantiateCamera(Node *node, string url) {
  auto name = url.substr(1, url.size() - 1);
  Camera *camera = &camera_id_map[name];
  camera->name = name;
  memcpy(camera->transform, node->transform, sizeof(float) * 16);
}

void ParseDAE::instantiateLight(Node *node, string url) {
  auto name = url.substr(1, url.size() - 1);
  Light *light = &light_id_map[name];
  light->name = name;
  for (int i = 0; i < 3; i++) {
    light->direction[i] = -node->transform[i * 4 + 2];
  }
  memcpy(light->transform, node->transform, sizeof(float) * 16);
}

void ParseDAE::instantiateGeometry(Node *node, string url) {

  auto name = url.substr(1, url.size() - 1);
  Mesh *mesh = &mesh_id_map[name];
  mesh->name = name;
  memcpy(mesh->transform, node->transform, sizeof(float) * 16);
}

void ParseDAE::instantiateController(const XMLNode *cur, Node *node,
                                     string url) {
  auto name = url.substr(1, url.size() - 1);
  Controller *controller = &controller_id_map[name];
  controller->name = name;
  memcpy(controller->transform, node->parent->transform, sizeof(float) * 16);


  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    if (string(child->Value()) == "skeleton") {
      string skeletonURL = child->FirstChild()->Value();
      controller->skeletons.push_back(&node_id_map[skeletonURL.substr(1, skeletonURL.size() - 1)]);
    }
  }
}

void ParseDAE::instantiateNode(const XMLNode *cur, Node *node, string url) {
  const char *instanceType = cur->Value();

  switch (getCase(instanceType)) {
  case INSTANCEGEOMETRY:
    instantiateGeometry(node, url);
    break;
  case INSTANCELIGHT:
    instantiateLight(node, url);
    break;
  case INSTANCECAMERA:
    instantiateCamera(node, url);
    break;
  case INSTANCECONTROLLER:
    instantiateController(cur, node, url);
    break;
  }
}

void ParseDAE::parseJointNode(const XMLNode *cur, Node *parent) {
  string id = string(cur->ToElement()->FindAttribute("id")->Value());
  Node* node = &node_id_map[id];
  node->id = id;
  node->sid = string(cur->ToElement()->FindAttribute("sid")->Value());
  node->type = "JOINT";
  if (parent)
    parent->children.push_back(node);
  node->parent = parent;

  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    if (string(child->Value()) == "matrix") {
      parseTransform(child, node);
    } else if (string(child->Value()) == "node") {
      parseNode(child, node);
    } else {

      //            printf("unhandled joint node child: %s \n",
      //            child->Value());
    }
  }
}

template <typename T> void ParseDAE::parseNode(const XMLNode *cur, T parent) {}

void ParseDAE::parseNode(const XMLNode *cur, Node *parent) {
  string id = string(cur->ToElement()->FindAttribute("id")->Value());
  string type = string(cur->ToElement()->FindAttribute("type")->Value());
  if (type == "JOINT") {
    parseJointNode(cur, parent);
  } else if (type == "NODE") {
    Node *node = &node_id_map[id];
    node->id = id;
        //        node->sid =
    //        string(cur->ToElement()->FindAttribute("sid")->Value());
    node->type = "NODE";
    if (parent)
      parent->children.push_back(node);
    node->parent = parent;
    for (const XMLNode *child = cur->FirstChild(); child;
         child = child->NextSibling()) {
      if (string(child->Value()) == "node") {
        parseNode(child, node);
      } else if (string(child->Value()) == "matrix") {
        parseTransform(child, node);
      } else {
        string url = string(child->ToElement()->FindAttribute("url")->Value());
        instantiateNode(child, node, url);
        //                    printf("unhandled node child: %s \n",
        //                    child->Value());
      }
    }
  } else {
    ASSERT(0, ("Invalid type: " + type).c_str());
  }
}

template <typename T>
void ParseDAE::parseObjectRecurse(const XMLNode *cur, T *object) {
  // objects may include Mesh, Light, Camera, or controller
  if (!cur)
    return;

  switch (getCase(cur->Value())) {
  case TECHNIQUECOMMON:
    parseTechnique(cur, object);
    return;
  case SOURCE:
    parseSource(cur);
    return;
  case VERTICES:
    parseVertices(cur);
    return;
  case TRIANGLES:
    parseTriangles(cur, object);
    return;
  case SKIN:
    parseSkin(cur, object);
    return;
  case NODE:
    parseNode(cur, object);
    return;
  }
  for (const XMLNode *child = cur->FirstChild(); child;
       child = child->NextSibling()) {
    parseObjectRecurse(child, object);
  }
}

template <typename T>
void ParseDAE::setIDAndType(const XMLNode *cur, T &object) {}

void ParseDAE::setIDAndType(const XMLNode *cur, Node &object) {
  auto id = cur->ToElement()->FindAttribute("id")->Value();
  object.id = id;
  object.type = string(cur->ToElement()->FindAttribute("type")->Value());
}

template <typename T> void ParseDAE::parseObject(const XMLNode *cur, unordered_map<string, T> & idMap) {
  auto id = cur->ToElement()->FindAttribute("id")->Value();
  T object;

  setIDAndType(cur, object);

  parseObjectRecurse(cur, &idMap[id]);

}

void ParseDAE::recurseChildren(const XMLNode *cur) {
  for (const XMLNode *node = cur->FirstChild(); node;
       node = node->NextSibling()) {
    recurse(node);
  }
}

void ParseDAE::recurse(const XMLNode *cur) {
  if (cur) {
    switch (getCase(cur->Value())) {

    case GEOMETRY:
      parseObject(cur, mesh_id_map);
      break;
    case CAMERA:
      parseObject(cur, camera_id_map);
      break;
    case LIGHT:
      parseObject(cur, light_id_map);
      break;
    case CONTROLLER:
      parseObject(cur, controller_id_map);
      break;
    case NODE:
      parseObject(cur, node_id_map);
      break;

    default:
      recurseChildren(cur);
    }
  }
}

void ParseDAE::parseDAE(const string &fileName) {

//  objects<Camera> = vector<Camera>();
//  objects<Light> = vector<Light>();
//  objects<Mesh> = vector<Mesh>();
//  objects<Controller> = vector<Controller>();

//  idMap<Camera> = unordered_map<string, Camera *>();
//  idMap<Light> = unordered_map<string, Light *>();
//  idMap<Mesh> = unordered_map<string, Mesh *>();
//  idMap<Controller> = unordered_map<string, Controller *>();

  XMLDocument doc;
  doc.LoadFile(fileName.c_str());

  XMLNode *root = (XMLNode *)doc.RootElement();

  recurse(root);

  unordered_set<Source *> pointers;
  for (auto it : source_id_map) {
    pointers.insert(&it.second);
  }

  for (auto it : pointers) {
    switch (it->type) {
    case FLOAT:
      delete[] (float *)it->array;
      break;
    case NAME:
      delete[] (string *)it->array;
      break;
    }
  }
}
