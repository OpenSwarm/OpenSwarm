function ePic = ePicKernel()
% ePicKernel constructor for an ePicKernel object.


% ePic.update - Values to update at next timestep
% if 0=do not update, 1=update, 2=update once
ePic.update.accel = 1;         % accelerometers
ePic.update.proxi = 1;         % proximity
ePic.update.light = 1;         % light sensor
ePic.update.micro = 1;         % microphones
ePic.update.speed = 1;         % motor speed
ePic.update.pos = 1;           % motor position
ePic.update.odom = 1;          % odometry
ePic.update.floor = 0;         % floor sensors
ePic.update.image = 0;         % camera
ePic.update.exter = 0;         % external sensor
ePic.update.custom = 0;        % custom command

% ePic.updated - Value that have been updated at last timestep
ePic.updated.accel = 0;         % accelerometers
ePic.updated.proxi = 0;         % proximity
ePic.updated.light = 0;         % light sensor
ePic.updated.micro = 0;         % microphones
ePic.updated.speed = 0;         % motor speed
ePic.updated.pos = 0;           % motor position
ePic.updated.odom = 0;          % odometry position
ePic.updated.floor = 0;         % floor sensorss
ePic.updated.image = 0;         % camera
ePic.updated.exter = 0;         % external sensor
ePic.updated.custom = 0;        % custom command

% ePic.value - Value of each different parameters
ePic.value.accel = zeros(1,3);   % accelerometers
ePic.value.proxi = zeros(1,8);   % proximity
ePic.value.light = zeros(1,8);   % light sensor
ePic.value.micro = zeros(1,3);   % microphones
ePic.value.speed = zeros(1,2);   % motor speed
ePic.value.pos = zeros(1,2);     % motor position
ePic.value.pos_old = zeros(1,2); % old motor position used for odometry
ePic.value.odom = zeros(1,3);    % odometry position : x, y, theta
ePic.value.floor = zeros(1,3);   % floor sensors
ePic.value.image = 0;            % camera
ePic.value.exter = 0;            % external sensor
ePic.value.custom = 0;           % custom command

% ePic.set - Value to set at next timestep;
ePic.set.speed = zeros(1,2);     % set speed at 0
ePic.set.pos = zeros(1,2);       % set position at 0
ePic.set.led = zeros(1,10);      % switch led on (8 body, 9 front, 10 all)
ePic.clear.led = ones(1,10);     % switch led off


% ePic.param - Parameter of ePic
ePic.param.connected = 0;       % 1=connected, 0=not connected
ePic.param.comPort = 0;         % connected communication port
ePic.param.controllerState = 0; % controller state
ePic.param.odomIni = 0;         % define if odometry has been initialized of not
ePic.param.imgMode = 1;         % camera mode
ePic.param.imgSize = [40 40];   % image size
ePic.param.imgZoom = 8;         % camera zoom factor
ePic.param.imgMod = 1;          % if camera param has been modified since last updateimage
ePic.param.extSel = 8;          % selected external sensor
ePic.param.ledIR = zeros(1,5);  % select the led of the IR sensor
ePic.param.customCommand = [];   % send a list of custom command
ePic.param.customSize = 0;       % size of the data to receive resulting from the custom command

ePic = class(ePic,'ePicKernel');
