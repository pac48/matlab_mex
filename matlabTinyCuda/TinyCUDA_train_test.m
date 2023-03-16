%% init
close all
tc = TinyCUDA();
s = RandStream('mlfg6331_64'); 

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
x = linspace(-1, 1.0, 32*4);
[X,Y] = meshgrid(x);
edgeInds = X==1 | Y==1 | Y==-1 | X==-1;

U = X.*exp(-(2*X).^2-(2*Y).^2);
U = U*0;
U(edgeInds) = -.1;

U(100:104, 100:104) = .1;
edgeInds(100:104, 100:104) = 1;

U(44:56, 44:56) = -.1;
edgeInds(44:56, 44:56) = 1;


% U(44:56, (44:56)+50) = -.1;
% edgeInds(44:56, (44:56)+50) = 1;


edgeInds = reshape(edgeInds, 1,[]);


figure(1)
subplot(1,2,1)
surf(X, Y, U)



Xflat = reshape(X, 1,[]);
Yflat = reshape(Y, 1,[]);

tmp = cat(1, Xflat, Yflat);
XGpu = gpuArray(tmp);
UGpu = gpuArray(reshape(U, 1, []));

UGpu = cat(1, UGpu, zeros(15, size(UGpu,2)));
XGpu = single(XGpu);
UGpu = single(UGpu);

params0 = double(gather(tc.getParams()));
% params0 = params0*0 + .1*(rand(size(params0))-.5);

params = gpuArray(single(params0));
%% train
population = 1:size(XGpu,2);

delta = 0.00005;
batch_size = 128;
factor2 = .001*(1/batch_size);

uDiff = gpuArray(zeros(16, batch_size, 'single'));
dL_dUpred1 = gpuArray(zeros(16, batch_size, 5, 'single'));
dL_dUpred2 = gpuArray(zeros(16, batch_size, 5, 'single'));

XGpuAll = cat(3, XGpu, XGpu+[delta;0], XGpu+[-delta;0], XGpu+[0; delta], XGpu+[0; -delta]);


for i = 1:2000
    inds = randsample(s, population, batch_size);
    XGpui = XGpuAll(:, inds, :);
    UGpui = UGpu(:, inds);
    edgeIndsi = edgeInds(:, inds);

    tc.setParams(params);
    UPred = tc.forward(reshape(XGpui, 2, []) );
    UPred = reshape(UPred, 16, batch_size, []);

    uDiff(:, :) = (UPred(:, :, 1) - UGpui).*edgeIndsi;
    uDiff(2:end,:) = 0;
    factor1 = 1;%(10/(1+sum(edgeIndsi)));
    loss1 = factor1*sum((uDiff).^2, 'all');
    dL_dUpred1(:, :, 1) = factor1*2*uDiff;

%     L = (UPred(:, :, 5) + UPred(:, :, 4) +  UPred(:, :, 3) + UPred(:, :, 2) - 4*UPred(:, :, 1))./delta;
    Lx = ((UPred(:, :, 2)-UPred(:, :, 1)) - (UPred(:, :, 1)-UPred(:, :, 3)) )./delta^2;
    Ly = ((UPred(:, :, 4)-UPred(:, :, 1))  - (UPred(:, :, 1)-UPred(:, :, 5)))./delta^2;

    L = Lx+Ly;
    L = L.*(1-edgeIndsi);
    L(2:end,:) = 0;

    loss2 = factor2*sum((L).^2, 'all');
    dL_dUpred2(:,:,1) = factor2*(-4)*2*L;
    dL_dUpred2(:,:, 2) = factor2*(1)*2*L;
    dL_dUpred2(:,:, 3) = factor2*(1)*2*L;
    dL_dUpred2(:,:, 4) = factor2*(1)*2*L;
    dL_dUpred2(:,:, 5) = factor2*(1)*2*L;
    
    
    dL_dUpred = dL_dUpred1 + dL_dUpred2;
    [dL_dX, dL_dparams]  = tc.backward(reshape(XGpui, 2, []), reshape(UPred, 16,[]), reshape(dL_dUpred, 16,[]));

    if (mod(i, 20)) == 0
        loss = double(gather(loss1)) + double(gather(loss2))
    end
    params = params - .1*dL_dparams;

end



%% train
if false
params0 = double(gather(tc.getParams()));
objFun = @(parameters) testTinyCudaobjectiveFunction(parameters, tc, XGpu, UGpu);
params = fmincon(objFun, params0,[],[],[],[],[],[],[],options);
end
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
surf(X, Y, UPred)

figure(2);contourf(UPred, 100)

% figure
% surf(X, Y, reshape(gather(UGpu), size(Y) ))

