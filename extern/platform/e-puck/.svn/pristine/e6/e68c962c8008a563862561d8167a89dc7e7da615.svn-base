function ePic = update(ePic)
% update ePic parameter. 
% ask for selected sensors information and set motor, led and other
% parameters values on the ePuck.
%
% use the methode '[ePic] = updateDef(ePic, propName, value)' to define
% which sensor will be updated
%   
% ePic = update(ePic)
%
% Results :
%   ePic            :   updated ePicKernel object
%
% Parameters :
%   ePic            :   ePicKernel object


if (ePic.param.connected == 0)
    return;
end

% reset updated values
ePic.updated.accel = 0;
ePic.updated.proxi = 0;
ePic.updated.light = 0;
ePic.updated.micro = 0;
ePic.updated.speed = 0;
ePic.updated.pos = 0;
ePic.updated.floor = 0;
ePic.updated.exter = 0;


% Construction of the command string
command=[];
sdata=0;

% choices for values asking
if (ePic.update.proxi > 0)   % Proximity
    command=[command -'N'];
    sdata=sdata+16;
end

if (ePic.update.light > 0)   % Light sensors
    command=[command -'O'];
    sdata=sdata+16;
end

if (ePic.update.accel > 0)   % Accelerometer
    command=[command -'a'];
    sdata=sdata+6;
end

if (ePic.update.pos > 0)     % Motor position
    command=[command -'Q'];
    sdata=sdata+4;
end

if (ePic.update.micro > 0)   % Microphones
    command=[command -'u'];
    sdata=sdata+6;
end

if (ePic.update.floor > 0)   % floor sensors
    command=[command -'M'];
    sdata=sdata+6;
end

if (ePic.update.exter > 0 && ePic.param.extSel > 5)
    % external sensor parameters
    extSensorValue = 9999;
    switch ePic.param.extSel
        case {7, 14}   % Gyro, Accelero
            % same as internal accelerometers -> read accelerometer value
            if (ePic.updated.accel == 1)
                ePic.value.exter = ePic.updated.accel;
            else
                ePic.value.exter = zeros(1,3);
                disp 'Error to access external sensor. The selected sensor needs to activate the accelerometer update';
            end
        case {9, 11, 12}   % analog sensors
            extSensorValue = 0;
            sdata=sdata+2;
        case 8                      % i2c compass 
            extSensorValue= -2;
            sdata=sdata+4;
        case {13,15}                % i2c distance sensors
            extSensorValue= -1;
            sdata=sdata+4;
        case 10                      % 5 led IR sensor
            extSensorValue = ePic.param.ledIR(1) *1 + ePic.param.ledIR(2) *2 + ePic.param.ledIR(3)*4 + ePic.param.ledIR(4) *8 + ePic.param.ledIR(5)*16;
            sdata=sdata+2;
    end
    % read external sensor
    if (extSensorValue ~= 9999)
%        c2 = bitshift(extSensorValue,-8); 
%        c1 = extSensorValue - bitshift(c2,8);
        command=[command -'W' typecast(int16(extSensorValue),'int8')];
        
%        flush(ePic);
%        tmp = sprintf('W,%d', extSensorValue);
%        write(ePic, tmp);
%        raw_data = read(ePic);
%        [identifier, ePic.value.exter] = fct_TokString(raw_data);
%        if ( strcmp(upper(identifier),'W') == 0)
%            ePic.value.exter = [0];
%        end
    end
%    ePic.updated.exter = 1;
end


% set motor speed
if (isempty(ePic.set.speed)~=1)          % Set motor speed
    command=[command typecast(int8(-'D'),'int8') typecast(int16(ePic.set.speed(1)),'int8') typecast(int16(ePic.set.speed(2)),'int8')];
    clear ePic.set.speed;
end

% More logical to put it after updating the values.
if (ePic.update.speed > 0)   % Motor speed
    command=[command -'E'];
    sdata=sdata+4;
end

% set led values
for i=1:10
    if (ePic.set.led(i)==1)
        command=[command -'L' i-1 1];
    elseif(ePic.clear.led(i)==1)
        command=[command -'L' i-1 0];
    end
end
ePic.set.led = zeros(1,10); 
ePic.clear.led = zeros(1,10); 

% send the custom command
if (ePic.update.custom > 0)   % Microphones
    command=[command ePic.param.customCommand];
    sdata=sdata+ePic.param.customSize;
end


command=[command 0];

% Asking for the data
flush(ePic);
writeBin(ePic, command);
raw_data = readBinSized(ePic, sdata);

% Converting the data
index=1;

