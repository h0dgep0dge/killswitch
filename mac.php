var mac = "<?php
echo exec("./cli_getmac ".$_SERVER['REMOTE_ADDR']);
//echo $_SERVER['REMOTE_ADDR'];
?>";
loaded = 1;
