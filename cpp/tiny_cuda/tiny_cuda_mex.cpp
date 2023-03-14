//
// Created by paulg on 1/16/2022.
//
#include "tiny_cuda_mex.h"
#include "tiny-cuda-nn/cpp_api.h"
//#include "tiny-cuda-nn/gpu_memory.h"
//#include "tiny-cuda-nn/network_with_input_encoding.h"
//#include "tiny-cuda-nn/config.h"



using namespace tcnn::cpp;
using namespace tcnn;
using namespace nlohmann;


double * getModelPtr(const mxArray* pa){
  double* pointer0 = mxGetPr(pa);
  mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
  return pointer0;
}

void init(int nlhs, mxArray *plhs[],
          int nrhs, const mxArray *prhs[] ){
    json network;
    json encoding;
    uint32_t n_output_dims;
    uint32_t n_input_dims;

    Module* module =  create_network_with_input_encoding(n_input_dims, n_output_dims, encoding, network);

}

void mexFunction( int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[] ) {

  int val = mxGetScalar(prhs[0]);
  switch (val) {
  case INIT: init(nlhs, plhs, nrhs, prhs); break;
  default: break;
  }

}

// Configure the model
nlohmann::json config = {
        {"loss", {
                         {"otype", "L2"}
                 }},
        {"optimizer", {
                         {"otype", "Adam"},
                         {"learning_rate", 1e-3},
                 }},
        {"encoding", {
                         {"otype", "HashGrid"},
                         {"n_levels", 16},
                         {"n_features_per_level", 2},
                         {"log2_hashmap_size", 19},
                         {"base_resolution", 16},
                         {"per_level_scale", 2.0},
                 }},
        {"network", {
                         {"otype", "FullyFusedMLP"},
                         {"activation", "ReLU"},
                         {"output_activation", "None"},
                         {"n_neurons", 64},
                         {"n_hidden_layers", 2},
                 }},
};

int main(){
//    json network;
//    json encoding;


    cudaStream_t stream;
    cudaStreamCreate(&stream);

    uint32_t n_output_dims = 1;
    uint32_t n_input_dims = 2;

//    Module* module =  create_network_with_input_encoding(n_input_dims, n_output_dims, encoding, network);
//    auto [loss, optimizer, network, trainer] =  tcnn::create_from_config(n_input_dims, n_output_dims, config);
    auto network = create_network_with_input_encoding(n_input_dims, n_output_dims, config.value("encoding", json::object()), config.value("network", json::object()));

    float* m_params_full_precision = nullptr;
    bool device_ptr = false;
    auto n_params = network->n_params();
//    GPUMemory<float> params(n_params);
    float* params;
    float* input_d;
    float* output_d;

    cudaMalloc(&params, n_params * sizeof(float));
    cudaMalloc(&m_params_full_precision, n_params * sizeof(float));
    cudaMalloc(&input_d, 2*128* sizeof(float));
    cudaMalloc(&output_d, 1*128* sizeof(float));


    float tmp = 1.12;
    cudaMemcpy(params, &tmp, sizeof(float), cudaMemcpyHostToDevice);

//    cudaMemcpy(m_params_full_precision, params, sizeof(float)*n_params, cudaMemcpyDeviceToDevice);

    network->initialize_params(0, m_params_full_precision);
    network->initialize_params(0, params);


//    float input[2*128] =  {0};
    float input[2*128] =  {0};
    cudaMemcpy(input_d, input, sizeof(float)*2*128, cudaMemcpyHostToDevice);

    float output[1*128] = {0};
    cudaMemcpy(output_d, output, sizeof(float)*1*128, cudaMemcpyHostToDevice);


    tcnn::cpp::Context ctx = network->forward(stream, 128, input_d, output_d, params, true);
    cudaMemcpy(output, output_d, sizeof(float)*1*128, cudaMemcpyDeviceToHost);

    float tmp2;
    cudaMemcpy(&tmp2, m_params_full_precision, sizeof(float), cudaMemcpyDeviceToHost);

    int o = 0;


    cudaStreamDestroy(stream);
    return 0;

}