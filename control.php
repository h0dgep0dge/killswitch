<?php
session_start();
if(!isset($_SESSION['logged_in']) || $_SESSION['logged_in'] == FALSE) {
	die('You\'re not logged in, click <a href=\'login.php\'>here</a> to log in');
}
?>
<h2>Control panel</h2>
<a href='register.php'>Register</a> a device<br/>
<a href='config.php'>Configure</a> registered devices<br/>
<a href='logout.php'>Log out</a><br/>