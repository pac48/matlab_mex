//
// Created by paulg on 1/16/2022.
//

#include "robot_mex.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "vector"
//#include "queue"
#include "rl/math/Transform.h"
#include "rl/math/Unit.h"
#include "rl/mdl/Dynamic.h"
#include "rl/mdl/Body.h"
#include <cstdio>
#include <string>
#include "rl/mdl/UrdfFactory.h"
#include "rl/mdl/Joint.h"

#include "tinyxml2.h"
#include "iostream"
#include "string"

using namespace rl;
using namespace std;
using namespace tinyxml2;


struct Node{
  string bodyName;
  string jointName;
  Node* parent;
  mdl::Body* body;
  int jointType;

  Node(string bodyNameIn="", string jointNameIn="", Node* parentIn=nullptr, mdl::Body* bodyIn=nullptr, int jointTypeIn=FIXED){
    bodyName = bodyNameIn;
    jointName = jointNameIn;
    parent = parentIn;
    body = bodyIn;
    jointType = jointTypeIn;
  }
};

class MatlabRobot{
public:
  const mdl::Dynamic* robotPtr;
  unordered_map<string, Node*> body2NodeMap;
  unordered_map<string, string> joint2BodyMap;
  unordered_map<string, int> joint2IndMap;

  MatlabRobot(const mdl::Dynamic* robotPtrIn, string urdf_file_name){
    this->robotPtr = robotPtrIn;
    parseURDF(urdf_file_name);


  }
  ~MatlabRobot(){
    for (auto keyPair : this->body2NodeMap){
      delete keyPair.second;
    }
  }

  void parseURDF(const string& fileName) {
    XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    XMLNode *root = (XMLNode *) doc.RootElement();

    unordered_map <string, string> parentLinkMap;
    //        unordered_set<string> linkNames;

    for (const XMLNode *xmlNode = root->FirstChild(); xmlNode; xmlNode = xmlNode->NextSibling()) {
      if( auto comment = dynamic_cast<const XMLComment*>(xmlNode) ) continue;
      const XMLElement* child = xmlNode->ToElement();

      if (strcmp(child->Value(), "joint") == 0) {
        string jointName = child->Attribute("name");
        string parentLinkName = child->FirstChildElement("parent")->Attribute("link");
        string childLinkName = child->FirstChildElement("child")->Attribute("link");
        //                linkNames.insert(parentLinkName);
        //                linkNames.insert(childLinkName);
        joint2BodyMap[jointName] = childLinkName;
        parentLinkMap[childLinkName] = parentLinkName;

        int jointType = FIXED;
        if (strcmp(child->Attribute("type"), "revolute") == 0){
          jointType = REVOLUTE;
        } else if (strcmp(child->Attribute("type"), "prismatic") == 0){
          jointType = PRISMATIC;
        }

        if (body2NodeMap.find(childLinkName) == body2NodeMap.end()){
          body2NodeMap[childLinkName] = new Node(childLinkName);
        }
        if (body2NodeMap.find(parentLinkName) == body2NodeMap.end()){
          body2NodeMap[parentLinkName] = new Node(parentLinkName);
        }

        body2NodeMap[childLinkName]->jointType = jointType;
        body2NodeMap[childLinkName]->jointName = jointName;
      }
    }

    for (auto keyPair : body2NodeMap){
      //            cout << "linkNames: " << linkName << endl;
      string linkName = keyPair.first;

      Node* parentNode = nullptr;

      if (parentLinkMap.find(linkName) != parentLinkMap.end()){
        string parentName = parentLinkMap[linkName];
        //                if (body2NodeMap.find(parentName) == body2NodeMap.end()){
        //                    body2NodeMap[parentName] = new Node(parentName);
        //                }
        parentNode = body2NodeMap[parentName];
      }

      //            if (body2NodeMap.find(linkName) == body2NodeMap.end()){
      //                body2NodeMap[linkName] = new Node(linkName);
      //            }

      body2NodeMap[linkName]->parent = parentNode;

    }

    for (int i = 0; i < robotPtr->getBodies(); i++){
      body2NodeMap[robotPtr->getBody(i)->getName()]->body = robotPtr->getBody(i);
    }

    for (int i = 0; i < robotPtr->getJoints(); i++) {
      joint2IndMap[robotPtr->getJoint(i)->getName()] = i;
    }
  }

};


