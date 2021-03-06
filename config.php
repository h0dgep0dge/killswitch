<?php
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
session_start();
if(!isset($_SESSION['logged_in']) || $_SESSION['logged_in'] == FALSE) { // Check if user is logged in
	die('You\'re not logged in');
}
if(!isset($_SESSION['uid'])) { // Check for missing user id, though ostensibly logged in
	die('Server error occured');
}


$sql = new mysqli('127.0.0.1',$username,$password,'killswitch');	// Creates an sqli object
if($sql->connect_error) die('Connect Error '.$sql->connect_error);	// Check for errors

$uid = strval($_SESSION['uid']); // Fetch user id from serssion

if(isset($_POST['dev']) != isset($_POST['def'])) { // Check if the form was posted, but there's a variable missing
	die('Server error');
}

$res = $sql->query('SELECT `owner` FROM `users` WHERE `uid`=\''.$uid.'\''); // What's my owner id?
if($res->num_rows <= 0) die('User not found');
if($sql->error) die('Select Error '.$sql->error);
$row = $res->fetch_row();
$owner = $sql->real_escape_string($row[0]);

if(isset($_POST['dev']) && isset($_POST['def'])) { // Checks if there's entries to update
	$def = $sql->real_escape_string($_POST['def']); // New default
	
	// Iterate over devices to update and update them
	foreach($_POST['dev'] as $key => $val) {
		$res = $sql->query('UPDATE `policies` SET `policy`=\''.$val.'\' WHERE `id`=\''.$key.'\' AND `owner`=\''.$owner.'\'');
		if($sql->error) die('Update error'.$sql->error);
	}
	
	// Create or update the deafult policy
	$res = $sql->query('SELECT `id` FROM `policies` WHERE `owner`=\''.$owner.'\' AND `addr`=\'00:00:00:00:00:00\'');
	if($sql->error) die('Select Error '.$sql->error);
	else if($res->num_rows > 0) {
		$row = $res->fetch_row();
		$res = $sql->query('UPDATE `policies` SET `policy`=\''.$def.'\' WHERE `id`=\''.$row[0].'\' AND `owner`=\''.$owner.'\'');
		if($sql->error) die('Select Error '.$sql->error);
	}
	else {
		$res = $sql->query('INSERT INTO `policies` VALUES (NULL, \''.$owner.'\', \'00:00:00:00:00:00\', \'DEFAULT\', \''.$def.'\')');
		if($sql->error) die('Select Error '.$sql->error);
	}
}

$def = 0;
$def_id = -1;
$res = $sql->query('SELECT * FROM `policies` WHERE `owner`=\''.$owner.'\''); // Get all current policies and display in a nice table
if($sql->error) die('Select Error '.$sql->error);
echo '<form method=\'post\'>';
echo '<table>';
echo '<tr><td>Name</td><td>Allow / Deny</td></tr>';
if($res->num_rows == 0) echo '<tr><td>No devices registered</td><td></td></tr>';
else for($i = 0;$i < $res->num_rows;$i++) { // Iterate over current policies
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
echo '<a href=\'control.php\'>Go back</a> to control panel';
?>
