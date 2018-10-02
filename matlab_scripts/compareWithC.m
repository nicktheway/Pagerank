%% --- What is this script ---
% This script was made to make sure that the results the C
% programs produce are correct.
%
% How to use:
% - Choose the path of the WGFile that was used as input to the C program
% - Choose the path of the binary output file of the C program for that input.
% - Choose the c, e constant values.
% - Run the script.
%
% After this script is finished, there will be some useful variables at your
% workspace, including:
% - SPM      : The web graph sparse matrix.
% - sizes    : The number of nodes and edges in the web graph matrix.
% - mPRresult: The pagerank vector of the WGFile as calculated in MATLAB.
% - cPRresult: The pagerank vector of the WGFile from the C calculation.
% - percError: A vector containing the relative difference of each element
%               of mPRresult, cPRresult in relation to mPRresult.
%               Multiply by 100% to have the percentage error.
%
% Author: Katomeris Nikolaos
% Last change at: October 2nd, 2018.

%% Web graph input file's path.
% Be careful, selecting a large dataset as input will take
% a really long time to be computed with MATLAB/octave.
dataset = "input_test"; % Other example: "web-NotreDame"
inputWGFilePath = ["../data/WGFiles/" dataset ".wg"];
pagerankOutputFilePath = ["../results/serial/pageranks/" dataset ".data"];

%% Teleportations constant and error.
c = 0.15;
e = 1e-6;

%% There is no need to modify anything bellow this line.
%-------------------------------------------------------%

%% Read input and store it into a sparse matrix.
wgFile = fopen(inputWGFilePath);
sizes = fread(wgFile, [1,2], 'uint32')';
N = sizes(1); % Nodes and edges, N = nodes.
A = fread(wgFile, [2,inf], 'uint32')';
fclose(wgFile);
Sp = A(1:end, :) + 1;
SPM = sparse(Sp(:,1), Sp(:,2), ones(size(Sp, 1), 1),N, N);

%% Read the result of the C program.
prFile = fopen(pagerankOutputFilePath);
cPRresult = fread(prFile, "double");
fclose(prFile);

%% Calculate the result with the MATLAB program.
mPRresult = pr_mod_gauss_seidel(SPM, 1-c, e);

%% Calculate the error
percError = (cPRresult-mPRresult) ./ mPRresult;

%% Clear unnecessary variables.
clear A Sp wgFile prFile inputWGFilePath pagerankOutputFilePath dataset