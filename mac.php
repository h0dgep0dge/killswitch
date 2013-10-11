<?php
header("Location: /test.php?mac=".exec("./cli_getmac ".$_SERVER['REMOTE_ADDR']));
?>