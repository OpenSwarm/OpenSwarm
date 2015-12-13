function [SerRawData] = read(ePic)
%   read allow you to read a line of datas form
%   the buffer. Datas are in raw format
%   
%   [SerRawData] = read(ePic)
%
% Results :
%   SerRawData      :   recieved data in raw format
%
% Parameters :
%   ePic            :   ePicKernel object

try
    SerRawData = fscanf(ePic.param.comPort);
catch
    disp 'Erreur while data reading'
end