function [data] = EpuckSendData(data, dataType)
global EpuckPort;

size = length(data);

if(strcmp(dataType,'char'))
    int8(data);
    fwrite(EpuckPort,size,'uint16');
    fwrite(EpuckPort,data,'int8');
else
    int16(data);
    fwrite(EpuckPort,2*size,'uint16');
    fwrite(EpuckPort,data,'int16');
end
return;

