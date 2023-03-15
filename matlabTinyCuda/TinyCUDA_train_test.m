%% init
tc = TinyCUDA();

options = optimoptions("fmincon", ...
    HessianApproximation="lbfgs", ...
    MaxIterations=100, ...
    MaxFunctionEvaluations=7500, ...
    OptimalityTolerance=1e-5, ...
    SpecifyObjectiveGradient=true ...
    );

%% data
x = linspace(-1, 1, 16);
y = x;
[X,Y] = meshgrid(x);
U = X.*exp(-(2*X).^2-(2*Y).^2);
figure(1)
subplot(1,2,1)
surf(X, Y, U)


tmp = cat(3, X, Y);
tmp = permute(tmp,[3 1 2]);
XGpu = gpuArray(tmp);
XGpu = reshape(XGpu, 2, []);
UGpu = reshape(gpuArray(U), 1, []);
UGpu = cat(1, UGpu, zeros(15, size(UGpu,2)));
XGpu = single(XGpu);
UGpu = single(UGpu);

%% train
objFun = @(parameters) testTinyCudaobjectiveFunction(parameters, tc, XGpu, UGpu);
params0 = double(gather(tc.getParams()));
params = fmincon(objFun, params0,[],[],[],[],[],[],[],options);

%% vis
tc.setParams(gpuArray(single(params)));

UPred = tc.forward(single(XGpu));
UPred = UPred(1, :);
UPred = reshape(gather(UPred), size(Y));
figure(1)
subplot(1,2,2)
surf(X, Y, UPred)

% figure
% surf(X, Y, reshape(gather(UGpu), size(Y) ))

