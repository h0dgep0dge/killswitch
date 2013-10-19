<?php
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
$sql = new mysqli('127.0.0.1',$username,$password,'killswitch');
$addr = '55:55:55:55:55:57';
$owner = '1';
$policy = '0';
// No input sanitation, yet.
$res = $sql->query('SELECT * FROM `policies` WHERE `owner`=\''.$owner.'\' AND `addr`=\''.$addr.'\'');
if($res->num_rows > 0) die('Device already registered');

$res = $sql->query('INSERT INTO `policies` VALUES (NULL, \'1\', \'55:55:55:55:55:57\', \'0\')');
if($sql->affected_rows < 1) die('Insert error occured');
echo 'Device registered';
?>
