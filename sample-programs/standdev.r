
array int tee[5] ; 

var int i ;

var int m  ; 
i := 0 ; 

m := 4 ; 

while  i < 5 
  do
{
   tee[i] := m  ; 
   m := m + 1 ;      

}

printfloat(ShowStdTable(tee , 5)) ; 

