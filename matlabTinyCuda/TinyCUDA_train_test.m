%% init
tc = TinyCUDA();

options = optimoptions("fmincon", ...
    HessianApproximation="lbfgs", ...
    MaxIterations=100, ...
    MaxFunctionEvaluations=7500, ...
    OptimalityTolerance=1e-5, ...
    SpecifyObjectiveGradient=true ...
    );

% options = optimoptions('fmincon','Algorithm','interior-point',...
%     "SpecifyConstraintGradient",true,"SpecifyObjectiveGradient",true,...
%     'MaxIterations', 100, ...
%     'HessianFcn', []);

%% data
x = linspace(-1, 1.0, 32);
[X,Y] = meshgrid(x);
U = X.*exp(-(2*X).^2-(2*Y).^2);
figure(1)
subplot(1,2,1)
surf(X, Y, U)


Xflat = reshape(X, 1,[]);
Yflat = reshape(Y, 1,[]);

tmp = cat(1, Xflat, Yflat);
XGpu = gpuArray(tmp);
UGpu = gpuArray( Xflat.*exp(-(2*Xflat).^2-(2*Yflat).^2) );

UGpu = cat(1, UGpu, zeros(15, size(UGpu,2)));
XGpu = single(XGpu);
UGpu = single(UGpu);

params0 = double(gather(tc.getParams()));
% params0 = params0*0 + .1*(rand(size(params0))-.5);

%% train
params = gpuArray(single(params0));
population = 1:size(XGpu,2);
s = RandStream('mlfg6331_64'); 

for i = 1:100000
    inds = randsample(s, population, 128);
%     inds = 1:128;
    XGpui = XGpu(:, inds);
    UGpui = UGpu(:, inds);

    batch_size = size(UGpui,2);
    factor = (20/batch_size);
    tc.setParams(params);
    UPred = tc.forward(XGpui);
    uDiff = UPred - UGpui;
    uDiff(2:end, :) = 0;

    loss = factor*sum((uDiff).^2, 'all');
    dL_dUpred = factor*2*uDiff;
    [dL_dX, dL_dparams]  = tc.backward(XGpui, UPred, dL_dUpred);

    if (mod(i, 20)) == 0
        loss = double(gather(loss))
    end
    params = params - .1*dL_dparams;

end



%% train
% objFun = @(parameters) testTinyCudaobjectiveFunction(parameters, tc, XGpu, UGpu);
% params = fmincon(objFun, params0,[],[],[],[],[],[],[],options);

%% vis
% x = linspace(-1, 1, 16*2);
% y = x;
% [X,Y] = meshgrid(x);
% Xflat = reshape(X, 1,[]);
% Yflat = reshape(Y, 1,[]);
% tmp = cat(1, Xflat, Yflat);
% XGpu = gpuArray(tmp);
% XGpu = reshape(XGpu, 2, []);
% XGpu = single(XGpu);
% XGpu = XGpu(:,1:4:end)

UPred = tc.forward(XGpu);
UPred = UPred(1, :);
UPred = reshape(gather(UPred), [sqrt(size(XGpu,2)) sqrt(size(XGpu,2))]);
figure(1)
subplot(1,2,2)
% surf(X, Y, UPred)
surf(UPred)

% figure
% surf(X, Y, reshape(gather(UGpu), size(Y) ))