mdl::Dynamic* getModelPtr(const mxArray* pa){
  double* pointer0 = mxGetPr(pa);
  mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
  intptr_t pointer1 = (intptr_t) pointer0[0];
  auto robot = (MatlabRobot*) pointer1;
  return (mdl::Dynamic*) robot->robotPtr;
}

MatlabRobot* getMatlabRobotPtr(const mxArray* pa){
  double* pointer0 = mxGetPr(pa);
  mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
  intptr_t pointer1 = (intptr_t) pointer0[0];
  auto robot = (MatlabRobot*) pointer1;
  return (MatlabRobot*) robot;
}

void setMatrixOutput(mxArray*& pa, const math::Matrix& M){
  pa = mxCreateDoubleMatrix(M.rows(), M.cols(), mxREAL);
  double* data_ptr = mxGetPr(pa);
  memcpy(data_ptr, M.data(), sizeof(double)*M.cols()*M.rows());
}

void setVectorOutput(mxArray*& pa, const vector<double> vec, bool reverse=false){
  pa = mxCreateDoubleMatrix(vec.size(), 1, mxREAL);
  double* data_ptr = mxGetPr(pa);
  vector<double> vecR;
  const double* vecData; //vec.data()

  if (reverse){
    std::copy(vec.rbegin(), vec.rend(), std::inserter(vecR, vecR.begin()));
    vecData = vecR.data();
  } else{
    vecData = vec.data();
  }

  memcpy(data_ptr, vecData, sizeof(double)*vec.size());
}

void setMatrixOutput(mxArray*& pa, const math::Transform& T){
  pa = mxCreateDoubleMatrix(4, 4, mxREAL);
  double* data_ptr = mxGetPr(pa);
  memcpy(data_ptr, T.data(), sizeof(double)*16);
}

void myAssert(int expression, const char *error_message){
  if (!expression){
    //        printf(error_message);
    fprintf(stderr, error_message);
    mexErrMsgTxt(error_message);
  }
}
void myAssert(int expression, const string &error_message){
  myAssert(expression, error_message.c_str());
}




void init(int nlhs, mxArray *plhs[],
          int nrhs, const mxArray *prhs[] ){

  myAssert(nrhs == 2, "Init function requires the name of the urdf file");
  myAssert(nlhs == 8, "expects 8 output argument to be assigned");

  char * filename =  mxArrayToString(prhs[1]);
  string name(filename);
  string urdf_file_name = name;
  auto* model = new mdl::Model();
  rl::mdl::UrdfFactory urdf;
  urdf.load(urdf_file_name, model);
  mdl::Dynamic* dynamics = (mdl::Dynamic*) model;
  dynamics->forwardPosition();
  plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
  double* robot_ptr = mxGetPr(plhs[0]);
  auto robot = new MatlabRobot(dynamics, urdf_file_name);
  robot_ptr[0] = (intptr_t) robot;

  plhs[1] = mxCreateDoubleMatrix(1,1,mxREAL);
  double* numJoint = mxGetPr(plhs[1]);
  numJoint[0] = model->getJoints();

  plhs[2] = mxCreateDoubleMatrix(1,1,mxREAL);
  double* numBodies = mxGetPr(plhs[2]);
  numBodies[0] = dynamics->getBodies();

  mwSize numDims = 1;
  mwSize dims[1] = {(mwSize) model->getJoints()};


  plhs[3] = mxCreateCellArray(numDims, dims);
  for (int i = 0; i < model->getJoints(); i++){
    mxSetCell(plhs[3], i, mxCreateString(model->getJoint(i)->getName().c_str()));
  }

  dims[0] = dynamics->getBodies();
  plhs[4] = mxCreateCellArray(numDims, dims);
  for (int i = 0; i < model->getBodies(); i++){
    mxSetCell(plhs[4], i, mxCreateString(dynamics->getBody(i)->getName().c_str()));
  }

  plhs[5] = mxCreateDoubleMatrix(1,1,mxREAL);
  double* numEE = mxGetPr(plhs[5]);
  numEE[0] = dynamics->getOperationalDof();


  plhs[6] = mxCreateDoubleMatrix(model->getJoints(), 1, mxREAL);
  double* minJoints = mxGetPr(plhs[6]);
  for (int i = 0; i < model->getJoints(); i++){
    minJoints[i] = model->getJoint(i)->getMinimum()[0];
  }

  plhs[7] = mxCreateDoubleMatrix(model->getJoints(), 1, mxREAL);
  double* maxJoints = mxGetPr(plhs[7]);
  for (int i = 0; i < model->getJoints(); i++){
    maxJoints[i] = model->getJoint(i)->getMaximum()[0];
  }

}

