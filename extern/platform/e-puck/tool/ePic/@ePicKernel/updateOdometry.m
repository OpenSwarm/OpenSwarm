function ePic = updateOdometry(ePic)
% This function will compute the ePuck position by odometry. The wheel
% encoders update flag need to be enable to perform odometry
%   
% use the methode '[ePic] = updateDef(ePic, propName, value)' to enable
% odometry and wheel encoder
%
% ePic = updateOdometry(ePic)
%
% Results :
%   ePic            :   updated ePicKernel object
%
% Parameters :
%   ePic            :   ePicKernel object

persistent dpos;
if (isempty(dpos) == 1)  % initialize dpos is necessary
    dpos = [0 0 0];
end

ePic.updated.odom = 0;

if (ePic.updated.pos == 0 && ePic.update.odom > 0)
    disp 'Odometry has not been updated. Wheel position must be updated before using "update" method';
end

% If the position has been updated
if (ePic.updated.pos > 0 && ePic.update.odom > 0)
    % calculate the trajectory by odometry
    if (ePic.param.odomIni == 0)
        ePic.value.pos_old = ePic.value.pos;
        ePic.param.odomIni = 1;
        dpos = [0 0 0];
    else
        dsr = (int16(ePic.value.pos(2))-int16(ePic.value.pos_old(2)));   % difference of the right wheel encoders from the last step (right wheel encoder ePic.value.pos are <0 for forward movement)
        dsl = (int16(ePic.value.pos(1))-int16(ePic.value.pos_old(1)));   % difference of the left wheel encoders from the last step

        if ((abs(dsr)<500) && (abs(dsl)<500))                   % otherwise something went wrong with the lecture of new wheel step encoder ePic.value.pos
            ePic.value.pos_old = ePic.value.pos;                % store for the next step                
            drob = 0.0535;                                      % robot wheel distance in [m]
            cwheel = 0.0412*pi/1000;                            % the circumference of a wheel (41.2mm*pi) equals to 1000 steps, in [m/steps]

            dpos(1) = double((dsr+dsl))*cwheel/2*cos(ePic.value.odom(3));      % difference for the robot's movement in the x axis
            dpos(2) = double((dsr+dsl))*cwheel/2*sin(ePic.value.odom(3));      % difference for the robot's movement in the y axis
            dpos(3) = double((dsr-dsl))*cwheel/drob;                           % difference of the current theta value to the theta value of the last step

            ePic.value.odom(1) = ePic.value.odom(1) + dpos(1);                    % update robot position's x value
            ePic.value.odom(2) = ePic.value.odom(2) + dpos(2);                    % update robot position's y value
            ePic.value.odom(3) = ePic.value.odom(3) + dpos(3);                    % update theta value    
        end    
    end
    ePic.updated.odom = 1;      % odometry has been updated
end
if (ePic.update.odom > 1)   % disable odometry if update value is sup to 1
    ePic.update.odom = 0;
end

