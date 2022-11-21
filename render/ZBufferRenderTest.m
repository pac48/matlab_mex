

% addpath('DAE');
% addpath('DAE/mex/');
% addpath('URDF');
% addpath('URDF/mex/');

% human = Human('female');
% robot = Robot('yumi');

p = which('ZBufferRenderTest.m');
directory = p(1:max(find(p==filesep,9999)));
filename = fullfile(directory, '..', 'DAELoader', 'data', 'female.dae');
%  [~, ~, cameras, lights] = parseDAEMex(filename);
 scene = Scene(filename);

 human = scene.actors{1};
% scene = struct();
% scene.lights = lights;
% scene.actors = {human, robot}; robot broken
% human2 = Human('female');
% human2.transform(2,end)= .5;
% human3 = Human('female');
% human3.transform(2,end)= -.5;

% scene.actors = {human, human2, human3};
% scene.cameras = cameras;
% scene.meshes = meshes;


% scene.cameras{1}.xfov = 100;
im  = zeros(256*2,256*2,3,'single');
for x = 0:.1:2*pi*88 % pi/2;
    tic

    Trot = [eul2rotm([.1 0 0]) [0;0;0]; 0 0 0 1]';
%     scene.cameras{1}.transform = Trot*scene.cameras{1}.transform;
%     human.transform = Trot*human.transform;
    human.boneRotations(:,:,6) = human.boneRotations(:,:,6)*eul2rotm([.1 0 0]);
    im = render(scene);
    img = gather(im);
    image(img)
    drawnow

    toc

end