void setJoints(int nlhs, mxArray *plhs[],
               int nrhs, const mxArray *prhs[]){

  myAssert(nrhs == 3, "three input argument expected");
  myAssert(nlhs == 0, "no output arguments expected");
  mdl::Dynamic* model = getModelPtr(prhs[1]);

  double* jointAngle = (double*) mxGetData(prhs[2]);
  const mwSize* dims = mxGetDimensions(prhs[2]);
  mwSize numDims = mxGetNumberOfDimensions(prhs[2]);

  auto curJoint = model->getPosition();
  //    printf("number of dims is %d and value is %d", numDims, dims[0]);
  myAssert(numDims == 2 && dims[0] == curJoint.size(), "number of joints must be equal to length of input array");

  for (int i =0; i < curJoint.size(); i++){
    curJoint[i] = jointAngle[i];
  }
  model->setPosition(curJoint);
  model->forwardPosition();
}

void getJoints(int nlhs, mxArray *plhs[],
               int nrhs, const mxArray *prhs[]){

  myAssert(nrhs == 2, "two input argument expected");
  myAssert(nlhs == 1, "one output arguments expected");
  mdl::Dynamic* model = getModelPtr(prhs[1]);

  auto curJoint = model->getPosition();
  plhs[0] = mxCreateDoubleMatrix(curJoint.size(), 1, mxREAL);
  double* joints_ptr = mxGetPr(plhs[0]);

  for (int i =0; i < curJoint.size(); i++){
    joints_ptr[i] = curJoint[i];
  }

}

void getOperationalPosition(int nlhs, mxArray *plhs[],
                            int nrhs, const mxArray *prhs[]){

  myAssert(nrhs == 3, "two input argument expected");
  myAssert(nlhs == 1, "one output arguments expected");
  mdl::Dynamic* model = getModelPtr(prhs[1]);

  int index = mxGetScalar(prhs[2])-1;
  myAssert(index < model->getOperationalDof() && index >= 0, "index out of bounds");
  rl::math::Transform T = model->getOperationalPosition(index);

  setMatrixOutput(plhs[0], T);

}

void getJacobian(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]){
  myAssert(nrhs == 2, "two input argument expected");
  myAssert(nlhs == 1, "one output arguments expected");
  mdl::Dynamic* model = getModelPtr(prhs[1]);
  int numEE = model->getOperationalDof();
  int numDof = model->getDof();
  math::Matrix J = math::Matrix(6*numEE, numDof);
  model->calculateJacobian(J);

  setMatrixOutput(plhs[0], J);
}

mdl::Body* getBody(MatlabRobot* robot, const mxArray * query){
  mdl::Dynamic* model = (mdl::Dynamic*) robot->robotPtr;
  mdl::Body* body;
  if (mxIsDouble(query)){
    int index = mxGetScalar(query)-1;
    myAssert(index < model->getBodies() && index >= 0, "index out of bounds");
    body = model->getBody(index);
  } else{
    string bodyName = string(mxArrayToString(query));
    myAssert(robot->body2NodeMap.find(bodyName)!=robot->body2NodeMap.end(), "the body name: " + bodyName + " is not in the robot model");
    Node* node = robot->body2NodeMap[bodyName];
    body = node->body;
  }
  return body;
}

void getBodyTransform(int nlhs, mxArray *plhs[],
                      int nrhs, const mxArray *prhs[]){
  myAssert(nrhs == 3, "two input argument expected");
  myAssert(nlhs == 1, "one output arguments expected");

  MatlabRobot* robot = getMatlabRobotPtr(prhs[1]);
  auto body = getBody(robot, prhs[2]);
  //    MatlabRobot* robot = getMatlabRobotPtr(prhs[1]);
  //    mdl::Dynamic* model = (mdl::Dynamic*) robot->robotPtr;
  //    mdl::Body* body;
  //    if (mxIsDouble(prhs[2])){
  //        int index = mxGetScalar(prhs[2])-1;
  //        myAssert(index < model->getBodies() && index >= 0, "index out of bounds");
  //        body = model->getBody(index);
  //    } else{
  //        string bodyName = string(mxArrayToString(prhs[2]));
  //
  //        myAssert(robot->body2NodeMap.find(bodyName)!=robot->body2NodeMap.end(), "the body name: " + bodyName + " is not in the robot model");
  //
  //        Node* node = robot->body2NodeMap[bodyName];
  //        body = node->body;
  //    }
  setMatrixOutput(plhs[0], body->t);
}

