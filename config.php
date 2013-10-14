<?php
include('creds.php'); // Includes my creds, so i don't need to post them on the internet.
$sql = new mysqli('127.0.0.1',$username,$password,'killswitch');
$result = $sql->query('SELECT * FROM `policies` WHERE `owner`=\'1\'');
$conf = Array();
$o = 0;
while(($conf[$o] = $result->fetch_row()) != NULL) $o++;
for($i = 0;$i < $o;$i++) echo 'client={',$conf[$i][2],',',$conf[$i][3],'}',PHP_EOL;
?>