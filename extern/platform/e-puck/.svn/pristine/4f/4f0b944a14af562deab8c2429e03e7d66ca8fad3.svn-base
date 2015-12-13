function [dist, dangle] = controllersub_GoToFct(goalPos)
% FUNCTION controllersub_GoToFcts(GOALPOS)
% Implement motion control from current position to a goal position GOALPOS.

global ePic;
% get current position in m
[val,up] = get(ePic,'odom');
curPos = val(1:2);
curAngle = val(3);

% controller parameters, an initial choice for the values is given but has to be
% adjusted later on in the exercise
k_rho = 0.3;
k_alpha = .8;
k_beta = -0.4;


% "v_c" is the robot's velocity in its longitudinal direction
% the values range from -1000 to +1000
% which corresponds approx. to max. 130mm/s
v_adapt = 1000/0.13;  % conversion-factor for speed in [m/s] to e-Puck speed units

% "omega_c" is the robot's rotational speed around the vertical axis
% (positiv for turns in counter-clockwise direction)
% the value is defined to range from -2000 to 2000
% representing turn rates of max. 270°/s
omega_adapt = 2000/(270*pi/180);  % conversion-factor for turn rate in [rad/s] to e-Puck speed units



% calculate current distance and angles to goal position
rho_c = norm([curPos(1)-goalPos(1) curPos(2)-goalPos(2)]);

% "beta_c" is the angle between the current direction to the goal
% and the heading to be obtained finally
beta_c = -atan2(goalPos(2)-curPos(2),goalPos(1)-curPos(1)) + goalPos(3);

alpha_c = -beta_c-curAngle + goalPos(3);
if (alpha_c>pi) % to prevent alpha from getting to big
	alpha_c = alpha_c-2*pi;
elseif (alpha_c<-pi)
	alpha_c = alpha_c+2*pi;
end

% control law
v_c = k_rho*rho_c;
omega_c = k_alpha*alpha_c+k_beta*beta_c;


% adapt SI values to e-Puck units
v_e = v_c*v_adapt;
omega_e = omega_c*omega_adapt;


% finally set motor speed
v_left = int16(v_e-omega_e/2);
v_right = int16(v_e+omega_e/2);

dist = rho_c;
dangle = beta_c;

ePic = set(ePic,'speed',[v_left v_right]);