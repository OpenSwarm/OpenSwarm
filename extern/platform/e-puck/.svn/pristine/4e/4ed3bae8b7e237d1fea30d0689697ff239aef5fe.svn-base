% controller_pos is an exemple controller. 
% ---------------------------------------------
% It drives the epuck from the current position which is define as [0 0 0]
% to a goal position which can be set by the user.
% The control uses a smooth controller which drives the e-puck along 
% smooth curves from the current position to the goal position.


function controller_pos()
% This function is executed after the update data timer

% global ePic object. Use get and set methods to access the different
% fields and check if the required sensors are activated using the 
% updateDet methode. For more information about this different commands, 
% please read the help file.
global ePic;
global handles_save;
persistent goal_position;



% a global variable for the controller state
global ControllerState; % 0 = controller halted, all other states are active transition or static states;

% Controller states (content of ControllerState variable)
% 0 = "controller off" state
% 1 = transition to "controller on" state is in progress (initiated by user in main.m)
% -1 = transition to "controller off" state is in progress (initiated by user in main.m)
% -2 = means that the controller is in "suspend" state (automatically activated when control goal has been reached)
%
% all other states can be used freely in this function (e.g. to signify "controller on" state)



% put your controller variable declarations here (if possible not declared
% as "global" but as "persistent")
persistent done;


%-----------------------------------------------%
% main code for the different controller states %
%-----------------------------------------------%

%-------------------------------------------------------------------------%
% if controller is to be switched on, execute initialization code and go to
% "on"-state
if (ControllerState == 1)
    disp 'Controller has been switched on!';
	ControllerState = 2;

	%----------------------------%
	% setup controller variables %
	%----------------------------%
    % activate required "sensors"
    ePic = activate(ePic,'speed');
    ePic = activate(ePic,'pos');
    ePic = activate(ePic,'odom');
    % reset odometry
    ePic = set(ePic, 'odom' , [0 0 0]);
    
    % ----------- set goal position in m  ----------- 
    % [m m rad]
    goal_position = [-0.2 -0.3 .64];
    
	done = 0;


	%-------------------------------------------------------------------------%
	% if controller is to be switched off, execute termination code and go to "off"-state
elseif (ControllerState == -1)
    
	ePic = set(ePic,'speed',[0 0]);
	disp 'Controller has been switched off!';
	ControllerState = 0;


	%-------------------------------------------------------------------------%
	% if controller is in suspend state
elseif (ControllerState==-2)
	% don't do anything, and wait for user to switch controller off


	%-------------------------------------------------------------------------%
	% controller running, write your own code here
elseif (ControllerState~=0)

	if (done == 0)
        % call GoTo function
        [dist, dangle] = controllersub_GoToFct(goal_position);
        
        [val,up] = get(ePic,'odom');
        
        % comment this lines if you don't want to display the position graph
        figure(432);
        scatter([goal_position(1), val(1)], [goal_position(2), val(2)], 'b');
        hold on;
        
        % check for limits
        if (dist < 0.005)
            done = 1;
            ePic = set(ePic,'speed',[0 0]);
        end
    else
		ControllerState = -2;  % go to suspend state
    end
end