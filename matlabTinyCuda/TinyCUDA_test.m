%% init
tc = TinyCUDA();

%% num_params
tc.params


%% forward
input = gpuArray(zeros(2, 128, 'single'));

output = tc.forward(input);


%% destroy
clear tc