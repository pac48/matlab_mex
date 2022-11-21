function [im, vertInds, allVerts2dOut, visibleInds] = render(scene)

allVerts = [];
allNormals = [];
allRGB = [];

objects = {};
if ~isempty(scene.actors)
    objects = scene.actors;
end
objects = cat(1,objects, arrayfun(@(x) x, scene.meshes, 'UniformOutput',false));

for i = 1:length(objects)
    actor = objects{i};

    % calculalte rendering data
    [verts, normals, RGB] = actor.forwardRender();


    allVerts = cat(2, allVerts, verts);
    allNormals = cat(2, allNormals, normals);
    allRGB = cat(2, allRGB, RGB);
end


% transform to camera coordinates
TInv = inv(scene.cameras(1).transform);
allVertsCamera = TInv(1:3,1:3)*allVerts + TInv(1:3, end);

% truncate non-visible values
tmp = reshape(allVertsCamera, 3, 3, []);
tmp = tmp - tmp(:, 1 ,:);
tmp = cross(tmp(:,2,:),  tmp(:,3,:) );
tmp = dot(tmp, mean(reshape(allVertsCamera, 3, 3, []), 2));
visibleInds = find(int32(reshape(tmp,1,[]) <= 0),99999999)-1;
visibleInds = cat(1,visibleInds,visibleInds,visibleInds)*3 + [0;1;2];
visibleInds = reshape(visibleInds,1,[])+1;

% project to pixel coordiantes
fx = 2/(2*tand(scene.cameras(1).xfov/2));
fy = fx*scene.cameras(1).aspect_ratio;

cameraMatrix = [fx 0 0
    0 fy 0
    0  0  1];


allRGB = allRGB(:,visibleInds);
allNormals = allNormals(:,visibleInds);

allVerts2dOut = cameraMatrix*allVertsCamera;
allVerts2dOut = allVerts2dOut(1:2,:)./abs(allVerts2dOut(3,:));

allVertsCamera = allVertsCamera(:,visibleInds);
allVerts2d = allVerts2dOut(:,visibleInds);




% calculate lighting
lighDir = scene.lights(1).direction;
lighDir = lighDir./norm(lighDir);
allColor = (1/1)*scene.lights(1).color.*max(-lighDir'*allNormals, .15).*allRGB;
% allColor = allRGB;


% get bounding boxes
xMins = min(reshape(allVerts2d(1,:), 3, []));
xMaxs= max(reshape(allVerts2d(1,:), 3, []));
yMins = min(reshape(allVerts2d(2,:), 3, []));
yMaxs= max(reshape(allVerts2d(2,:), 3, []));
boundingBox = cat(1, xMins, xMaxs, yMins, yMaxs);

% build quad tree
% [tree, distances] = getQuadTree(allVerts2d);
[tree, distances] = getQuadTree_mex(allVerts2d);
Inds = tree(12:end, :)+1;
distancesMod = cat(2, [0;0;0;0], distances);
idxAll = zeros(size(Inds, 1), size(Inds, 2), 4);
for i = 1:4
    tmp = reshape(distancesMod(i, Inds), [], size(tree,2));
    [~,idx] = sort(tmp,'descend');
    idxAll(:,:,i) = idx;
end


arg2 = gpuArray((allVerts2d));
arg3 = gpuArray((allVertsCamera));
arg4 = gpuArray((allColor));
arg5 = gpuArray((boundingBox));
arg6 = gpuArray(single(tree));
arg7 = gpuArray(single(idxAll));
arg8 = gpuArray(single(distances));


[im, vertInds] = renderZBuffer_mex(arg2,arg3,arg4,arg5,arg6,arg7,arg8);

end