if (size(raw_data,2)>0)
    
    % Proximity sensors
    if (ePic.update.proxi > 0)
        prox = zeros(1,8);
        for i=1:8
            prox(i)=two_complement(raw_data(i*2-1:i*2));
        end
        ePic.value.proxi = filter_Prox(prox);
        ePic.updated.proxi = ePic.updated.proxi + 1;
        index=index+16;
    end
    
    % Light sensors
    if (ePic.update.light > 0)
        lights = zeros(1,8);
        for i=1:8
            lights(i)=two_complement(raw_data(index+i*2-2:index+i*2-1));
        end
        ePic.value.light = filter_Light(lights);
        ePic.updated.light = ePic.updated.light + 1;
        index=index+16;
    end

    % Accelerometers
    if (ePic.update.accel > 0)
        accel = zeros(1,3);
        for i=1:3
            accel(i)=two_complement(raw_data(index+i*2-2:index+i*2-1));
        end
        ePic.value.accel = filter_Accel(accel);
        ePic.updated.accel = ePic.updated.accel + 1;
        index=index+6;
    end

    % Motor position
    if (ePic.update.pos > 0)
        for i=1:2
            ePic.value.pos(i)=two_complement(raw_data(index+i*2-2:index+i*2-1));
        end
        ePic.updated.pos =  ePic.updated.pos + 1;
        index=index+4;
    end

    % Microphone
    if (ePic.update.micro > 0)
        micro = zeros(1,3);
        for i=1:3
            micro(i)=two_complement(raw_data(index+i*2-2:index+i*2-1));
        end
        ePic.value.micro = filter_Micro(micro);
        ePic.updated.micro = ePic.updated.micro + 1;
        index=index+6;
    end

    % Floor sensors
    if (ePic.update.floor > 0)
        floor = zeros(1,3);
        for i=1:3
            floor(i)=two_complement(raw_data(index+i*2-2:index+i*2-1));
        end
        ePic.value.floor = filter_Floor(floor);
        ePic.updated.floor = ePic.updated.floor + 1;
        index=index+6;
    end
    
    % External Sensors Turret
    if (ePic.update.exter > 0 && ePic.param.extSel > 5)
        if (extSensorValue==-2)
            extSensors = zeros(1,2);
            extSensors(1) = two_complement(raw_data(index:index+1));
            extSensors(2) = two_complement(raw_data(index+2:index+3));
            ePic.value.exter = extSensors;
            index=index+4;
        else
            if (extSensorValue==-1)
                extSensors = zeros(1,2);
                extSensors(1) = two_complement(raw_data(index:index+1));
                extSensors(2) = two_complement(raw_data(index+2:index+3));
                ePic.value.exter = extSensors;
                index=index+4;
            else
                ePic.value.exter = two_complement(raw_data(index:index+1));
                index=index+2;
            end
            
        end
        ePic.updated.exter = 1;
    end
        
    % Motor speed
    if (ePic.update.speed > 0)
        for i=1:2
            ePic.value.speed(i)=two_complement(raw_data(index+i*2-2:index+i*2-1));
        end
        ePic.updated.speed = ePic.updated.speed + 1;
        index=index+4;
    end    

    if (ePic.update.custom > 0)
        ePic.value.custom = raw_data(index:index+ePic.param.customSize-1);
        ePic.updated.custom = ePic.updated.custom + 1;
        index = index+ePic.param.customSize;
    end
    
end

% External sensors ------------ use ASCII mode
% if (ePic.update.exter > 0 && ePic.param.extSel > 5)
%     % external sensor parameters
%     extSensorValue = 9999;
%     switch ePic.param.extSel
%         case {6, 13}   % Gyro, Accelero
%             % same as internal accelerometers -> read accelerometer value
%             if (ePic.updated.accel == 1)
%                 ePic.value.exter = ePic.updated.accel;
%             else
%                 ePic.value.exter = zeros(1,3);
%                 disp 'Error to access external sensor. The selected sensor needs to activate the accelerometer update';
%             end
%         case {8, 10, 11}   % analog sensors
%             extSensorValue = 0;
%         case 7                      % i2c compass 
%             extSensorValue= -2;
%         case {12,14}                % i2c distance sensors
%             extSensorValue= -1;
%         case 9                      % 5 led IR sensor
%             extSensorValue = ePic.param.ledIR(1) *1 + ePic.param.ledIR(2) *2 + ePic.param.ledIR(3)*4 + ePic.param.ledIR(4) *8 + ePic.param.ledIR(5)*16;
%     end
%     % read external sensor
%     if (extSensorValue ~= 9999)
%         flush(ePic);
%         tmp = sprintf('W,%d', extSensorValue);
%         write(ePic, tmp);
%         raw_data = read(ePic);
%         [identifier, ePic.value.exter] = fct_TokString(raw_data);
%         if ( strcmp(upper(identifier),'W') == 0)
%             ePic.value.exter = [0];
%         end
%     end
%     ePic.updated.exter = 1;
% end


% Reset update once parameters
if ePic.update.accel == 2
    ePic.update.accel = 0; end;
if ePic.update.proxi == 2
    ePic.update.proxi = 0; end;
if ePic.update.light == 2
    ePic.update.light = 0; end;
if ePic.update.micro == 2
    ePic.update.micro = 0; end;
if ePic.update.speed == 2
    ePic.update.speed = 0; end;
if ePic.update.pos == 2
    ePic.update.pos = 0; end;
if ePic.update.floor == 2
    ePic.update.floor = 0; end;
if ePic.update.exter == 2
    ePic.update.exter = 0; end;
if ePic.update.custom == 2
    ePic.update.custom = 0; end;