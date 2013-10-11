function get_mac() {
    return "<?php echo exec("./cli_getmac ".$_SERVER['REMOTE_ADDR']) ?>";
}

