classdef TinyCUDA < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here

    properties(Constant, Access=private)
        INIT = 0
        DESTROY = 1
        FORWARD = 2;
        BACKWARD = 3;
        NUM_PARAMS = 4;
    end
    properties(Access=private)
        ptr
        params
    end

    methods
        function obj = TinyCUDA()
            encoding.otype = 'OneBlob';
            encoding.n_bins = 32;

            network.activation = 'ReLU';
            network.n_hidden_layers = 5;
            network.n_neurons = 128*4;
            network.otype = 'FullyFusedMLP';
            network.output_activation= 'None';

            optimizer.learning_rate = 1E-3;
            optimizer.otype = 'Adam';

            s.encoding = encoding;
            s.loss.otype = 'L2';
            s.network = network;
            s.optimizer = optimizer;

            json_str = jsonencode(s);
            n_input_dims = 2;
            n_output_dims = 1;
            obj.ptr = tiny_cuda_mex(obj.INIT, json_str, n_input_dims, n_output_dims);

            num_params = tiny_cuda_mex(obj.NUM_PARAMS, obj.ptr);
            obj.params = gpuArray(.1*(rand(num_params, 1,'single')-.5) );
        end

        function outputs = forward(obj, inputs)
            if ~isa(inputs, 'gpuArray')
                assert(0)
            end
            outputs = tiny_cuda_mex(obj.FORWARD, obj.ptr, obj.params, inputs);
        end

        function [dL_dinput, dL_dparams]  = backward(obj, input, output, dL_doutput)
            if ~isa(input, 'gpuArray') || ~isa(output, 'gpuArray') || ~isa(dL_doutput, 'gpuArray')
                assert(0)
            end
            [dL_dinput, dL_dparams] = tiny_cuda_mex(obj.BACKWARD, obj.ptr, dL_doutput, input, output, obj.params);
        end

        function out = getParams(obj)
            out = obj.params;
        end

        function setParams(obj, params)
            if ~isa(params, 'gpuArray')
                assert(0)
            end
            obj.params = params;
        end

        function delete(obj)
            tiny_cuda_mex(obj.DESTROY, obj.ptr);
        end
    end
end