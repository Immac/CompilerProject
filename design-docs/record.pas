<html>
	<%
		 type TMember = record 
		        firstname, lastname : string;
		        address: array [1 .. 3] of string;
		        phone: string;
		        birthdate: TDateTime;
				moriya: (kanako,suwako,sanae);
		        paidCurrentSubscription: boolean;
	        end;
		
	%>
</html>