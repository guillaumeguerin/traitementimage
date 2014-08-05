function offsets_stats(a, b)
imgA = imread('a.png');
imgB = imread('b.png');
display('PatchMatch');
[ann, annd] = patchmatch(imgA, imgB);
display('Compute Offset');
histo = computeOffset(ann, imgA, imgB);
display('Publishing histogram');
mesh(histo);
end

function [ann, annd] = patchmatch(imgA, imgB)
[hA, wA, zA] = size(imgA);
[hB, wB, zA] = size(imgB);

ann = zeros(hA, wA, 2);
annd = zeros(hB, wB, 'int32');

aew = wA - Constants.PATCH_W;
aeh = hA - Constants.PATCH_W;
bew = wB - Constants.PATCH_W;
beh = hB - Constants.PATCH_W;

for ay = 1:aeh
    for ax = 1:aew
        bx = randi([1, bew]);
        by = randi([1, beh]);
        ann(ay, ax, 1) = bx;
        ann(ay, ax, 2) = by;
        annd(ay, ax) = distance(imgA, imgB, ax, ay, bx, by);
    end
end

for iter = 0:Constants.PM_ITERS-1
    display(iter);
    ystart = 1;
    yend = aeh;
    ychange = 1;
    
    xstart = 1;
    xend = aew;
    xchange = 1;
    
    if mod(iter, 2) == 1
        ystart = yend;
        yend = 1;
        ychange = -1;
        
        xstart = xend;
        xend = 1;
        xchange = -1;
    end
    
    ay = ystart;
    while ay ~= yend
        ax = xstart;
        while ax ~= xend
            xbest = ann(ay, ax, 1);
            ybest = ann(ay, ax, 2);
            dbest = annd(ay, ax);
            
            if ax - xchange < aew
                if ax - xchange > 0
                    xp = ann(ay, ax-xchange, 1) + xchange;
                    yp = ann(ay, ax-xchange, 2);

                    if xp < bew
                        if xp > 0
                            [xbest, ybest, dbest] = improve_guess(imgA, imgB, ax, ay, xbest, ybest, dbest, xp, yp);
                        end
                    end
                end
            end
            
            if ay - ychange < aeh
                if ay - ychange > 0
                    xp = ann(ay - ychange, ax, 1);
                    yp = ann(ay - ychange, ax, 2) + ychange;

                    if yp < beh
                        if yp > 0
                            [xbest, ybest, dbest] = improve_guess(imgA, imgB, ax, ay, xbest, ybest, dbest, xp, yp);
                        end
                    end
                end
            end
            
            rs_start = Constants.RS_MAX;
            if rs_start > max(wB, hB)
                rs_start = max(wB, hB);
            end
            mag = rs_start;
            while mag >= 1
                xmin = max(xbest-mag, 0);
                xmax = min(xbest+mag+1, bew);
                ymin = max(ybest-mag, 0);
                ymax = min(ybest+mag+1, beh);
                xp = xmin + randi([1, xmax-xmin]);
                yp = ymin + randi([1, ymax-ymin]);
               [xbest, ybest, dbest] = improve_guess(imgA, imgB, ax, ay, xbest, ybest, dbest, xp, yp);
                mag = floor(mag / 2);
            end

            ann(ay, ax, 1) = xbest;
            ann(ay, ax, 2) = ybest;
            annd(ay, ax) = dbest;
            ax = ax + xchange;
        end
        ay = ay + ychange;
    end
end
end

function [xbest, ybest, dbest] = improve_guess(a, b, ax, ay, xbest, ybest, dbest, bx, by)
d = distance(a, b, ax, ay, bx, by, dbest);
if d < dbest
    dbest = d;
    xbest = bx;
    ybest = by;
end
end

function res = distance(a, b, ax, ay, bx, by, cutoff)
if nargin < 7
    cutoff = intmax;
end
res = 0;
for dy = 1:Constants.PATCH_W
    arow = a(ay+dy,:,:);
    brow = b(by+dy,:,:);
    for dx = 1:Constants.PATCH_W
        ac = arow(1,ax+dx,:);
        bc = brow(1,bx+dx,:);
        ac = double(ac);
        bc = double(bc);
        dr = ac(1,1,1) - bc(1,1,1);
        dg = ac(1,1,2) - bc(1,1,2);
        db = ac(1,1,3) - bc(1,1,3);
        res = res + dr*dr + dg*dg + db*db;
        res = int32(res);
    end
    if(res >= cutoff)
        res = cutoff;
        return
    end
end
end

function histo = computeOffset(ann, a, b)
[hA, wA, zA] = size(a);
[hB, wB, zB] = size(b);
offset = zeros(hA, wA, 2);
histo = zeros(hA + hB, wA + wB);

aew = wA - Constants.PATCH_W;
aeh = hA - Constants.PATCH_W;

for y = 1:aeh
   for x = 1:aew
       offX = ann(y, x, 1) - x;
       offY = ann(y, x, 2) - y;
       offset(y, x, 1) = offX;
       offset(y, x, 2) = offY;
   end
end

minU = min(min(offset(:,:, 1)));
minV = min(min(offset(:,:, 2)));

for y = 1:aeh
   for x = 1:aew
      u = offset(y, x, 1);
      v = offset(y, x, 2);
      histo(v-minV+1, u-minU+1) = histo(v-minV+1, u-minU+1) +1;
   end
end
end