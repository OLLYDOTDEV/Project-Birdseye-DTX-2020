<?php
require('assets/Control.php');
Control();

echo shell_exec('sudo at now -f /projects/Project-Birdseye-DTX-2020/CODE/Release/WebServer/commands/off.sh ');

home();
?>
