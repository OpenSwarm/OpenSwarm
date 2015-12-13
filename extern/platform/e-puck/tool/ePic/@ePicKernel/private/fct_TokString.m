function [identifier, values] = fct_TokString(rawdata)
% function who do the conversion form Raw data to numerical datas

[identifier, rawdatatmp] = strtok(rawdata,',');
values = str2num(rawdatatmp);