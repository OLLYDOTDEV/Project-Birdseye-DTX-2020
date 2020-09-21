<?php
function Control(){
echo
'

<html>
 <head>
 
<title>Control Page</title>
 
<link rel="stylesheet" href="assets/Styles.css">
</head>

 <body>
 <center>
<table>

 <tr>
<td colspan="6" class="control">Control panel</td>
 </tr>

 <tr>
<td> <a href="all.php">ALL </a></td>
<td> <a href="off.php">OFF </a></td>
<td> <a href="pir.php">PIR </a></td>
<td> <a href="ir.php">IR </a></td>
<td> <a href="kill.php">KILL </a> </td>
 </tr>
 <tr>
 <td> Processes: <br><span class="tooltiptext">F5 to update Processes</span></td>
 <td  colspan="5" >';
echo shell_exec("sudo ps aux | grep -ie RPI | grep -v grep");
echo '</td>
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

pagechange()
</script>
' ;
}
?> 
