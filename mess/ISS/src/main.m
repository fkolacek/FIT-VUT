% ISS projekt 2
% xkolac12 <xkolac12@stud.fit.vutbr.cz>

% disable Intel Integrated Performance Primitives
iptsetpref('UseIPPL',false);

% pomocne promenne
noise = 0;
matrix1 = [-0.5 -0.5 -0.5; -0.5 5.0 -0.5; -0.5 -0.5 -0.5];
matrix2 = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1];
matrix2 = matrix2 / 49;
N = 2;
l = 0;
m = 255;

% nacteni obrazku
fileHandler0 = imread('xkolac12.bmp');
 
% Ukol 1 - ostreni
fileHandler1 = imfilter(fileHandler0, matrix1);
imwrite(fileHandler1, 'step1.bmp');
 
% Ukol 2 - preklopeni
fileHandler2 = fliplr(fileHandler1);
imwrite(fileHandler2, 'step2.bmp');
 
% Ukol 3 - median
fileHandler3 = medfilt2(fileHandler2, [5 5]);
imwrite(fileHandler3, 'step3.bmp');
 
% Ukol 4 - rozmazani
fileHandler4 = imfilter(fileHandler3, matrix2);
imwrite(fileHandler4, 'step4.bmp');
 
% Ukol 5 - sum v obraze
imErrAux = fliplr(fileHandler0);
imTmp = double(imErrAux);
imNew = double(fileHandler4);

for (x=1:512)
    for (y=1:512)
        noise=noise+(abs(imTmp(x,y) - imNew(x,y)));
    end;
end;

noise = noise/(512*512)
 
% Roztazeni histogramu
fileHandler4 = im2double(fileHandler4);
minimum = min(min(im2double(fileHandler4)));
maximum = max(max(im2double(fileHandler4)));

fileHandler5 = imadjust(fileHandler4, [minimum maximum], [0 1]);
fileHandler4 = im2uint8(fileHandler4);
fileHandler5 = im2uint8(fileHandler5);
imwrite(fileHandler5, 'step5.bmp');
 
% Vysledne hodnoty
fileHandler5 = double(fileHandler5);
fileHandler4 = double(fileHandler4);

mean_hist = mean2(im2double(fileHandler5))
std_hist = std2(im2double(fileHandler5))
mean_no_hist = mean2(im2double(fileHandler4))
std_no_hist = std2(im2double(fileHandler4))

% Kvantizace
p = size(fileHandler5);

for (j = 1:p(1))
    for (k = 1:p(2))
        fileHandler6(j,k) = round(((2^N)-1)*(fileHandler5(j,k)-l)/(m-l))*(m-l)/((2^N)-1) + l;
    end
end
fileHandler6 = uint8(fileHandler6);
imwrite(fileHandler6, 'step6.bmp'); 


