function [ePic, mode, sizexy] = updateImage(ePic)
% This function will ask for the e-puck to get a image 
% and store it in the image field
%   
% use the methode '[ePic] = updateDef(ePic, 'image', value)' to enable
% image update
%
% [ePic, mode, sizexy] = updateImage(ePic)
%
% Results :
%   ePic            :   updated ePicKernel object
%   mode            :   return image mode
%   sizexy          :   reutnr image size
%
% Parameters :
%   ePic            :   ePicKernel object

persistent perImgMode;
persistent perImgSize;
persistent perImgZoom;

% clear updated flag
ePic.updated.image = 0;

% verify if image update is enable
if (ePic.update.image == 0)
    return;
end

% set camera mode if the camera has never been initialized or if the
% parameters has changed
if (ePic.param.imgMod == 1)
    ePic.param.imgMod = 0;
    
    perImgMode = ePic.param.imgMode;
    perImgSize = ePic.param.imgSize;
    perImgZoom = ePic.param.imgZoom;
    
    % Set camera parameters using ASCII mode
    tmp = sprintf('J,%d,%d,%d,%d', perImgMode, perImgSize(1), perImgSize(2), perImgZoom);
    write(ePic,tmp);
    pause(0.05);
    read(ePic);
end

% ask for camera datas
flush(ePic);
writeBin(ePic, [-'I' 0]);
pause(0.1);

ePic.value.image= zeros(perImgSize(2),perImgSize(1),3);

mode=0;
sizexy=[0 0];

% read the datas
dataimg=readBinSized(ePic,3);
if (numel(dataimg)==3)
    mode=dataimg(1);
    sizexy = [dataimg(2), dataimg(3)];
    size_to_receive = dataimg(2)*dataimg(3);
    if (mode==1)
        size_to_receive=size_to_receive*2;
    end

    imgtmp=readBinSized(ePic,size_to_receive);
    if (numel(imgtmp)==size_to_receive)
        % process the datas
        if (mode == 0)
            % gray scale image
            index = 1;
            for line=1:sizexy(1)
                for col=0:sizexy(2)-1
                    if index <= size(imgtmp,1)
                        % Get pixel value
                        Pix = uint8(imgtmp(index));
                        % Set same color for R,G and B -> gray
                        ePic.value.image(sizexy(2)-col,line,1) = double(Pix)/255.;
                        ePic.value.image(sizexy(2)-col,line,2) = double(Pix)/255.;
                        ePic.value.image(sizexy(2)-col,line,3) = double(Pix)/255.;
                    end
                    % each pixel is coded with 1 byte
                    index = index +1;
                end
            end
        else
            % color ePic.value.image
            index = 1;
            for line=1:sizexy(1)
                for col=0:sizexy(2)-1
                    if index <= size(imgtmp,1)
                        % Get RGB 565 value
                        Pix_1 = uint8(imgtmp(index));
                        Pix_2 = uint8(imgtmp(index+1));
                        % Get values for R, G and B
                        Red_pix =  bitand(Pix_1,248);
                        Green_pix = bitshift(bitand(Pix_1,7),5)+ bitshift(bitand(Pix_2,224),-3);
                        Blue_pix = bitshift(bitand(Pix_2,31),3);
                        % Set color to the ePic.value.image matrix
                        ePic.value.image(sizexy(2)-col,line,1) = double(Red_pix)/255.;
                        ePic.value.image(sizexy(2)-col,line,2) = double(Green_pix)/255.;
                        ePic.value.image(sizexy(2)-col,line,3) = double(Blue_pix)/255.;
                    end
                    % each pixel is coded with 2 bytes
                    index = index +2;
                end
            end
        end
    end
end    
% set updated flag
ePic.updated.image = 1;
% change update flag is refresh is not set to 1
if (ePic.update.image ~= 1)
    ePic.update.image = 0;
end