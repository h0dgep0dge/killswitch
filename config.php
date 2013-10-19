<?php
// <form method='post'>
	// <table>
		// <tr><td>Name</td><td>Allow / Deny</td></tr>
		// <tr><td>Robbie</td><td><input type='radio' name='dev[0]' value='1' />/<input type='radio' name='dev[0]' value='0' /></td></tr>
		// <tr><td>Default</td><td><input type='radio' name='dev[1]' value='1' />/<input type='radio' name='dev[1]' value='0' /></td></tr>
	// </table>
	// <input type='submit' value='Apply' />
// </form>
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
session_start();
if(!isset($_SESSION['logged_in']) || $_SESSION['logged_in'] == FALSE) {
	die('You\'re not logged in');
}
if(!isset($_SESSION['uid'])) {
	die('Server error occured');
}

if(isset($_POST['dev']) && isset($_POST['def'])) {
	print_r($_POST);
}
else if(isset($_POST['dev']) || isset($_POST['def'])) {
	die('Server error');
}
$sql = new mysqli('127.0.0.1',$username,$password,'killswitch');

$uid = strval($_SESSION['uid']);
// No input sanitation, yet.

// echo '<form action=\'\' method=\'post\'>';
$res = $sql->query('SELECT `owner` FROM `users` WHERE `uid`=\''.$uid.'\'');
if($res->num_rows <= 0) die('User not found');
if($res->num_rows > 1) die('Server error');
$row = $res->fetch_row();
$owner = $row[0];
//Sanitize.

$def = 0;
$def_id = -1;
$res = $sql->query('SELECT * FROM `policies` WHERE `owner`=\''.$owner.'\'');
if($res->num_rows < 0) die('Server error');
if($res->num_rows == 0) echo 'No devices registered';
echo '<form method=\'post\'>';
echo '<table>';
echo '<tr><td>Name</td><td>Allow / Deny</td></tr>';
for($i = 0;$i < $res->num_rows;$i++) {
	$row = $res->fetch_row();
	if($row[2] == "00:00:00:00:00:00") {
		$def = intval($row[4]);
		continue;
	}
	echo '<tr><td>'.$row[3].'</td><td><input type=\'radio\' name=\'dev['.$row[0].']\' value=\'1\' '.(intval($row[4])==1?'checked':'').' />/<input type=\'radio\' name=\'dev['.$row[0].']\' value=\'0\' '.(intval($row[4])==0?'checked':'').' /></td></tr>';
}
echo '<tr><td>Default</td><td><input type=\'radio\' name=\'def\' value=\'1\' '.(intval($def)==1?'checked':'').' />/<input type=\'radio\' name=\'def\' value=\'0\' '.(intval($def)==0?'checked':'').' /></td></tr>';
echo '</table>';
echo '<input type=\'submit\' value=\'Apply\' />';
echo '</form>';
?>
