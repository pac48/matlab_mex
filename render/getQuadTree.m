function [bins, distances] = getQuadTree(allVerts2d) %#codegen

% child1 = 1;
% child2 = 2;
% child3 = 3;
% child4 = 4;
overflowChild = 5;
leftLim = 6;
rightLim = 7;
bottomLim = 8;
topLim = 9;
countInd = 10;
depthInd = 11;
vertsStart = 12;

maxVerts = 50;
depthLimit = 3;
maxTreeSize = 10000;

bin = zeros(vertsStart+maxVerts-1,1);
bin(leftLim) = -1;
bin(rightLim) = 1;
bin(bottomLim) = -1;
bin(topLim) = 1;

bins = zeros(vertsStart+maxVerts-1, maxTreeSize);

% bins = [];
% coder.varsize('bins');

bins(:, 1) = bin;
binsSize = 1;

distances = zeros(4, size(allVerts2d,2)/3);

xMins = min(reshape(allVerts2d(1,:),3,[]));
xMaxes= max(reshape(allVerts2d(1,:),3,[]));
yMins = min(reshape(allVerts2d(2,:),3,[]));
yMaxes = max(reshape(allVerts2d(2,:),3,[]));
mins = cat(1, xMins, yMins);
maxes = cat(1, xMaxes, yMaxes);

coder.varsize('binInds');

canidateBounds = zeros(topLim, 1);

for i = 1:3:size(allVerts2d, 2)
    binInds = 1;
    while ~isempty(binInds)
        binInd = binInds(1);
        binInds = binInds(2:end);
        vertInd = (i-1)/3+1;
        fits = false;
        b = 0;
        if bins(depthInd, binInd) < depthLimit
            for b = 1:4
                canidateBounds(leftLim:topLim) = bins(leftLim:topLim, binInd);
                switch b
                    case 1
                        canidateBounds(rightLim) = canidateBounds(rightLim) - (canidateBounds(rightLim)- canidateBounds(leftLim))/2;
                        canidateBounds(bottomLim) = canidateBounds(bottomLim) + (canidateBounds(topLim) - canidateBounds(bottomLim))/2;
                    case 2
                        canidateBounds(leftLim) = canidateBounds(leftLim) + (canidateBounds(rightLim)- canidateBounds(leftLim))/2;
                        canidateBounds(bottomLim) = canidateBounds(bottomLim) + (canidateBounds(topLim) - canidateBounds(bottomLim))/2;
                    case 3
                        canidateBounds(rightLim) = canidateBounds(rightLim) - (canidateBounds(rightLim)- canidateBounds(leftLim))/2;
                        canidateBounds(topLim) = canidateBounds(topLim) - (canidateBounds(topLim) - canidateBounds(bottomLim))/2;
                    case 4
                        canidateBounds(leftLim) = canidateBounds(leftLim) + (canidateBounds(rightLim)- canidateBounds(leftLim))/2;
                        canidateBounds(topLim) = canidateBounds(topLim) - (canidateBounds(topLim) - canidateBounds(bottomLim))/2;
                end
                fits = ~((mins(1,vertInd) < canidateBounds(leftLim) || mins(2,vertInd)  < canidateBounds(bottomLim)) || (maxes(1,vertInd) > canidateBounds(rightLim) || maxes(2,vertInd) > canidateBounds(topLim)));
                if fits
                    break
                end
            end
        end
        if fits % create bin and add to stack
            if bins(b, binInd) ~= 0 % child already exists
                childBinInd = bins(b, binInd);
                binInds = cat(2, binInds, childBinInd);
            else % create new child
                newBin = zeros(vertsStart+maxVerts-1, 1);
                newBin(leftLim:topLim) = canidateBounds(leftLim:topLim);
                newBin(depthInd) = bins(depthInd, binInd)+1;
%                 bins = cat(2, bins, newBin);
                binsSize = binsSize+1;
                bins(:, binsSize) = newBin;
                newBinInd = binsSize;%size(bins, 2);
                bins(b, binInd) = newBinInd;
                binInds = cat(2, binInds, newBinInd);
            end
        else
            % should we insert the triangle?
            cond = mins(1,vertInd) > bins(rightLim, 1) || maxes(1,vertInd) < bins(leftLim, 1) ...
                || mins(2, vertInd) > bins(topLim, 1) || maxes(2,vertInd) < bins(bottomLim, 1);
            if ~cond
                count = bins(countInd, binInd);
                while count >= maxVerts
                    if bins(overflowChild, binInd) == 0 % add new noverflow bin
                        newBin = zeros(vertsStart+maxVerts-1, 1);
                        newBin(leftLim:topLim) = bins(leftLim:topLim, binInd);
%                         bins = cat(2, bins, newBin);
                         binsSize = binsSize+1;
                         bins(:, binsSize) = newBin;
                        bins(overflowChild, binInd) = binsSize;
                    end

                    binInd = bins(overflowChild, binInd);
                    count = bins(countInd, binInd);
                end
                % insert vert
                bins(count+vertsStart, binInd) = vertInd;
                bins(countInd, binInd) = count+1;

                midX = bins(leftLim, binInd)+(bins(rightLim, binInd) - bins(leftLim, binInd))/2;
                midY = bins(bottomLim, binInd)+(bins(topLim, binInd) - bins(bottomLim, binInd))/2;

                vertSplit = (mins(1, vertInd) < midX) && (maxes(1, vertInd) > midX); %split vert
                horzSplit = (mins(2, vertInd) < midY) && (maxes(2, vertInd) > midY); %split horiontal

                distances(1, vertInd) = (midX-mins(1, vertInd));
                distances(2, vertInd) = (maxes(1, vertInd)-midX);
                distances(3, vertInd) = (midY-mins(2, vertInd));
                distances(4, vertInd) = (maxes(2, vertInd)-midY);
                if ~vertSplit && ~horzSplit % depth limit must have been reached
                    distances(:, vertInd) = max(distances(:, vertInd),0);
                else
                    distances(1, vertInd) = vertSplit*distances(1, vertInd);
                    distances(2, vertInd) = vertSplit*distances(2, vertInd);
                    distances(3, vertInd) = horzSplit*distances(3, vertInd);
                    distances(4, vertInd) = horzSplit*distances(4, vertInd);
                end

            end
        end

    end

end

bins = bins(:,1:binsSize);