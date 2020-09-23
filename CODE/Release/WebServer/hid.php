<html>
<body>

<?php
// https://www.w3schools.com/php/php_superglobals_get.asp
$file = $_GET['active'];
echo "Received: " . $file. "<br>";

echo shell_exec("sudo chmod +x ../HID/* 2>&1");
echo shell_exec("sudo cp ../HID/".$file." ../HID/Selected.sh");

echo" Copyed ../HID/".$file.", To ../HID/Selected.sh";
?>
</body>
</html>