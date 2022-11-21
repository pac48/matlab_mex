//
// Created by paulg on 2/9/2022.
//
#include "struct_types.h"

#include "algorithm"
#include "globals.h"
#include "sstream"
#include "string"
#include "tinyxml2.h"
#include "unordered_map"
#include "unordered_set"

using namespace tinyxml2;
using namespace std;

enum Cases {
  CONTROLLER,
  GEOMETRY,
  SOURCE,
  MESH,
  FLOATARRAY,
  VERTICES,
  TRIANGLES,
  NAMEARRAY,
  TECHNIQUECOMMON,
  INPUT,
  PRIMITIVE,
  NODE,
  CAMERA,
  LIGHT,
  INSTANCELIGHT,
  INSTANCECAMERA,
  INSTANCEGEOMETRY,
  INSTANCECONTROLLER,
  EXTRA,
  SKIN,
  BINDSHAPEMATRIX,
  JOINTS,
  VERTEXWEIGHTS,
  VCOUNT,
  INFLUENCES,
  NONE
};

class ParseDAE {
public:
  Cases getCase(const char *type);

  template <typename T> void fillArray(const char *data, T *array, int count);

  void parseTransform(const XMLNode *cur, Node *node);

  template <typename T> void parseArray(const XMLNode *cur, Source *source);

  void parseTechnique(const XMLNode *cur, Source *source);

  template <typename T>
  void parseTechnique(const XMLNode *cur, T *object); // default implementation

  void parseTechnique(const XMLNode *cur, Light *light);

  void parseTechnique(const XMLNode *cur, Camera *camera);

  void parseSource(const XMLNode *cur);

  void parseVertices(const XMLNode *cur);

  template <typename T> void parseInput(const XMLNode *cur, T &inputs);

  void parseInput2(const XMLNode *cur, unordered_map<int, Input> &inputs,
                   Input &input);

  void parseInput2(const XMLNode *cur, vector<Input> &inputs, Input &input);

  void parseJoints(const XMLNode *cur, Controller *controller);

  void parsePrimitives(const XMLNode *cur, unordered_map<int, Input> meshInputs,
                       Mesh *mesh);

  template <typename T> void parseTriangles(const XMLNode *cur, T *mesh);

  void parseTriangles(const XMLNode *cur, Mesh *mesh);

  void parseInfluences(const char *data, int *vcount, int numVerts,
                       Controller *controller,
                       unordered_map<int, Input> &controllerInputs);

  void parseVertexWeights(const XMLNode *cur, Controller *controller);

  template <typename T> void parseSkin(const XMLNode *cur, T controller);

  void parseSkin(const XMLNode *cur, Controller *controller);

  void instantiateCamera(Node *node, string url);

  void instantiateLight(Node *node, string url);

  void instantiateGeometry(Node *node, string url);

  void instantiateController(const XMLNode *cur, Node *node, string url);

  void instantiateNode(const XMLNode *cur, Node *node, string url);

  void parseJointNode(const XMLNode *cur, Node *parent);

  template <typename T> void parseNode(const XMLNode *cur, T parent);

  void parseNode(const XMLNode *cur, Node *parent);

  template <typename T> void parseObjectRecurse(const XMLNode *cur, T *object);

  template <typename T> void setIDAndType(const XMLNode *cur, T &object);

  void setIDAndType(const XMLNode *cur, Node &object);

  template <typename T>
  void parseObject(const XMLNode *cur, unordered_map<string, T> &idMap);

  void recurseChildren(const XMLNode *cur);

  void recurse(const XMLNode *cur);

  void parseDAE(const string &fileName);

  //  template<typename T>
  //  vector<Camera> camera_objects;
  //  vector<Controller> controller_objects;
  //  vector<Light> light_objects;
  //  vector<Mesh> mesh_objects;
  //  vector<Node> node_objects;

  unordered_map<string, Camera> camera_id_map;
  unordered_map<string, Controller> controller_id_map;
  unordered_map<string, Light> light_id_map;
  unordered_map<string, Mesh> mesh_id_map;
  unordered_map<string, Node> node_id_map;
  unordered_map<string, Source> source_id_map;

  //
  //  template<typename T>
  //  unordered_map<string, T*> idMap;

  unordered_map<string, Cases> caseMap = {
      {"controller", CONTROLLER},
      {"geometry", GEOMETRY},
      {"float_array", FLOATARRAY},
      {"mesh", MESH},
      {"source", SOURCE},
      {"vertices", VERTICES},
      {"triangles", TRIANGLES},
      {"Name_array", NAMEARRAY},
      {"technique_common", TECHNIQUECOMMON},
      {"p", PRIMITIVE},
      {"input", INPUT},
      {"node", NODE},
      {"camera", CAMERA},
      {"light", LIGHT},
      {"instance_light", INSTANCELIGHT},
      {"instance_camera", INSTANCECAMERA},
      {"instance_geometry", INSTANCEGEOMETRY},
      {"instance_controller", INSTANCECONTROLLER},
      {"skin", SKIN},
      {"extra", EXTRA},
      {"joints", JOINTS},
      {"bind_shape_matrix", BINDSHAPEMATRIX},
      {"vertex_weights", VERTEXWEIGHTS},
      {"vcount", VCOUNT},
      {"v", INFLUENCES}};
};