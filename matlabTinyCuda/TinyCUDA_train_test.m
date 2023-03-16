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
x = linspace(-1, 1, 32*2);
[X,Y] = meshgrid(x);

% U = (X-2).*exp(-(2*(X-2)).^2-(2*(Y-2)).^2);
U = zeros(size(X));

edgeInds = X==1 | Y==1 | Y==-1 | X==-1;
U(edgeInds) = -.1;


tmp = (X>.55 & X<.62) & (Y>.55 & Y<.62);
U(tmp) = .1;
edgeInds(tmp) = 1;

tmp = (X>-.3 & X<-.1) & (Y>-.3 & Y<-.1);
U(tmp) = -.1;
edgeInds(tmp) = 1;

tmp = (X>.3 & X<.5) & (Y>-.3 & Y<-.1);
U(tmp) = -.1;
edgeInds(tmp) = 1;


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

delta = (x(2)-x(1));%0.05;
batch_size = 128;
factor2 = .05*(1/batch_size);
factor3 = 10;

uDiff = gpuArray(zeros(16, batch_size, 'single'));
dL_dUpred1 = gpuArray(zeros(16, batch_size, 5, 'single'));
dL_dUpred2 = gpuArray(zeros(16, batch_size, 5, 'single'));
dL_dUpred3 = gpuArray(zeros(16, batch_size, 5, 'single'));

XGpuAll = cat(3, XGpu, XGpu+[delta;0], XGpu+[-delta;0], XGpu+[0; delta], XGpu+[0; -delta]);


for i = 1:10000
    inds = randsample(s, population, batch_size);
    XGpui = XGpuAll(:, inds, :);
    UGpui = UGpu(:, inds);
    edgeIndsi = edgeInds(:, inds);

    tc.setParams(params);
    UPred = tc.forward(reshape(XGpui, 2, []) );
    UPred = reshape(UPred, 16, batch_size, []);

    uDiff(:, :) = (UPred(:, :, 1) - UGpui).*edgeIndsi;
    uDiff(2:end,:) = 0;
%     factor1 = 3;%(10/(1+sum(edgeIndsi)));
    factor1 = 10/(.1+sum(edgeIndsi));
    loss1 = factor1*sum((uDiff).^2, 'all');
    dL_dUpred1(:, :, 1) = factor1*2*uDiff;

%     dt = 0.001;
%     L = (UPred(:, :, 5) + UPred(:, :, 4) +  UPred(:, :, 3) + UPred(:, :, 2) - 4*UPred(:, :, 1) )./delta.^2;

    w = exp(UPred(:,:,2:end))./sum(exp(UPred(:,:,2:end)),3);
%     w = exp(w)./sum(exp(w),3);
%     w = exp(w)./sum(exp(w),3);
    L = (w(:,:,4).*UPred(:, :, 5) + w(:,:,3).*UPred(:, :, 4) +  w(:,:,2).*UPred(:, :, 3) + w(:,:,1).*UPred(:, :, 2) - 4*UPred(:, :, 1) );
    

%     UPredMax = max(UPred(:,:,2:end),[], 3);
%     UPredMin = min(UPred(:,:,2:end),[], 3);
%     L = (4*.99*UPredMax + 4*.01*UPredMin - 4*UPred(:, :, 1) )./delta.^2; 
%     L = (4*UPredMax - 4*UPred(:, :, 1) )./delta.^2; 
    
%     L = dt*L;

    %     Lx = ((UPred(:, :, 2)-UPred(:, :, 1)) - (UPred(:, :, 1)-UPred(:, :, 3)) )./delta^2;
%     Ly = ((UPred(:, :, 4)-UPred(:, :, 1))  - (UPred(:, :, 1)-UPred(:, :, 5)))./delta^2;
%     L = dt*(Lx+Ly);

    L = L.*(1-edgeIndsi);
    L(2:end,:) = 0;
    loss2 = factor2*sum((L).^2, 'all');
    dL_dUpred2(:,:,1) = factor2*(-4)*2*L;


    UPredMax = max(UPred(:,:,2:end),[], 3);
    maxViolation = UPred > UPredMax;
    maxViolation = maxViolation.*(1-edgeIndsi);
    Udiff2 = maxViolation.*(UPred - (-.1));
    Udiff2(2:end,:) = 0;
    loss3 = factor3*sum((Udiff2).^2, 'all'); 
    dL_dUpred3 = factor3*2*Udiff2;
    
    
    dL_dUpred = dL_dUpred1 + dL_dUpred2 + dL_dUpred3;
    [dL_dX, dL_dparams]  = tc.backward(reshape(XGpui, 2, []), reshape(UPred, 16,[]), reshape(dL_dUpred, 16,[]));

    if (mod(i, 20)) == 0
        loss3 = double(gather(loss3))
        loss2 = double(gather(loss2))
         loss1 = double(gather(loss1))
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

figure(2);contourf(X, Y,UPred, 100)

% figure
% surf(X, Y, reshape(gather(UGpu), size(Y) ))

