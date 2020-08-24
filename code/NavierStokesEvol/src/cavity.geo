// mesh width associated with points
lc = 0.1;

L = 1;
l = 1.0/7.0 * L;


Point(1) = { 0.0 , 0.0 , 0.0 , lc };
Point(2) = { l   , 0.0 , 0.0 , lc };
Point(3) = { l   , -l  , 0.0 , lc };
Point(4) = { 2*l , -l  , 0.0 , lc };
Point(5) = { 2*l , 0.0 , 0.0 , lc };
Point(6) = { L   , 0.0 , 0.0 , lc };
Point(7) = { L   , 5*l , 0.0 , lc };
Point(8) = { L+l , 5*l , 0.0 , lc };
Point(9) = { L+l , 6*l , 0.0 , lc };
Point(10) = { L   , 6*l , 0.0 , lc };
Point(11) = { L   , L   , 0.0 , lc };
Point(12) = { 0.0 , L   , 0.0 , lc };

Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,5};
Line(5) = {5,6};
Line(6) = {6,7};
Line(7) = {7,8};
Line(8) = {8,9};
Line(9) = {9,10};
Line(10) = {10,11};
Line(11) = {11,12};
Line(12) = {12,1};




Line Loop(100) = {1,2,3,4,5,6,7,8,9,10,11,12};  
Plane Surface(200) = {100};  
