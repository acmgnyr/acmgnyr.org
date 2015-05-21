program logic;

var
   dat, out : text;
   circuit : array [ 0..101, 0..101 ] of char;
   rstart, cstart : integer;
   input : array [ 'A'..'Z' ] of boolean;

(* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *)

procedure loadcircuit;

   var
      s : string;
      r, c : integer;

   begin
   for r := 0 to 101 do for c := 0 to 101 do circuit[ r, c ] := ' ';
   r := 0;
   readln( dat, s);
   while s <> '*' do
      begin
      inc( r );
      for c := 1 to length( s ) do
         begin
         circuit[ r, c ] := s[ c ];
         if s[ c ] = '?' then begin rstart := r; cstart := c end
         end;
      readln( dat, s )
      end
   end;

(* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *)

procedure updatedirection( r, c : integer; var dr, dc : integer );

   begin
   if circuit[ r, c ] = ':' then
      begin dr := 0; dc := -1 end
   else if (circuit[ r, c - 1 ] = '-') and (dc = 0) then
      begin dr := 0; dc := -1 end
   else if (circuit[ r, c + 1 ] = '-') and (dc = 0) then
      begin dr := 0; dc := 1 end
   else if (circuit[ r - 1, c ] = '|') and (dr = 0) then
      begin dr := -1; dc := 0 end
   else if (circuit[ r + 1, c ] = '|') and (dr = 0) then
      begin dr := 1; dc := 0 end
   end;

(* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *)

procedure followpath( var r, c : integer; var v : char );

   var
      dr, dc : integer;

   begin
   v := circuit[ r, c ];
   dr := 0;
   if v = '-' then dc := -1 else dc := 0;
   while not (v in [ 'A'..'Z', 'o', ')', '>' ]) do
      begin
      if v in [ '+', '?', ':' ] then updatedirection( r, c, dr, dc );
      r := r + dr;
      c := c + dc;
      v := circuit[ r, c ]
      end;
   end; { followpath }

(* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *)

function outval( r, c : integer ) : boolean;

   var
      v : char;

   begin
   followpath( r, c, v );
   case v of
      'A'..'Z' : outval := input[ v ];
      'o' : outval := not outval( r, c - 1 );
      ')' : outval := outval( r - 1, c - 2 ) and outval( r + 1, c - 2 );
      '>' : outval := outval( r - 1, c - 2 ) or outval( r + 1, c - 2 )
      end
   end;

(* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *)

procedure computeoutputs;

   var
      s : string;
      c : char;
      i : integer;

   begin
   readln( dat, s );
   while s <> '*' do
      begin
      i := 0;
      for c := 'A' to 'Z' do
         begin
         inc( i );
         input[ c ] := s[ i ] = '1';
         end;
      if outval( rstart, cstart ) then writeln( out, 1 ) else writeln( out, 0 );
      readln( dat, s )
      end;
   writeln( out )
   end;

(* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *)

begin
assign( dat, 'logic.in' );
reset( dat );
assign( out, 'logic.out' );
rewrite( out );
while not eof( dat ) do
   begin
   loadcircuit;
   computeoutputs
   end;
close( dat );
close( out )
end.
