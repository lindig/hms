.load ./hms
.mode csv
.import "test-1.csv" test
.mode table
select 
  cast(x as text) as x,
  y,
  hms(x) as h,
  cast (round(duration(y),3) as text) as d
from test
where
  x != d or y != h
;
