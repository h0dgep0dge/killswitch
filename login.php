<?php
if(!isset($_POST['username']) && !isset($_POST['password'])) {
?>
<form method='post'>
	Username: <input type='text' name='username' /><br />
	Password: <input type='password' name='password' /><br />
	<input type='submit' value='Login' />
</form>
<?php
die();
}
else if(!isset($_POST['username']) || !isset($_POST['password'])) {
	die('A server error has occured');
}
$username = $_POST['username'];
$password = $_POST['password'];
// Need input sanitation!

$res = $sql->query('SELECT * FROM `users` WHERE `username`=\''.$username.'\''); // Add error detection
if($res->num_rows <= 0) {
	die('Username or password incorrect');
}
$row = $res->fetch_row();
	$policy = $row[0];

?>