
<?php
function Control(){
echo
'

<html>
<head>
 
<title>Control Page</title>
 
<link rel="stylesheet" href="assets/Styles.css">
</head>
 <center> 

<table>
<tr>
<td colspan="2" class="control">Control panel</td>
</tr>
<tr>
<td>
<table>

<tr>
<td colspan="2" >ROMS:</td>
<td> <a href="all.php">ALL </a></td>
<td> <a href="off.php">OFF </a></td>
<td> <a href="pir.php">PIR </a></td>
<td> <a href="ir.php">IR </a></td>
<td> <a href="kill.php">KILL </a> </td>
</tr>
<tr>
<td colspan="2"> Processes: <br><span class="tooltiptext">F5 to update Processes</span></td>
<td  colspan="6" >';
echo shell_exec("sudo ps aux | grep -ie RPI | grep -v grep");
echo '</td>
 </tr>
</table>
</td>
<td>
<table>

<tr>
<td colspan="100" > Select HID Script:</td>
</tr>
<tr>
';

$folderPath = "../HID";
$countFile = 0;
$totalFiles = glob($folderPath . "*");

 $countFile = count($totalFiles);

$files = array_diff(scandir($folderPath), array('..', '.','.htaccess','Selected.sh'));// removes the .. , . from linux file systems


$i = 0;
foreach($files as $key){
 $filepath= $folderPath.$key;
if($i  == 4) { // set amount of HID script per row 
 echo "</tr><tr>" ; 
 $i = 0;
}
  echo "<td> <a href='hid.php?active=".$key."'>".$key."</a></td>";
  $i++;
 }

echo '

</tr>

</table>
</td>
</tr>
</table> 
 </center>
 </body>
</html>

';
}

function home(){

echo'
<script>

function pagechange(){
    setTimeout(function() { 
        window.location.href = "index.php";
    }, 5000); 
}

pagechange();
</script>
' ;
}
?> 
