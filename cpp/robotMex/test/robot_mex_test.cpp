#include "robot_mex.h"
#include <gtest/gtest.h>

TEST(RobotMexTests, Yumi) {
  int nlhs = 8;
  mxArray *plhs[10];
  int nrhs = 2;
  mxArray *prhs[10];

  prhs[0] = mxCreateDoubleScalar(INIT);
  prhs[1] = mxCreateString("/home/pac48/matlab_mex/cpp/robotMex/test/yumi");

  mexFunction(nlhs, plhs, nrhs, (const mxArray **)prhs);

  auto robotPointer = plhs[0];

  // get joint transform
  prhs[0] = mxCreateDoubleScalar(GETJOINTTRANS);
  prhs[1] = robotPointer;
  //    prhs[2] = mxCreateDoubleScalar(500);
  prhs[2] = mxCreateString("yumi_link_4_r");
  nlhs = 1;
  nrhs = 3;

  getBodyTransform(nlhs, plhs, nrhs, (const mxArray **)prhs);

  prhs[2] = mxCreateString("gripper_r_finger_r");
  nlhs = 2;
  nrhs = 3;
  getJointTree(nlhs, plhs, nrhs, (const mxArray **)prhs);

  ASSERT_EQ(1, 1);
}

TEST(RobotMexTests, Sawyer) {
  int nlhs = 8;
  mxArray *plhs[10];
  int nrhs = 2;
  mxArray *prhs[10];

  prhs[0] = mxCreateDoubleScalar(INIT);
  prhs[1] = mxCreateString("/home/pac48/matlab_mex/cpp/robotMex/test/sawyer");

  mexFunction(nlhs, plhs, nrhs, (const mxArray **)prhs);

  auto robotPointer = plhs[0];

  // get joint transform
  prhs[0] = mxCreateDoubleScalar(GETJOINTTRANS);
  prhs[1] = robotPointer;
  //    prhs[2] = mxCreateDoubleScalar(500);
  prhs[2] = mxCreateString("right_l5");
  nlhs = 1;
  nrhs = 3;

  getBodyTransform(nlhs, plhs, nrhs, (const mxArray **)prhs);

  prhs[2] = mxCreateString("right_gripper_l_finger");
  nlhs = 2;
  nrhs = 3;
  getJointTree(nlhs, plhs, nrhs, (const mxArray **)prhs);

  ASSERT_EQ(1, 1);
}

//
// int main(int argc, const char** arvg){
//  int nlhs = 8;
//  mxArray *plhs[10];
//  int nrhs = 2;
//  mxArray *prhs[10];
//
//  prhs[0] = mxCreateDoubleScalar(INIT);
////  prhs[1] =
///mxCreateString("/home/pac48/matlab_mex/cpp/robotMex/test/sawyer");
//  prhs[1] = mxCreateString("/home/pac48/matlab_mex/cpp/robotMex/test/yumi");
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}