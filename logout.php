<?php
session_start();
if(!isset($_SESSION['logged_in']) || $_SESSION['logged_in'] == FALSE) {
	die('You\'re not logged in, click <a href=\'login.php\'>here</a> to log in');
}
$_SESSION['logged_in'] = FALSE;
unset($_SESSION['logged_in']);
session_destroy();
echo 'You\'ve been logged out'
?>