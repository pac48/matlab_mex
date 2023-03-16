#include <iostream>
#include "tiny_cuda_mex.h"
#include "tiny-cuda-nn/cpp_api.h"

using namespace tcnn::cpp;
using namespace nlohmann;

struct Config {
    Config(Context *ctxIn, uint32_t n_input_dimsIn, uint32_t n_output_dimsIn) : ctx{ctxIn},
                                                                                n_input_dims{n_input_dimsIn},
                                                                                n_output_dims{n_output_dimsIn} {}

    Context *ctx;
    uint32_t n_input_dims;
    uint32_t n_output_dims;
};

typedef std::tuple<Module *, cudaStream_t *, Config *> netStreamPair;


netStreamPair *getPairPtr(const mxArray *pa) {
    double *pointer0 = mxGetPr(pa);
    mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
    intptr_t pointer1 = (intptr_t) pointer0[0];
    netStreamPair *p = (netStreamPair *) pointer1;
    return p;
}

netStreamPair getPair(const mxArray *pa) {
    return *getPairPtr(pa);
}


void destroy(int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[]) {

    auto [network_ptr, stream_ptr, tmp] = getPair(prhs[1]);
    printf("deleted tiny cuda network\n");
    delete network_ptr;
    cudaStreamDestroy(*stream_ptr);
    delete stream_ptr;
    delete getPairPtr(prhs[1]);

}

void init(int nlhs, mxArray *plhs[],
          int nrhs, const mxArray *prhs[]) {

    auto stream_ptr = new cudaStream_t();
    cudaStreamCreate(stream_ptr);

    auto val = mxInitGPU();
    printf("%s: %d\n", "the gpu api status is", val);

    char *input_string = mxArrayToString(prhs[1]);
    uint32_t n_input_dims = mxGetScalar(prhs[2]);
    uint32_t n_output_dims = mxGetScalar(prhs[3]);

    auto config_string = std::string(input_string);
    mxFree(input_string);
    nlohmann::json config = json::parse(config_string);

    auto encoding_config = config.value("encoding", json::object());
    auto network_config = config.value("network", json::object());
//    auto network = create_network_with_input_encoding(n_input_dims, n_output_dims, encoding_config, network_config);
    auto network = create_network(n_input_dims, n_output_dims, network_config);

    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    double *ptr = mxGetPr(plhs[0]);

    auto ctx = new Context();
    ctx->ctx = nullptr;
    ;
    auto conf = new Config(ctx, network->n_input_dims(), network->n_output_dims());

    ptr[0] = (intptr_t) new netStreamPair(network, stream_ptr, conf);

}


void num_params(int nlhs, mxArray *plhs[],
                int nrhs, const mxArray *prhs[]) {
    auto [network_ptr, stream, tmp] = getPair(prhs[1]);
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    auto double_ptr = mxGetPr(plhs[0]);
    double_ptr[0] = network_ptr->n_params();

}


void forward(int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[]) {

    const int netPtrInd = 1;
    const int paramInd = 2;
    const int inputInd = 3;

    auto [network_ptr, stream, config] = getPair(prhs[netPtrInd]);

    mxAssert(mxIsGPUArray(prhs[paramInd]) != 0, "params must be a gpu array");
    mxAssert(mxIsGPUArray(prhs[inputInd]) != 0, "inputs must be a gpu array");

    auto paramGpuArray = mxGPUCopyFromMxArray(prhs[paramInd]);
    const mwSize *paramDims = mxGPUGetDimensions(paramGpuArray);
    int paramNumDims = mxGPUGetNumberOfDimensions(paramGpuArray);
    mxAssert(paramNumDims == 2 && paramDims[1] == 1, "params is a 1-d vector of network weights");

    auto inputGpuArray = mxGPUCopyFromMxArray(prhs[inputInd]);
    const mwSize *inputDims = mxGPUGetDimensions(inputGpuArray);
    int inputNumDims = mxGPUGetNumberOfDimensions(inputGpuArray);
    mxAssert(inputNumDims == 2, "Input must of shape (input_dim x batch_size)");
    mxAssert(inputDims[0] == config->n_input_dims, "Input must of shape (input_dim x batch_size)");

    auto input_gpu_ptr = mxGPUCopyFromMxArray(prhs[inputInd]);
    void *gpu_ptr = mxGPUGetData(input_gpu_ptr);
    float *input_d = static_cast<float *>(gpu_ptr);

    mxGPUArray *params_gpu_ptr = mxGPUCopyFromMxArray(prhs[paramInd]);
    gpu_ptr = mxGPUGetData(params_gpu_ptr);
    float *params = static_cast<float *>(gpu_ptr);

    mwSize batch_size = inputDims[1];
    const mwSize num_dims = 2;
    mwSize dims[num_dims];
    dims[0] = config->n_output_dims;
    dims[1] = batch_size;
    mxGPUArray *gpu_array_ptr = mxGPUCreateGPUArray(num_dims, dims, mxSINGLE_CLASS, mxREAL, MX_GPU_INITIALIZE_VALUES);
    float *output_d = static_cast<float *>(mxGPUGetData(gpu_array_ptr));

    tcnn::cpp::Context ctx = network_ptr->forward(*stream, batch_size, input_d, output_d, params, true);
    config->ctx->ctx = std::move(ctx.ctx);

    plhs[0] = mxGPUCreateMxArrayOnGPU(gpu_array_ptr);
//    plhs[1] = mxGPUCreateMxArrayOnGPU(input_gpu_ptr);

    mxGPUDestroyGPUArray(paramGpuArray);
    mxGPUDestroyGPUArray(inputGpuArray);

    mxGPUDestroyGPUArray(input_gpu_ptr);
    mxGPUDestroyGPUArray(params_gpu_ptr);
    mxGPUDestroyGPUArray(gpu_array_ptr);

}

