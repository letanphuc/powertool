<?php
include "database/member.php";
?>


<?php
if ($_GET ['username'] || $_GET ['password']) {
	if (login ( $_GET ['username'], $_GET ['password'] )) {
		header ( 'Location: ' . "/test.php" );
		die ();
	} else {
		echo "Good bye";
	}
	
	exit ();
}
?>



<!DOCTYPE html>
<html>
<body>
	<h1>My First Heading</h1>
	<p>My first paragraph.</p>
	<form action="<?php $_PHP_SELF ?>" method="GET">
		User name:<br> <input type="text" name="username"> <br> Password:<br>
		<input type="text" name="password"> <br> <input type="submit"
			value="Submit"> <br>

	</form>
</body>
</html>