void getJointTransform(int nlhs, mxArray *plhs[],
                       int nrhs, const mxArray *prhs[]){
  myAssert(nrhs == 3, "two input argument expected");
  myAssert(nlhs == 1, "one output arguments expected");

  MatlabRobot* robot = getMatlabRobotPtr(prhs[1]);
  mdl::Dynamic* model = (mdl::Dynamic*) robot->robotPtr;
  string jointName;

  if (mxIsDouble(prhs[2])){
    int jointIndex = mxGetScalar(prhs[2])-1;
    myAssert(jointIndex < model->getJoints() && jointIndex >= 0, "index out of bounds");
    jointName = model->getJoint(jointIndex)->getName();
  } else{
    jointName = string(mxArrayToString(prhs[2]));
    myAssert(robot->joint2BodyMap.find(jointName)!=robot->joint2BodyMap.end(), "the joint name: " + jointName + " is not in the robot model");
  }
  string bodyName = robot->joint2BodyMap[jointName];
  mdl::Body* body = robot->body2NodeMap[bodyName]->body;
  setMatrixOutput(plhs[0], body->t);
}

void getJointTree(int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[]){
  myAssert(nrhs == 3, "two input argument expected");
  myAssert(nlhs == 2, "two output arguments expected");
  MatlabRobot* robot = getMatlabRobotPtr(prhs[1]);

  auto body = getBody(robot, prhs[2]);

  Node* node = robot->body2NodeMap[body->getName()];
  vector<double> jointInds;
  vector<double> jointTypes;

  while (node){
    if (node->jointType != FIXED){
      jointTypes.push_back(node->jointType);
      jointInds.push_back(robot->joint2IndMap[node->jointName]+1);
    }
    node = node->parent;
  }

  setVectorOutput(plhs[0], jointInds, true);
  setVectorOutput(plhs[1], jointTypes, true);

}

void destroy(int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[] ){
  myAssert(nrhs == 2, "Only two input argument expected");
  myAssert(nlhs == 0, "no output arguments expected");
  mdl::Dynamic* model = getModelPtr(prhs[1]);
  MatlabRobot* robot = getMatlabRobotPtr(prhs[1]);
  printf("deleted robot object\n");

  delete model;
  delete robot;
}



void mexFunction( int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[] ) {

  myAssert(true, "test test test!!!!");


  myAssert(nrhs >= 1, "input must specify a function using an int");

  int val = mxGetScalar(prhs[0]);
  switch (val) {
  case INIT: init(nlhs, plhs, nrhs, prhs); break;
  case DESTROY: destroy(nlhs, plhs, nrhs, prhs); break;
  case SETJOINTS: setJoints(nlhs, plhs, nrhs, prhs); break;
  case GETJOINTS: getJoints(nlhs, plhs, nrhs, prhs); break;
  case GETJACOB: getJacobian(nlhs, plhs, nrhs, prhs); break;
  case GETBODYTRANS: getBodyTransform(nlhs, plhs, nrhs, prhs); break;
  case GETJOINTTRANS: getJointTransform(nlhs, plhs, nrhs, prhs); break;
  case GETOPPOS: getOperationalPosition(nlhs, plhs, nrhs, prhs); break;
  case GETJOINTTREE: getJointTree(nlhs, plhs, nrhs, prhs); break;
  default: break;
  }

}


//int main(int argc, const char** arvg){
//  int nlhs = 8;
//  mxArray *plhs[10];
//  int nrhs = 2;
//  mxArray *prhs[10];
//
//  prhs[0] = mxCreateDoubleScalar(INIT);
//  prhs[1] = mxCreateString("/home/pac48/matlab_mex/cpp/robotMex/test/sawyer");
//
//  mexFunction(nlhs, plhs, nrhs, (const mxArray **) prhs);
//
//
//  auto robotPointer = plhs[0];
//
//  // get joint transform
//  prhs[0] = mxCreateDoubleScalar(GETJOINTTRANS);
//  prhs[1] = robotPointer;
//  //    prhs[2] = mxCreateDoubleScalar(500);
//  prhs[2] = mxCreateString("right_l5");
//  nlhs = 1;
//  nrhs = 3;
//
//  getBodyTransform(nlhs, plhs, nrhs, (const mxArray **) prhs);
//
//  prhs[2] = mxCreateString("right_gripper_l_finger");
//  nlhs = 2;
//  nrhs = 3;
//  getJointTree(nlhs, plhs, nrhs, (const mxArray **) prhs);
//
//  return 0;
//}