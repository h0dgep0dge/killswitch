<?php
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
if(!isset($_POST['addr']) && !isset($_POST['name']) && !isset($_POST['owner'])) header('Location: ./mac.html');
if(!isset($_POST['addr']) || !isset($_POST['name']) || !isset($_POST['owner'])) die('Server error');
if($_POST['addr'] == "00:00:00:00:00:00") die('Invalid mac address');
if(preg_match('/\A(?:[0-9a-fA-F]{2}:?){6}\z/',$_POST['addr']) <= 0) die('Invalid mac address');
if(preg_match('/\A[0-9a-zA-Z \'"]{1,256}\z/m',$_POST['name']) <= 0) die('Name with bad characters');

$sql = new mysqli('127.0.0.1',$username,$password,'killswitch'); // Add error detection
$addr = $sql->real_escape_string($_POST['addr']);
$owner = $sql->real_escape_string($_POST['owner']);
$name = $sql->real_escape_string($_POST['name']);
$policy = '0'; // Default

$res = $sql->query('SELECT `policy` FROM `policies` WHERE `owner`=\''.$owner.'\' AND `addr`=\'00:00:00:00:00:00\''); // Add error detection
if($res->num_rows > 0) {
	$row = $res->fetch_row();
	$policy = $row[0];
}

$res = $sql->query('SELECT * FROM `policies` WHERE `owner`=\''.$owner.'\' AND `addr`=\''.$addr.'\''); // Add error detection
if($res->num_rows > 0) die('Device already registered');

$res = $sql->query('INSERT INTO `policies` VALUES (NULL, \''.$owner.'\', \''.$addr.'\', \''.$name.'\', \''.$policy.'\')');
if($sql->affected_rows <= 0) die('Insert error occured');
echo 'Device registered';
?>