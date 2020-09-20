<?php
require('assets/Control.php');
Control();
echo shell_exec('sudo at now -f /projects/Project-Birdseye-DTX-2020/CODE/Release/WebServer/commands/all.sh');
//echo shell_exec('sudo at now -f /projects/Project-Birdseye-DTX-2020/CODE/Release/WebServer/commands/all.sh 2>&1'); for debug

home();
?>
