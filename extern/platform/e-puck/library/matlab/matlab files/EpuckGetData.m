function [data] = EpuckGetData()
global EpuckPort;
i = 0;

while (i ~= 5)
    c = fread(EpuckPort,1,'char');
    switch i
        case 0
            if(c ~= 'E')
                continue;
            else
                i = 1;
            end
        case 1
            if(c ~= 'O')
                i = 0;
                continue;
            else
                i = 2;
            end
        case 2
            if(c ~= 'F')
                i = 0;
                continue;
            else
                i = 3;
            end
        case 3
            if(c ~= 0)
                i = 0;
                continue;
            else
                i = 4;
            end
        case 4
            if(c == 'I')
                i = 5;
                receivedFormat = c;
            elseif(c == 'C')
                i = 5;
                receivedFormat = c;
            else
                i = 0;
                continue;
            end
    end
end

if(receivedFormat == 'I')  
    size = fread(EpuckPort,1,'uint16');
    data = fread(EpuckPort,size/2,'int16');
elseif(receivedFormat == 'C')
    size = fread(EpuckPort,1,'uint16');
    data = fread(EpuckPort,size,'int8');
end
return;

