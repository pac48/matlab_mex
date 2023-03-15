//
// Created by paulg on 1/16/2022.
//
#include <iostream>
#include "tiny_cuda_mex.h"
#include "gpu/mxGPUArray.h"
#include "tiny-cuda-nn/cpp_api.h"

using namespace tcnn::cpp;
using namespace nlohmann;





//double * getModelPtr(const mxArray* pa){
//  double* pointer0 = mxGetPr(pa);
//  mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
//  return pointer0;
//}

Module *getModelPtr(const mxArray *pa) {
  double *pointer0 = mxGetPr(pa);
  mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
  intptr_t pointer1 = (intptr_t) pointer0[0];
  return (Module *) pointer1;
}

void destroy(int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[]) {
  Module *network_ptr = getModelPtr(prhs[1]);
  printf("deleted tiny cuda network\n");

  delete network_ptr;
}

void init(int nlhs, mxArray *plhs[],
          int nrhs, const mxArray *prhs[]) {


  auto val = mxInitGPU();
  printf("%s: %d\n", "the gpu api status is", val);

  char *input_string = mxArrayToString(prhs[1]);
  uint32_t n_input_dims = mxGetScalar(prhs[2]);
  uint32_t n_output_dims = mxGetScalar(prhs[3]);

  auto config_string = std::string(input_string);
  mxFree(input_string);
  nlohmann::json config = json::parse(config_string);

//    std::stringstream ss;
//    ss << config_string;
//    printf("%s", ss.str().c_str());

  auto encoding_config = config.value("encoding", json::object());
  auto network_config = config.value("network", json::object());
  auto network = create_network_with_input_encoding(n_input_dims, n_output_dims, encoding_config, network_config);

  plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
  double *network_ptr = mxGetPr(plhs[0]);
  network_ptr[0] = (intptr_t) network;

  float *m_params_full_precision = nullptr;
  float *params = nullptr;
  auto n_params = network->n_params();
  cudaMalloc(&params, n_params * sizeof(float));
  cudaMalloc(&m_params_full_precision, n_params * sizeof(float));
  network->initialize_params(0, m_params_full_precision);
  network->initialize_params(0, params);

}


void num_params(int nlhs, mxArray *plhs[],
                int nrhs, const mxArray *prhs[]) {
  Module *network_ptr = getModelPtr(prhs[1]);
  plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
  auto double_ptr = mxGetPr(plhs[0]);
  double_ptr[0] = network_ptr->n_params();
  auto nmu = network_ptr->n_params();
  int o = 0;
}

void forward(int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[]) {
  cudaStream_t stream;
  cudaStreamCreate(&stream);

  const int netPrtInd = 1;
  const int paramInd = 2;
  const int inputInd = 3;

  mxAssert(mxIsGPUArray(prhs[paramInd]) != 0, "params must be a gpu array");
  mxAssert(mxIsGPUArray(prhs[inputInd]) != 0, "inputs must be a gpu array");


  auto paramGpuArray = mxGPUCreateFromMxArray(prhs[paramInd]);
  const mwSize *paramDims = mxGPUGetDimensions(paramGpuArray);
  int paramNumDims = mxGPUGetNumberOfDimensions(paramGpuArray);
  mxAssert(paramNumDims == 2 && paramDims[1] == 1, "params is a 1-d vector of network weights");

  auto inputGpuArray = mxGPUCreateFromMxArray(prhs[inputInd]);
  const mwSize *inputDims = mxGPUGetDimensions(inputGpuArray);
  int inputNumDims = mxGPUGetNumberOfDimensions(inputGpuArray);
  mxAssert(inputNumDims == 2, "Input must of shape (input_dim x batch_size)");

  auto input_gpu_ptr = mxGPUCopyFromMxArray(prhs[inputInd]);
  void *gpu_ptr = mxGPUGetData(input_gpu_ptr);
  float *input_d = static_cast<float *>(gpu_ptr);

  mxGPUArray *params_gpu_ptr = mxGPUCopyFromMxArray(prhs[paramInd]);
  gpu_ptr = mxGPUGetData(params_gpu_ptr);
  float *params = static_cast<float *>(gpu_ptr);


  mwSize batch_size = inputDims[1];
  const mwSize num_dims = 2;
  mwSize dims[num_dims];
  dims[0] = 1;
  dims[1] = batch_size;
  mxGPUArray *gpu_array_ptr = mxGPUCreateGPUArray(num_dims, dims, mxSINGLE_CLASS, mxREAL, MX_GPU_DO_NOT_INITIALIZE);
  float *output_d = static_cast<float *>(mxGPUGetData(gpu_array_ptr));

  Module *network_ptr = getModelPtr(prhs[netPrtInd]);

  //  float * params2;
//  float * input_d2;
//  float * output_d2;
//  cudaMalloc(&input_d2, 2 * batch_size * sizeof(float));
//  cudaMalloc(&output_d2, 1 * batch_size * sizeof(float));
//  auto n_params = network_ptr->n_params();
//  cudaMalloc(&params2, n_params * sizeof(float));
//  network_ptr->initialize_params(0, params2);

  tcnn::cpp::Context ctx = network_ptr->forward(stream, batch_size, input_d, output_d, params, false);

  plhs[0] = mxGPUCreateMxArrayOnGPU(gpu_array_ptr);


  mxGPUDestroyGPUArray(paramGpuArray);
  mxGPUDestroyGPUArray(inputGpuArray);

  mxGPUDestroyGPUArray(input_gpu_ptr);
  mxGPUDestroyGPUArray(params_gpu_ptr);
  mxGPUDestroyGPUArray(gpu_array_ptr);

  cudaStreamDestroy(stream);
}

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]) {



  int val = mxGetScalar(prhs[0]);

  switch (val) {
    case INIT:
      init(nlhs, plhs, nrhs, prhs);
      break;
    case DESTROY:
      destroy(nlhs, plhs, nrhs, prhs);
      break;
    case FORWARD: {
      forward(nlhs, plhs, nrhs, prhs);
      break;

    }
    case BACKWARD:
      init(nlhs, plhs, nrhs, prhs);
      break;
    case NUM_PARAMS:
      num_params(nlhs, plhs, nrhs, prhs);
      break;
    default:
      break;
  }

}

