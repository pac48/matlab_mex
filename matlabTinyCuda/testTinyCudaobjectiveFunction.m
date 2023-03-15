function [loss, gradients] = testTinyCudaobjectiveFunction(parameters, tc, X, U)

tc.setParams(gpuArray(single(parameters)));
UPred = tc.forward(X);

uDiff = UPred - U;
loss = sum((uDiff).^2, 'all');
dL_dUpred = 2*uDiff;

[dL_dX, dL_dparams]  = tc.backward(X, UPred, dL_dUpred); 


loss = double(gather(loss))
gradients = double(gather(dL_dparams));

end