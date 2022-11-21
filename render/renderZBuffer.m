function [im, vertInds] = renderZBuffer(allVerts2d, allVertsCamera, ...
    allColor, boundingBox, tree, idxAll, distances) %#codegen

numThreads = 256;
% resolution = 256*4;
im = coder.nullcopy(zeros(480,640, 3, 'single'));
resY = size(im,1);
resX = size(im, 2);
% vertInds = coder.nullcopy(zeros(resX, resY, 'int32'));
vertInds = coder.nullcopy(zeros(resX, resY));

coder.gpu.kernel([resY*resX/numThreads  1 1],[numThreads 1 1]);

% for px = int32(1):resX
%     for py = int32(1):resY
for px = 1:resX
    for py = 1:resY
        [color, faceInd] = kernalFunc(px, py, resX, resY, allVerts2d, allVertsCamera, ...
            allColor, boundingBox, tree, idxAll, distances);
        im(resY - py + 1, px,:) = color;
        vertInds(resY - py + 1, px) = faceInd;

    end
end

end


function [color, faceInd] = kernalFunc(px, py, resX, resY, allVerts2d, allVertsCamera, ...
    allColor, boundingBox, tree, idxAll, distances) %#codegen

faceInd = int32(-1);
pixelX = 2*single(px - resX/2)/single(resX);
pixelY = 2*single(py - resY/2)/single(resY);
% pixelX = 2*(px - resX/2)/(resX);
% pixelY = 2*(py - resY/2)/(resY);
depth = single(inf);
% depth = (inf);
color = zeros(3, 1,'single');
% color = coder.nullcopy(zeros(3, 1));

overflowChild = int32(5);
leftLim = int32(6);
rightLim = int32(7);
bottomLim = int32(8);
topLim = int32(9);
countInd = int32(10);
vertsStart = int32(12);


binInd = int32(1);
count = 0;

while binInd ~= 0
    binInd2 = binInd;
    midX = tree(leftLim, binInd)+(tree(rightLim, binInd) - tree(leftLim, binInd))/2;
    midY = tree(bottomLim, binInd)+(tree(topLim, binInd) - tree(bottomLim, binInd))/2;
    childInd = single(1);
    childInd = childInd + 2*(pixelY<midY);
    childInd = childInd + 1*(pixelX>midX);

    distInd1 = mod(childInd-1, 2)+1;
    distInd2 = 4-floor((childInd-1)/2);
    distInds = [distInd1 distInd2];

    deltaX = abs(pixelX - midX);
    deltaY = abs(pixelY - midY);
    deltas = [deltaX deltaY];

    % for k = 1:4
    % distInd1 = mod(k-1, 2)+1;
    %     distInd2 = floor((k-1)/2)+3;
    %     [distInd1 distInd2]
    % end
    % child1: dist 1 4
    % child2: dist 2 4
    % child3: dist 1 3
    % child4: dist 2 3

    while binInd2 ~= 0 % loop through all overflow
        for dInd = 1:2
            i = int32(0);
            while i < int32(tree(countInd, binInd2))
                i = i+1;
                ind = int32(idxAll(i, binInd2, distInds(dInd)))-1;
                vertexInd = int32(tree(vertsStart + ind, binInd2));
                count = count+1;

                if distances(distInds(dInd), vertexInd) < deltas(dInd)
                    break
                end

                if  (pixelX < boundingBox(1, vertexInd)) ...
                        || (pixelX > boundingBox(2, vertexInd)) ||(pixelY < boundingBox(3, vertexInd))...
                        || pixelY > boundingBox(4, vertexInd)
                    continue
                else
                    v = 3*(vertexInd-1) + 1;
                    verts2d = allVerts2d(:, int32(0:2) + v);
                    point2d = [pixelX; pixelY] - verts2d(:,1);
                    verts2d = verts2d - verts2d(:,1);

                    u = [verts2d(:, 2) verts2d(:, 3)]\point2d;
                    if any(u < 0) || any(u > 1) || u(1)+u(2)>1 % miss
                        continue
                    end

                    verts = allVertsCamera(:, int32(0:2) + v);
                    point = verts(:,1) + (verts(:,2:3)-verts(:,1))*u;

                    d = norm(point);
                    if d < depth
                        depth = d;
                        colors = allColor(:, int32(0:2) + v);
                        color = colors(:,1) + (colors(:,2:3)-colors(:,1))*u;
                        faceInd = vertexInd;
                    end
                end

            end
        end
        binInd2 = int32(tree(overflowChild, binInd2));

    end
    binInd = int32(tree(childInd, binInd));
end

% count

end
