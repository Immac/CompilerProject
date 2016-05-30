<html>
	<%(*test*)
           if (a = 10)  then
                writeln('Value of a is 10' );
            else if ( a = 20 ) then
                writeln('Value of a is 20' );
            else if( a = 30 ) then 
                writeln('Value of a is 30' );
            else
                writeln('None of the values is matching' );
			writeln('Exact value of a is: ', a );
            type TMember = record 
		        firstname, lastname : string;
		        address: array [1 .. 3] of string;
		        phone: string;
		        birthdate: TDateTime;
		        paidCurrentSubscription: boolean;
	        end;

               CONST
                    (* Max array size. *)
                   MaxElts = 50;              
				   TYPE 
                    (* Type of the element array. *)
                   IntArrType = ARRAY [1 .. MaxElts] OF Integer;

                type dias = (lunes, martes, miercoles, jueves, viermes, sabado, domingo);


            type  arr1 = array [1 .. 3,1 .. 3] of integer; 
            var a1, a2, a3 : arr1;                                                                     
            var i, j : integer;
            procedure initialise1(var a1 : arr1); 
            begin 
                for i:=1 to 3 do 
                    begin 
                    for j:=1 to 3 do 
                    begin 
                        {a1[i][j]:=random(10);}
						a1 := 0;
                        {%> <html> <% }
                    end;
                end; 
            end; 
			%> <html> <%
           CONST MaxElts = 50;
            const FiveFoo      = 5;
            const StringFoo    = 'string constant';
            const AlphabetSize = Ord ('Z') - Ord ('A') + 1;
            const i: Integer = 0;

            a := 100;
           (* check the boolean condition *)
           if( a < 20 ) then
              (* if condition is true then print the following *)
              writeln('a is less than 20' );
   
           else
              (* if condition is false then print the following *) 
              writeln('a is not less than 20' );
              writeln('value of a is : ', a);

            procedure output2(var a2 : arr1); 
            begin 
               (* PROCEDURE ReadArr(VAR size: Integer; VAR a: IntArrType);
                BEGIN
                    size := 1;
                    WHILE NOT eof DO BEGIN
                        {}
                        IF NOT eof <> false THEN 
                            size := size + 1;
                    END;
                END;*)
				PROCEDURE ReadArr(VAR size: Integer, VAR a: IntArrType);
                BEGIN
                    size := 1;
                    WHILE NOT eof DO BEGIN
                        {}
                        IF NOT eof <> false THEN 
                            size := size + 1;
                    END;
                END;
                for i:=1 to 3 do 
                begin 
                    for j:=1 to 3 do 
                    begin 
                        write(a2,s); 
                    end; 
                    {writeln(); }
                end; 
            end;

             case place of
               1: ShowMessage('sds');
               2: ShowMessage(sdds);
               3: ShowMessage(sd.test); 
               else ShowMessage(sdsd); 
             end;

            while a + 6 do
            begin
              writeln (a);
              a := a + 1;
              repeat
                 DoSomethingHere(x);
                 x := x + 1;
                 while a + 6 do
                  writeln (a);
               until x = 10;
            end;

            (* function returning the max between two numbers *)
            function max(var num1, num2: integer): integer;
            begin
             var result: integer = 3;
               (* local variable declaration *)
               if (num1 > num2) then
                  result := num1;
               else
                  result := num2; }

               for i:= 1 to 10 do writeln(i); 

               for Color in TColor do
                DoSomething(Color); }

               for a := 10  to 20 do
               begin
                  writeln('value of a: ', a);
                  case place of
                     1: begin ShowMessage('sds');
                        ShowMessage('sds');
                        ShowMessage('sds'); end;
                     2: ShowMessage(sdds); }
                     3+expureichion(arr[expureichion(arr[4].algo[4][4].dd)]): ShowMessage(sd.test); 
                    else ShowMessage(sdsd); 
                   end;
               end;
               max := result;
            end;
			<html>
	<% 
		Sum := Num1 + Num2; 
		Write('Hello World!'); 
		Readln(); 
	%> 
		text 
	<% 
		Var name, surname: String; 
	%>
</html>
	 %> 
 </html>