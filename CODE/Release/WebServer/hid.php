<?php
require('assets/Control.php');
home();
// https://www.w3schools.com/php/php_superglobals_get.asp
$file = $_GET['active'];
//echo "Received: " . $file. "<br>";

echo shell_exec("sudo chmod +x ../HID/* 2>&1");
echo shell_exec("sudo chmod 777 ../HID/* 2>&1");
echo shell_exec("sudo cp ../HID/".$file." ../HID/Selected.sh");

//echo"<script>alert('$file. Set as Active HID Script');</script> ";


// https://www.w3schools.com/php/php_file_create.asp
$txtfile = fopen("../HID/Selected.txt", "w") or die("Unable to open file!");
fwrite($txtfile, $file);
fclose($txtfile);


?>
