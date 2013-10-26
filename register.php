<?php
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
if(!isset($_POST['addr']) && !isset($_POST['name']) && !isset($_POST['owner'])) {
	header('Location: ./mac.html'); // If the form hasn't been submitted, redirect to the form
	die('Redirecting.');
}
if(!isset($_POST['addr']) || !isset($_POST['name']) || !isset($_POST['owner'])) die('Server error'); // There are missing variables
if($_POST['addr'] == "00:00:00:00:00:00") die('Invalid mac address'); // I don't know why you'd get this, but it's already being used for the default
if(preg_match('/\A(?:[0-9a-fA-F]{2}:?){6}\z/',$_POST['addr']) <= 0) die('Invalid mac address'); // Mac address is mis-formatted
if(preg_match('/\A[0-9a-zA-Z \'",]{1,256}\z/m',$_POST['name']) <= 0) die('Name with bad characters'); // Alias can only contain certain characters

$sql = new mysqli('127.0.0.1',$username,$password,'killswitch'); // Create sqli object
if($sql->connect_error) die('Connect Error '.$sql->connect_error);

$addr = $sql->real_escape_string($_POST['addr']);
$owner = $sql->real_escape_string($_POST['owner']);
$name = $sql->real_escape_string($_POST['name']);
$policy = '0'; // Default

$res = $sql->query('SELECT `policy` FROM `policies` WHERE `owner`=\''.$owner.'\' AND `addr`=\'00:00:00:00:00:00\''); // Get current deafult value from database
if($sql->error) die('Select Error '.$sql->error);

if($res->num_rows > 0) { // Only change the default when the default is set in the database
	$row = $res->fetch_row();
	$policy = $row[0];
}

$res = $sql->query('SELECT * FROM `policies` WHERE `owner`=\''.$owner.'\' AND `addr`=\''.$addr.'\''); // Check that the given owner/addr combination doesn't already exist
if($sql->error) die('Select Error '.$sql->error);
if($res->num_rows > 0) die('Device already registered');

$res = $sql->query('INSERT INTO `policies` VALUES (NULL, \''.$owner.'\', \''.$addr.'\', \''.$name.'\', \''.$policy.'\')'); // Register the device
if($sql->error) die('Insert Error '.$sql->error);
echo 'Device registered';
?>