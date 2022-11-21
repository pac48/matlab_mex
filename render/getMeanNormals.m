function meanNorms = getMeanNormals(normals, vertOrder, meanNorms)

nums = zeros(size(meanNorms,2), 1);
tmp3 = normals';
for n = 1:length(vertOrder)
    ind = vertOrder(n);
    nums(ind) = nums(ind)+1; 
end
tmp4 = meanNorms(:, vertOrder) + tmp3;
for n = 1:length(vertOrder)
    ind = vertOrder(n);
    meanNorms(:, ind) = meanNorms(:, ind) + tmp4(:,n);
end
meanNorms = meanNorms./nums';

end