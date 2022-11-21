close all

% addpath('DAE\');
% addpath('mex/');

filename = 'data/female.dae';
% filename = 'yumi.dae';
% filename = 'sawyer.dae';
% [controllers, meshes, cameras, lights] = parseDAEMex(filename);

scene = Scene(filename);

%%
meshes = [];
if ~isempty(scene.actors)
    meshes = cat(1,meshes, scene.actors.skin);
end
meshes = cat(1,meshes, scene.meshes);

for i = 1:length(meshes)
    figure
    mesh = meshes(i);
    verts = mesh.verts;
    scatter3(verts(:,1), verts(:,2), verts(:,3)); hold on
    axis equal
    figure
    plot(mesh.textureCoords(:,1), mesh.textureCoords(:,2), 'Marker','.','LineStyle','none')
end





for i = 1:length(meshes)
    figure
    mesh = meshes(i);
%     verts = mesh.verts;
    lighDir = [-1; -.5; -1];
    lighDir = lighDir./norm(lighDir);
    tmp = verts;%+ rand(1,3);
    [verts, normals, RGB] = mesh.forwardRender(); 
    T = ones(4);
    vNew = cat(2, tmp, ones(size(tmp,1), 1))*T';
    vNormNew = mesh.normals;% + rand(1,3);
    color = max(-vNormNew*lighDir, .3);

    vNew = vNew(:,1:3);
    T = reshape(1:size(vNew, 1), 3, [])';

    trisurf(T, tmp(:, 1), tmp(:, 2),  tmp(:, 3), color); hold on
    axis equal
    % ylim([-.7 .7])
    % xlim([-.5 .9])
    % zlim([-0 1.2])
    % view(120, 15)
    colormap gray
    shading interp
end