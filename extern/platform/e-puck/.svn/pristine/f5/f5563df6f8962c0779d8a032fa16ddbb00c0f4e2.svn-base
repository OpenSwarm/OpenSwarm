function controller()
% This function is executed after the update data timer

% global ePic object. Use get and set methods to access the different
% fields and check if the required sensors are activated using the 
% updateDet methode. For more information about this different commands, 
% please read the help file.
global ePic;                    



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
if (ControllerState==1) 
  disp 'Controller has been switched on!';
  ControllerState = 2;
  
  %----------------------------%
  % setup controller variables %  
  %----------------------------%
  
  % ******* PUT YOUR CONTROLLER INITIALISATION CODE HERE ********
  % you can comment of edit these lines to write you controller
  
  % activate the requiered sensors
  ePic = activate(ePic,'accel');
  ePic = activate(ePic,'proxi');
  %ePic = activate(ePic,'image'); % activate the camera
  % ...... add other sensors
  
  % desactivate the unrequired sensors
  ePic = deactivate(ePic, 'light');
  % ...... add other sensors
  
  % reset odometry
  ePic = set(ePic, 'odom' , [0 0 0]);
  
  % put your variable initialization code here
  done = 0;
  

%-------------------------------------------------------------------------%  
% if controller is to be switched off, execute termination code and go to "off"-state  
elseif (ControllerState==-1)
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

 if (done==0)    
     % ******* PUT YOUR CONTROLLER MAIN CODE HERE ********
     
     % read the sensors values
     [val, up] = get(ePic, 'accel'); % read accelerometer values
     % ........ read other sensors
   
     % if you want to use the camera from the ePuck, uncomment
     % thoses lines and activate camera during initialisation
     % ePic = updateImage(ePic);
     % [image, up] = get(ePic, 'image');
     
    % determine somewhere here, if variable "done" has to be set to 1 to
    % end the controller
     
    % finally set motor speeds
    v_left = 100;
    v_right = 100;
    ePic = set(ePic,'speed',[v_left,v_right]);
 else
    ControllerState = -2;  % go to suspend state
 end  
    
end