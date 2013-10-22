<?php
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
session_start();
if(isset($_SESSION['logged_in']) && $_SESSION['logged_in']) {
	die('You\'re already logged in');
}
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
$sql = new mysqli('127.0.0.1',$username,$password,'killswitch'); // Add error detection
if ($mysqli->connect_error) {
	die('Connect Error '.$mysqli->connect_error);
}
$username = $sql->real_escape_string($_POST['username']);
$password = $sql->real_escape_string($_POST['password']);

$res = $sql->query('SELECT * FROM `users` WHERE `username`=\''.$username.'\''); // Add error detection
if($res->num_rows <= 0) {
	die('Username or password incorrect');
}
$row = $res->fetch_row();
if($row[2] != sha1($password)) die('Username or password incorrect');
// Authenticated
$_SESSION['logged_in'] = TRUE;
$_SESSION['uid'] = intval($row[0]);
echo 'You\'re now logged in';
?>