// Configure the model
//nlohmann::json config = {
//        {"loss", {
//                         {"otype", "L2"}
//                 }},
//        {"optimizer", {
//                         {"otype", "Adam"},
//                         {"learning_rate", 1e-3},
//                 }},
//        {"encoding", {
//                         {"otype", "HashGrid"},
//                         {"n_levels", 16},
//                         {"n_features_per_level", 2},
//                         {"log2_hashmap_size", 19},
//                         {"base_resolution", 16},
//                         {"per_level_scale", 2.0},
//                 }},
//        {"network", {
//                         {"otype", "FullyFusedMLP"},
//                         {"activation", "ReLU"},
//                         {"output_activation", "None"},
//                         {"n_neurons", 64},
//                         {"n_hidden_layers", 2},
//                 }},
//};


std::string config_string = "{\"encoding\":{\"base_resolution\":16,\"log2_hashmap_size\":19,\"n_features_per_level\":2,\"n_levels\":16,\"otype\":\"HashGrid\",\"per_level_scale\":2.0},\"loss\":{\"otype\":\"L2\"},\"network\":{\"activation\":\"ReLU\",\"n_hidden_layers\":2,\"n_neurons\":64,\"otype\":\"FullyFusedMLP\",\"output_activation\":\"None\"},\"optimizer\":{\"learning_rate\":0.001,\"otype\":\"Adam\"}}";

int main() {

  nlohmann::json config = json::parse(config_string);

//    std::stringstream ss;
//    ss << config;
//    auto var=  ss.str();
//    json network;
//    json encoding;
  cudaStream_t stream;
  cudaStreamCreate(&stream);

  uint32_t n_output_dims = 1;
  uint32_t n_input_dims = 2;
  const uint32_t batch_size = 128;

//    Module* module =  create_network_with_input_encoding(n_input_dims, n_output_dims, encoding, network);
//    auto [loss, optimizer, network, trainer] =  tcnn::create_from_config(n_input_dims, n_output_dims, config);
  auto encoding_config = config.value("encoding", json::object());
  auto network_config = config.value("network", json::object());
  auto network = create_network_with_input_encoding(n_input_dims, n_output_dims, encoding_config, network_config);

  float *m_params_full_precision = nullptr;
  bool device_ptr = false;
  auto n_params = network->n_params();
//    GPUMemory<float> params(n_params);
  float *params;
  float *input_d;
  float *output_d;

  cudaMalloc(&params, n_params * sizeof(float));
  cudaMalloc(&m_params_full_precision, n_params * sizeof(float));
  cudaMalloc(&input_d, 2 * batch_size * sizeof(float));
  cudaMalloc(&output_d, 1 * batch_size * sizeof(float));

  float tmp = 1.12;
  cudaMemcpy(params, &tmp, sizeof(float), cudaMemcpyHostToDevice);

//    cudaMemcpy(m_params_full_precision, params, sizeof(float)*n_params, cudaMemcpyDeviceToDevice);

  network->initialize_params(0, m_params_full_precision);
  network->initialize_params(0, params);


//    float input[2*batch_size] =  {0};
  float input[2 * batch_size] = {0.0};
  cudaMemcpy(input_d, input, sizeof(float) * 2 * batch_size, cudaMemcpyHostToDevice);

  float output[1 * batch_size] = {0};
  cudaMemcpy(output_d, output, sizeof(float) * 1 * batch_size, cudaMemcpyHostToDevice);


  tcnn::cpp::Context ctx = network->forward(stream, batch_size, input_d, output_d, params, false);
  cudaMemcpy(output, output_d, sizeof(float) * 1 * batch_size, cudaMemcpyDeviceToHost);

  float tmp2;
  cudaMemcpy(&tmp2, m_params_full_precision, sizeof(float), cudaMemcpyDeviceToHost);

  int o = 0;


  cudaStreamDestroy(stream);
  return 0;

}