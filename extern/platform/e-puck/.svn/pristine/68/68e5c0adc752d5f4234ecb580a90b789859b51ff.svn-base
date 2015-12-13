function [image2] = filter_image2(image)
% Function called before displaying image 2

% Sobel border detection filter example
image_tmp = edge( rgb2gray(image), 'sobel' );
image2(:,:,1) = image_tmp;               % needed to get a B&W image 
image2(:,:,2) = image_tmp;
image2(:,:,3) = image_tmp;