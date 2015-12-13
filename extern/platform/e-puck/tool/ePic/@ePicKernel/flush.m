function [result] = flush(ePic)
%   ser_flush Flush the input and output buffer of Matlab
%   This function is useful each time you do a send / receive cylce with
%   the e-puck. In fact, Matlab probably send old data if you don't do this
%   flush, so the e-puck could crash...
%   
%   [result] = flush(ePic)
%
% Results :
%   result          :   flush result (1:ok, 0:error)
%
% Parameters :
%   ePic            :   ePicKernel object


try
    flushinput(ePic.param.comPort);
    flushoutput(ePic.param.comPort);
    result = 1;
catch
    disp 'Erreur while serial port flushing';
    result = 0;
end