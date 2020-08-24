// mesh width associated with points
lc = 0.1;


//sve u centimetrima, tj. cgs sustav
D = 1.2;
d = 0.4;
tan10 = 0.1763269807;
T = 4;

dx = (D-d)*0.5/tan10;

Point(1) ={ 0 , D/2 , 0 , lc };
Point(2) ={ 5*D , D/2 , 0 , lc };
Point(3) ={ 5*D+dx , d/2 , 0 , lc };
Point(4) ={ 5*D+dx+T , d/2 , 0 , lc };
Point(5) ={ 5*D+dx+T , D/2 , 0 , lc };
Point(6) ={ 5*D+dx+T+5*D , D/2 , 0 , lc };

Point(7) ={ 5*D+dx+T+5*D , -D/2 , 0 , lc };
Point(8) ={ 5*D+dx+T , -D/2 , 0 , lc };
Point(9) ={ 5*D+dx+T , -d/2 , 0 , lc };
Point(10) ={ 5*D+dx , -d/2 , 0 , lc };
Point(11) ={ 5*D , -D/2 , 0 , lc };
Point(12) ={ 0 , -D/2 , 0 , lc };

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
//+
Extrude {{0, 1, 0}, {0, 0, 0}, 2*Pi} {
  Surface{200}; 
}
//+
Extrude {{0, 1, 0}, {0, 0, 0}, 2*Pi} {
  Surface{200}; 
}
//+
Extrude {{0, 1, 0}, {0, 0, 0}, 2*Pi} {
  Surface{200}; 
}
//+
Extrude {{0, 1, 0}, {0, 0, 0}, 2*Pi} {
  Surface{200}; 
}
