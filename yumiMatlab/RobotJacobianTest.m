
robot = Yumi();
% robot = Sawyer();

% q = robot.getJoints();
% robot.setJoints(q+rand(size(q)) );


tic
for i = 1:100
    J = robot.getControlPointsJacobians();

    q = robot.getJoints();
    tmp = 0.02*sin(toc);
    qd = pinv(J)*[0; 0; tmp; 0;0;-0.0];
    robot.setJoints(q+qd);

    robot.plotObject
    drawnow
end