void backward(int nlhs, mxArray *plhs[],
              int nrhs, const mxArray *prhs[]) {

    const int netPtrInd = 1;
    const int dL_doutpuInd = 2;
    const int inputInd = 3;
    const int outputInd = 4;
    const int paramsInd = 5;

    auto [network_ptr, stream, config] = getPair(prhs[netPtrInd]);

    // get const input data
    auto dL_doutput_gpu_ptr = mxGPUCopyFromMxArray(prhs[dL_doutpuInd]); // these should be mxGPUCreateFromMxArray
    const auto *dL_doutput = static_cast<const float *>(mxGPUGetDataReadOnly(dL_doutput_gpu_ptr));

    auto input_gpu_ptr = mxGPUCopyFromMxArray(prhs[inputInd]);
    const auto *input_d = static_cast<const float *>(mxGPUGetDataReadOnly(input_gpu_ptr));

    auto output_gpu_ptr = mxGPUCopyFromMxArray(prhs[outputInd]);
    const auto *output_d = static_cast<const float *>(mxGPUGetDataReadOnly(output_gpu_ptr));

    auto params_gpu_ptr = mxGPUCopyFromMxArray(prhs[paramsInd]);
    const auto *params = static_cast<const float *>(mxGPUGetDataReadOnly(params_gpu_ptr));

    // create memory for output
    const mwSize *inputDims = mxGPUGetDimensions(input_gpu_ptr);
    const mwSize numInputDims = mxGPUGetNumberOfDimensions(input_gpu_ptr);
    mwSize batch_size = inputDims[1];
    auto dL_dinput_gpu_ptr = mxGPUCreateGPUArray(numInputDims, inputDims, mxSINGLE_CLASS, mxREAL,
                                                 MX_GPU_INITIALIZE_VALUES);
    float *dL_dinput = static_cast<float *>(mxGPUGetData(dL_dinput_gpu_ptr));

    const mwSize *paramsDims = mxGPUGetDimensions(params_gpu_ptr);
    const mwSize numParamsDims = mxGPUGetNumberOfDimensions(params_gpu_ptr);
    auto dL_dparams_gpu_ptr = mxGPUCreateGPUArray(numParamsDims, paramsDims, mxSINGLE_CLASS, mxREAL,
                                                  MX_GPU_INITIALIZE_VALUES);
    float *dL_dparams = static_cast<float *>(mxGPUGetData(dL_dparams_gpu_ptr));

    network_ptr->backward(*stream, *config->ctx, batch_size, dL_dinput, dL_doutput, dL_dparams, input_d, output_d,
                          params);

    plhs[0] = mxGPUCreateMxArrayOnGPU(dL_dinput_gpu_ptr);
    plhs[1] = mxGPUCreateMxArrayOnGPU(dL_dparams_gpu_ptr);

    mxGPUDestroyGPUArray(input_gpu_ptr);
    mxGPUDestroyGPUArray(output_gpu_ptr);
    mxGPUDestroyGPUArray(params_gpu_ptr);
    mxGPUDestroyGPUArray(dL_doutput_gpu_ptr);
    mxGPUDestroyGPUArray(dL_dinput_gpu_ptr);
    mxGPUDestroyGPUArray(dL_dparams_gpu_ptr);

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
        case FORWARD:
            forward(nlhs, plhs, nrhs, prhs);
            break;
        case BACKWARD:
            backward(nlhs, plhs, nrhs, prhs);
            break;
        case NUM_PARAMS:
            num_params(nlhs, plhs, nrhs, prhs);
            break;
        default:
            break;
    }

}

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
//    auto network = create_network_with_input_encoding(n_input_dims, n_output_dims, encoding_config, network_config);
    auto network = create_network(n_input_dims, n_output_dims, network_config);

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